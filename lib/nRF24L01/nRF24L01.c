#include "nRF24L01.h"

#include "../debug.h"
#include "../utils.h"

enum nRF24L01_Commands
{
    R_REGISTER   = 0x00,
    W_REGISTER   = 0x20,
    R_RX_PAYLOAD = 0x61,
    W_TX_PAYLOAD = 0xA0,
    FLUSH_TX     = 0xE1,
    FLUSH_RX     = 0xE2,
    REUSE_TX_PL  = 0xE3,
    NOP          = 0xFF,
};

#define REGISTER_MASK 0x1F

#define DUMMY 0xDE   // Dummy bits sent when we just want to receive

static void nRF24L01_setRegister(struct nRF24L01 *dev, enum nRF24L01_Register reg, uint8_t val)
{
    irq_lock_t lock;
    LOCK_IRQ(lock);

    GPIO_resetPin(dev->CSN);
    dev->spi_sendrecv(W_REGISTER);
    dev->spi_sendrecv(reg);
    dev->spi_sendrecv(val & REGISTER_MASK);
    GPIO_setPin(dev->CSN);

    UNLOCK_IRQ(lock);
}

static uint8_t nRF24L01_getRegister(struct nRF24L01 *dev, enum nRF24L01_Register reg)
{
    irq_lock_t lock;
    LOCK_IRQ(lock);

    GPIO_resetPin(dev->CSN);
    dev->spi_sendrecv(R_REGISTER);
    dev->spi_sendrecv(reg);
    uint8_t result = dev->spi_sendrecv(DUMMY) & REGISTER_MASK;
    GPIO_setPin(dev->CSN);
    return result;

    UNLOCK_IRQ(lock);
}

static void nRF24L01_modifyRegister(struct nRF24L01 *dev, enum nRF24L01_Register reg,
                                    uint8_t mask, uint8_t flags)
{
    uint8_t value = nRF24L01_getRegister(dev, reg);
    value = (value & mask) | flags;
    nRF24L01_setRegister(dev, reg, value);
}

bool nRF24L01_init(struct nRF24L01 *dev)
{
    assert(dev->channel <= 127);
}

void nRF24L01_interrupt(struct nRF24L01 *dev)
{
}
