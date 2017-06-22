#include "nRF24L01.h"
#include "nRF24L01_internal.h"

#include "../debug.h"
#include "../utils.h"

/**
 * @brief nRF24L01 write operation help function
 *
 * @param dev    [in/out] nRF24L01 device object
 * @param cmd    [in]     Command byte. Shoudl be a write operation
 * @param dataIn [in]     Pointer to data bytes to write operation. Can be NULL if len == 0. Data should be in LSByte to
 *                        MSByte order, with MSBit in each byte first.
 * @param len    [in]     How many data bytes. From 0 to maximum defined by the command
 */
static void nRF24L01_writeOp(struct nRF24L01 *dev, uint8_t cmd, const uint8_t *dataIn, size_t len)
{
    irq_lock_t lock;
    LOCK_IRQ(lock);

    GPIO_resetPin(&dev->CSN);
    dev->status = dev->spi_sendrecv(cmd);
    for (size_t i = 0; i < len; ++i)
        dev->spi_sendrecv(dataIn[i]);
    GPIO_setPin(&dev->CSN);

    UNLOCK_IRQ(lock);
}

/**
 * @brief nRF24L01 read operation help function
 *
 * @param dev     [in/out] nRF24L01 device object
 * @param cmd     [in]     Command byte. Should be a read operation.
 * @param dataOut [out]    Pointer where received data bytes are stored. At maximum len bytes are stored. Caller is
 *                         responsible that we do not write out of bounds. Data is written LSByte to MSByte, with MSBit
 *                         in each byte first.
 * @param len     [in]     How many data bytes we expect to read back from the operation
 */
static void nRF24L01_readOp(struct nRF24L01 *dev, uint8_t cmd, uint8_t *dataOut, size_t len)
{
    irq_lock_t lock;
    LOCK_IRQ(lock);

    GPIO_resetPin(&dev->CSN);
    dev->status = dev->spi_sendrecv(cmd);
    for (size_t i = 0; i < len; ++i)
        dataOut[i] = dev->spi_sendrecv(DUMMY);
    GPIO_setPin(&dev->CSN);

    UNLOCK_IRQ(lock);
}

static void nRF24L01_NOP(struct nRF24L01 *dev)
{
    nRF24L01_writeOp(dev, NOP, NULL, 0);
}

static void nRF24L01_writeTxPayload(struct nRF24L01 *dev, const uint8_t *data, size_t len)
{
    assert(len <= 32);
    nRF24L01_writeOp(dev, W_TX_PAYLOAD, data, len);
}

static void nRF24L01_getRxPayload(struct nRF24L01 *dev, uint8_t *data, size_t len)
{
    assert(len <= 32);
    nRF24L01_readOp(dev, R_RX_PAYLOAD, data, len);
}

/**
 * @brief Helper function to write to 8 bit registers on the nRF24L01
 */
static void nRF24L01_setRegister8(struct nRF24L01 *dev, enum nRF24L01_Register reg, uint8_t val)
{
    nRF24L01_writeOp(dev, W_REGISTER | (reg & REGISTER_MASK), &val, sizeof(val));
}

/**
 * @brief Helper function to read 8 bit registers on the nRF24L01
 */
static uint8_t nRF24L01_getRegister8(struct nRF24L01 *dev, enum nRF24L01_Register reg)
{
    uint8_t result;
    nRF24L01_readOp(dev, R_REGISTER | (reg & REGISTER_MASK), &result, sizeof(result));
    return result;
}

/**
 * @brief Helper functions for read-modify-write on 8 bit nRF24L01 register
 */
__attribute__((unused))
static void nRF24L01_modifyRegister8(struct nRF24L01 *dev, enum nRF24L01_Register reg,
                                     uint8_t mask, uint8_t flags)
{
    uint8_t value = nRF24L01_getRegister8(dev, reg);
    value = (value & mask) | flags;
    nRF24L01_setRegister8(dev, reg, value);
}

bool nRF24L01_init(struct nRF24L01 *dev)
{
    assert(dev->channel <= 127);

    nRF24L01_setRegister8(dev, RF_CH_Reg, dev->channel & 0x7f);
    nRF24L01_setRegister8(dev, EN_AA_Reg, (dev->useACK) ? EN_AA_ENAA_All : 0);
    nRF24L01_setRegister8(dev, EN_RXADDR_Reg, EN_RXADDR_ERX_P1);

    uint8_t rf_setup = RF_SETUP_LNA_HCURR;
    rf_setup |= (dev->airDataRate == nRF24L01_2Mbps) ? RF_SETUP_RF_DR : 0;
    switch (dev->power)
    {
        case nRF24L01_TXPower_Minus18dBm:
            rf_setup |= RF_SETUP_RF_PWR_Minus18dBm;
            break;
        case nRF24L01_TXPower_Minus12dBm:
            rf_setup |= RF_SETUP_RF_PWR_Minus12dBm;
            break;
        case nRF24L01_TXPower_Minus6dBm:
            rf_setup |= RF_SETUP_RF_PWR_Minus6dBm;
            break;
        case nRF24L01_TXPower_Minus0dBm:
            rf_setup |= RF_SETUP_RF_PWR_Minus0dBm;
            break;
    }
    nRF24L01_setRegister8(dev, RF_SETUP_Reg, rf_setup);

    uint8_t config = CONFIG_MASK_MAX_RT;
    if (dev->useCRC == nRF24L01_CRC)
        config |= CONFIG_EN_CRC;

    if (dev->mode == nRF24L01_RX)
    {
        nRF24L01_setRegister8(dev, CONFIG_Reg, config | CONFIG_MASK_TX_DS | CONFIG_PRIM_RX);
    }
    else if (dev->mode == nRF24L01_TX)
    {
        nRF24L01_setRegister8(dev, CONFIG_Reg, (config | CONFIG_MASK_RX_DR) & ~CONFIG_PRIM_RX);
    }
    else
    {
        die("nRF24L01: Bad mode!");
    }

    return true;
}

void nRF24L01_interrupt(struct nRF24L01 *dev)
{
    if (dev->mode == nRF24L01_RX)
    {
        for (uint8_t fifo_status = nRF24L01_getRegister8(dev, FIFO_STATUS_Reg);
             !(fifo_status & FIFO_STATUS_RX_EMPTY);
             fifo_status = nRF24L01_getRegister8(dev, FIFO_STATUS_Reg))
        { // Until RX FIFO is empty
            // TODO: data word width setting here, determining how much we should be fetching at a time
            uint8_t recv;
            nRF24L01_getRxPayload(dev, &recv, sizeof(recv));
            if (dev->rx_cb != NULL)
                dev->rx_cb(dev, &recv, sizeof(recv));
        }
        // Clear RX_DR flag
        nRF24L01_setRegister8(dev, STATUS_Reg, STATUS_RX_DR);
    }
    else if (dev->mode == nRF24L01_TX)
    {
        // Clear TX_DS flag
        nRF24L01_setRegister8(dev, STATUS_Reg, STATUS_TX_DS);
    }
}
