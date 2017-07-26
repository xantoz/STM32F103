#include "snesCon_client.h"

#include "exti.h"

bool snesCon_client_init(const struct snesCon_client * const client)
{
    irq_lock_t lock;
    LOCK_IRQ(lock);

    // remember to set pins up to work in 5V tolerant mode! (no pull-ups)
    GPIO_setMODE_setCNF(&client->pinDef.data, GPIO_MODE_Output_10MHz, GPIO_Output_CNF_GPOpenDrain);
    GPIO_resetPin(&client->pinDef.data);

    // init EXTI for LATCH and CLK
    GPIO_setMODE_setCNF(&client->pinDef.latch, GPIO_MODE_Input, GPIO_Input_CNF_Floating);
    GPIO_setMODE_setCNF(&client->pinDef.clock, GPIO_MODE_Input, GPIO_Input_CNF_Floating);
    EXTI_enableInterrupt(&client->pinDef.latch, EXTI_RISING);
    EXTI_enableInterrupt(&client->pinDef.clock, EXTI_RISING);

    UNLOCK_IRQ(lock);

    return true;
}

void snesCon_client_update(struct snesCon_client *client, snesCon_btn_t btn)
{
    client->reg = btn;
}

static void snesCon_client_driveData(const struct snesCon_client * const client)
{
    if (client->latchReg & 0x1)
        GPIO_setPin(&client->pinDef.data);
    else
        GPIO_resetPin(&client->pinDef.data);
}

void snesCon_client_latch(struct snesCon_client *client)
{
    // Button pressed == low, thus invert
    client->latchReg = ~client->reg; // Hopefully atomic *knock on wood* (== check the asm output)
    snesCon_client_driveData(client);
}

void snesCon_client_clock(struct snesCon_client *client)
{
    client->latchReg >>= 1;
    snesCon_client_driveData(client);
}
