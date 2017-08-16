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
        GPIO_setMODE_setCNF(outputs[i], GPIO_MODE_Output_10MHz, GPIO_Output_CNF_GPOpenDrain);
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

    // init EXTI for enable and select. Update on both flanks (74HC157 behavior is defined for
    // states of the pin, not just a single flank)
    EXTI_enableInterrupt(&client->pin.enable, EXTI_BOTH);
    EXTI_enableInterrupt(&client->pin.select, EXTI_BOTH);

    UNLOCK_IRQ(lock);

    return true;
}

void pceCon_client_update(struct pceCon_client *client, pceCon_btn_t btn)
{
    client->btn = ~btn; // Invert since controller buttons are active low
}

/**
 * @brief Helper that sets all output pins to low
 */
static inline void pceCon_client_resetOutputs(struct pceCon_client *client)
{
    GPIO_resetPin(&client->pin.output1Y);
    GPIO_resetPin(&client->pin.output2Y);
    GPIO_resetPin(&client->pin.output3Y);
    GPIO_resetPin(&client->pin.output4Y);
}

/**
 * @brief Helper that sets all four output pins based on the 4 LSB of settings
 *
 * @param client   [in] pceCon client object
 * @param settings [in] Reflect 4 LSB on output pins
 */
static INLINE void pceCon_client_setOutputs(struct pceCon_client *client, uint8_t settings)
{
    GPIO_setBit(&client->pin.output1Y, settings & (1 << 0));
    GPIO_setBit(&client->pin.output2Y, settings & (1 << 1));
    GPIO_setBit(&client->pin.output3Y, settings & (1 << 2));
    GPIO_setBit(&client->pin.output4Y, settings & (1 << 3));
}

static INLINE void pceCon_client_driveOutputs_2btnpad(struct pceCon_client *client)
{
    if (GPIO_read(&client->pin.select))
    {
        // select pin high == I II Start Select
        pceCon_client_setOutputs(client, (client->btn >> 4) & 0x0f);
    }
    else
    {
        // select pin low == DPAD
        pceCon_client_setOutputs(client, client->btn & 0x0f);
    }
}

static inline void pceCon_client_driveOutputs_6btnpad(struct pceCon_client *client)
{
    if (client->pollCtr & 1)
    {
        // Output D-pad all down + 4 extra buttons every odd poll
        if (GPIO_read(&client->pin.select))
        {
            // select pin high == III IV V VI
            pceCon_client_setOutputs(client, (client->btn >> 8) & 0x0f);
        }
        else
        {
            // select pin low == DPAD all pushed down (how the console tells this extra poll from the rest)
            pceCon_client_setOutputs(client, 0x00);
        }
    }
    else
    {
        // Output regular buttons on even polls
        pceCon_client_driveOutputs_2btnpad(client);
    }
}

static void pceCon_client_driveOutputs(struct pceCon_client *client)
{
    if (!client->enabled)
    {
        pceCon_client_resetOutputs(client);
        return;
    }

    if (pceCon_2BTN == client->mode)
        pceCon_client_driveOutputs_2btnpad(client);
    else /* if (pceCon_6BTN == client->mode) */
        pceCon_client_driveOutputs_6btnpad(client);
}

void pceCon_client_enable(struct pceCon_client *client)
{
    irq_lock_t lock;

    LOCK_IRQ(lock);
    if (!GPIO_read(&client->pin.enable))                    // Active low
    {
        ++client->pollCtr;
        client->enabled = true;
    }
    else
    {
        client->enabled = false;
    }
    UNLOCK_IRQ(lock);

    pceCon_client_driveOutputs(client);
}

void pceCon_client_select(struct pceCon_client *client)
{
    pceCon_client_driveOutputs(client);
}
