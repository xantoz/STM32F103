#include "pceCon_client.h"

#include "utils.h"
#include "exti.h"

bool pceCon_client_init(struct pceCon_client *client)
{
    irq_lock_t lock;
    LOCK_IRQ(lock);

    // remember to set pins up to work in 5V tolerant mode! (no pull-ups)

    struct GPIO_PortPin const * const outputs[] = {
        &client->pin.output1Y,
        &client->pin.output2Y,
        &client->pin.output3Y,
        &client->pin.output4Y
    };

    for (unsigned i = 0; i < ARRAYLEN(outputs); ++i)
    {
        GPIO_resetPin(outputs[i]);
        GPIO_setMODE_setCNF(outputs[i], GPIO_MODE_Output_50MHz, GPIO_Output_CNF_GPPushPull);
    }

    client->btn = 0;
    client->enabled = false;
    client->pollCtr = 0;

    // Setup pins for enable and select + read current state of pins
    GPIO_setMODE_setCNF(&client->pin.select, GPIO_MODE_Input, GPIO_Input_CNF_Floating);
    GPIO_setMODE_setCNF(&client->pin.enable, GPIO_MODE_Input, GPIO_Input_CNF_Floating);
    pceCon_client_enable(client);
    pceCon_client_select(client);

    client->pollCtr = 0; // Reset pollCtr (we do not wish to count the manual call to pceCon_client_enable)

    // init EXTI for enable and select. The select pin interrupt is triggered on both flanks,
    // however because of latency concerns the enable pin is not properly emulated (pulling it
    // high will not block output), and is instead simply treated as a clock signal, positive
    // flank (negative flank would be more correct, but triggering on the positive flank gives
    // us more time to react)
    EXTI_enableInterrupt(&client->pin.enable, EXTI_RISING);
    EXTI_enableInterrupt(&client->pin.select, EXTI_BOTH);

    UNLOCK_IRQ(lock);

    return true;
}

void pceCon_client_update(struct pceCon_client *client, const pceCon_btn_t btn)
{
    // Since the order of buttons in enum pceCon_buttons is laid out in an order that is meant to
    // map conveniently to enum snesCon_button, the bit order of the D-pad buttons (UDLR) in btn
    // does not correspond to the order that we shall be using when outputting them to 1Y,2Y,3Y,4Y
    // (URDL). We remap the D-PAD buttons here. That way, the more time-sensitive code that needs to
    // respond to interrupts can be written more efficiently.
    pceCon_btn_t remapBtn = btn & 0xff0f;
    remapBtn |= (((!!(btn & pceCon_BUTTON_Up))    << 4) |
                 ((!!(btn & pceCon_BUTTON_Right)) << 5) |
                 ((!!(btn & pceCon_BUTTON_Down))  << 6) |
                 ((!!(btn & pceCon_BUTTON_Left))  << 7));
    // Set the upper 4 DPAD-bits in the high byte, since all DPAD buttons pressed is what
    // signifies the second (III IV V VI) poll of a 6 button pad.
    remapBtn |= 0xf000;

    client->btn = ~remapBtn; // Invert since controller buttons are active low.
}

/**
 * @brief Helper that sets all four output pins based on the 4 LSB of settings
 *
 * @param client   [in] pceCon client object
 * @param settings [in] Reflect 4 LSB on output pins
 */
static inline void pceCon_client_setOutputs(struct pceCon_client *client, uint8_t settings)
{
    GPIO_setBit(&client->pin.output1Y, settings & (1 << 0));
    GPIO_setBit(&client->pin.output2Y, settings & (1 << 1));
    GPIO_setBit(&client->pin.output3Y, settings & (1 << 2));
    GPIO_setBit(&client->pin.output4Y, settings & (1 << 3));
}

static void pceCon_client_driveOutputs(struct pceCon_client *client, uint32_t cntr)
{
    uint32_t shift = (GPIO_read(&client->pin.select)) ? 4 : 0;
    if ((pceCon_6BTN == client->mode) && (cntr & 1))
        shift += 8;

    pceCon_client_setOutputs(client, client->btn >> shift);
}

void pceCon_client_enable(struct pceCon_client *client)
{
    irq_lock_t lock;
    LOCK_IRQ(lock);

    pceCon_client_driveOutputs(client, client->pollCtr + 1);
    ++client->pollCtr;

    UNLOCK_IRQ(lock);
}

void pceCon_client_select(struct pceCon_client *client)
{
    pceCon_client_driveOutputs(client, client->pollCtr);
}
