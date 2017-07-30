#include "nRF24L01.h"
#include "nRF24L01_internal.h"

#include "../debug.h"
#include "../utils.h"

/**
 * @brief nRF24L01 write operation help function
 *
 * @param dev    [in/out] nRF24L01 device object
 * @param cmd    [in]     Command byte. Should be a write operation
 * @param dataIn [in]     Pointer to data bytes to write operation. Can be NULL if len == 0. Data should be in LSByte to
 *                        MSByte order, with MSBit in each byte first.
 * @param len    [in]     How many data bytes. From 0 to maximum defined by the command
 */
static void nRF24L01_writeOp(struct nRF24L01 *dev, uint8_t cmd, const uint8_t *dataIn, size_t len)
{
    GPIO_resetPin(&dev->conf->CSN);
    dev->status = dev->conf->spi_sendrecv(cmd);
    for (size_t i = 0; i < len; ++i)
        dev->conf->spi_sendrecv(dataIn[i]);
    GPIO_setPin(&dev->conf->CSN);
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
    GPIO_resetPin(&dev->conf->CSN);
    dev->status = dev->conf->spi_sendrecv(cmd);
    for (size_t i = 0; i < len; ++i)
        dataOut[i] = dev->conf->spi_sendrecv(DUMMY);
    GPIO_setPin(&dev->conf->CSN);
}

static void nRF24L01_NOP(struct nRF24L01 *dev)
{
    nRF24L01_writeOp(dev, NOP, NULL, 0);
}

static void nRF24L01_flushTx(struct nRF24L01 *dev)
{
    nRF24L01_writeOp(dev, FLUSH_TX, NULL, 0);
}

static void nRF24L01_flushRx(struct nRF24L01 *dev)
{
    nRF24L01_writeOp(dev, FLUSH_RX, NULL, 0);
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

// TODO: * add options for active selecting pipes and their addresses
//       * add option to select TX addr
//       * special mode where we keep spraying the same number until the next send operation, or we
//         request to turn it off
bool nRF24L01_init(struct nRF24L01_Options const * const options, struct nRF24L01 *dev)
{
    dev->status = 0;
    dev->conf = options;

    assert(dev->conf->channel <= 127);
    assert(0 < dev->conf->payloadWidth && dev->conf->payloadWidth <= 32);
    assert(dev->conf->spi_sendrecv != NULL);

    // TODO: toggle the power line here to ensure it is truly reset? (will eat more GPIO... +
    // require a transistor)

    // Setup the GPIO outputs
    GPIO_setMODE_setCNF(&dev->conf->CE,  GPIO_MODE_Output_50MHz, GPIO_Output_CNF_GPPushPull);
    GPIO_setMODE_setCNF(&dev->conf->CSN, GPIO_MODE_Output_50MHz, GPIO_Output_CNF_GPPushPull);
    GPIO_resetPin(&dev->conf->CE); // Active high
    delay_us(4);
    GPIO_setPin(&dev->conf->CSN);  // Active low

    delay_us(10300); // 10.3 ms for Power on reset (when powering on for the first time) (TODO: move out of this fn)

    nRF24L01_setRegister8(dev, RF_CH_Reg, dev->conf->channel & 0x7f);
    nRF24L01_setRegister8(dev, EN_AA_Reg, (dev->conf->useACK == nRF24L01_ACK) ? EN_AA_ENAA_All : EN_AA_ENAA_None);
    nRF24L01_setRegister8(dev, EN_RXADDR_Reg, EN_RXADDR_ERX_P0);
    nRF24L01_setRegister8(dev, RX_PW_P0_Reg, dev->conf->payloadWidth & 0x1f);

    // TODO: RX/TX addr settings (need changes in the struct. Currently we just use the reset
    // defaults) + We would like to be able to change the address settings on the fly, so needs to
    // be copied to the RW part of the struct

    // TODO: apply retransmission setting from conf

    uint8_t rf_setup = RF_SETUP_LNA_HCURR;
    rf_setup |= (dev->conf->airDataRate == nRF24L01_2Mbps) ? RF_SETUP_RF_DR : 0;
    switch (dev->conf->power)
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

    uint8_t config = CONFIG_PWR_UP;
    if (dev->conf->useCRC == nRF24L01_CRC)
        config |= CONFIG_EN_CRC;

    if (dev->conf->mode == nRF24L01_RX)
    {
        nRF24L01_setRegister8(dev, CONFIG_Reg, config | CONFIG_MASK_TX_DS | CONFIG_PRIM_RX);
        delay_us(1500);                  // 1.5 ms to power up
        GPIO_setPin(&dev->conf->CE);     // In 103 us we are monitoring (but no need to delay)
    }
    else if (dev->conf->mode == nRF24L01_TX)
    {
        nRF24L01_setRegister8(dev, CONFIG_Reg, (config | CONFIG_MASK_RX_DR) & ~CONFIG_PRIM_RX);
        delay_us(1500);                  // 1.5 ms to power up
    }
    else
    {
        die("nRF24L01: Bad mode!");
    }

    // Flush FIFOs here (actually before setting CE)
    nRF24L01_flushTx(dev);
    nRF24L01_flushRx(dev);

    return true;
}

void nRF24L01_send(struct nRF24L01 *dev, const void *payload)
{
    assert(dev->conf->mode == nRF24L01_TX);

    // TODO: check if TX FIFO full, and fail/block in this case
    irq_lock_t lock;
    LOCK_IRQ(lock);

    nRF24L01_writeTxPayload(dev, payload, dev->conf->payloadWidth);

    // Toggle CE pin to send TX FIFO
    GPIO_setPin(&dev->conf->CE);
    delay_us(10);
    GPIO_resetPin(&dev->conf->CE);

    UNLOCK_IRQ(lock);
}

static void nRF24L01_TX_DS_handler(struct nRF24L01 *dev)
{
    assert(dev->conf->mode == nRF24L01_TX, "Got unexpected TX_DS interrupt (not in TX mode)");

    // TODO: atomically set flag in device struct to notify that we are now free to send more?

    if (dev->conf->tx_ds_cb != NULL)
        dev->conf->tx_ds_cb(dev);

    // Clear TX_DS flag
    nRF24L01_setRegister8(dev, STATUS_Reg, STATUS_TX_DS);
}

void nRF24L01_rxDispatchFIFO(struct nRF24L01 *dev)
{
    assert(dev->conf->mode == nRF24L01_RX, "RX mode only function");

    while (!(nRF24L01_getRegister8(dev, FIFO_STATUS_Reg) & FIFO_STATUS_RX_EMPTY))
    { // Until RX FIFO is empty
        uint8_t recv[dev->conf->payloadWidth];
        nRF24L01_getRxPayload(dev, &recv[0], sizeof(recv));
        // get the pipe number and send it on to rx_cb
        const uint8_t pipeNo = (dev->status & STATUS_RX_P_NO) >> STATUS_RX_P_NO_Pos;
        if (dev->conf->rx_cb != NULL)
            dev->conf->rx_cb(dev, pipeNo, &recv, sizeof(recv));
    }
}

static void nRF24L01_RX_DR_handler(struct nRF24L01 *dev)
{
    assert(dev->conf->mode == nRF24L01_RX, "Got unexpected RX_DR interrupt (not in RX mode)");

    nRF24L01_rxDispatchFIFO(dev);

    // Clear RX_DR flag
    nRF24L01_setRegister8(dev, STATUS_Reg, STATUS_RX_DR);
}

static void nRF24L01_MAX_RT_handler(struct nRF24L01 *dev)
{
    // Call callback if any
    if (dev->conf->max_rt_cb != NULL)
        dev->conf->max_rt_cb(dev);

    // Clear MAX_RT flag
    nRF24L01_setRegister8(dev, STATUS_Reg, STATUS_MAX_RT);
}

void nRF24L01_interrupt(struct nRF24L01 *dev)
{
    nRF24L01_NOP(dev);     // Update dev->status
    const uint8_t status = dev->status;

    // Interrupt handler dispatch
    if (status & STATUS_RX_DR)  nRF24L01_RX_DR_handler(dev);
    if (status & STATUS_TX_DS)  nRF24L01_TX_DS_handler(dev);
    if (status & STATUS_MAX_RT) nRF24L01_MAX_RT_handler(dev);
}
