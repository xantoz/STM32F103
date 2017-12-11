#include "nRF24L01.h"
#include "nRF24L01_internal.h"

#include <debug.h>
#include <utils.h>
#include <exti.h>

/**
 * @brief nRF24L01 write operation help function
 *
 * @param dev    [in]     nRF24L01 device object
 * @param cmd    [in]     Command byte. Should be a write operation
 * @param dataIn [in]     Pointer to data bytes to write operation. Can be NULL if len == 0. Data should be in LSByte to
 *                        MSByte order, with MSBit in each byte first.
 * @param len    [in]     How many data bytes. From 0 to maximum defined by the command
 *
 * @return STATUS reg contents (as returned on SPI MISO)
 */
static uint8_t nRF24L01_writeOp(const struct nRF24L01 *dev, uint8_t cmd, const uint8_t *dataIn, size_t len)
{
    GPIO_resetPin(&dev->CSN);
    uint8_t status = dev->spi_sendrecv(cmd);
    for (size_t i = 0; i < len; ++i)
        dev->spi_sendrecv(dataIn[i]);
    GPIO_setPin(&dev->CSN);
    return status;
}

/**
 * @brief nRF24L01 read operation help function
 *
 * @param dev     [in]     nRF24L01 device object
 * @param cmd     [in]     Command byte. Should be a read operation.
 * @param dataOut [out]    Pointer where received data bytes are stored. At maximum len bytes are stored. Caller is
 *                         responsible that we do not write out of bounds. Data is written LSByte to MSByte, with MSBit
 *                         in each byte first.
 * @param len     [in]     How many data bytes we expect to read back from the operation
 *
 * @return STATUS reg contents (as returned on SPI MISO)
 */
static uint8_t nRF24L01_readOp(const struct nRF24L01 *dev, uint8_t cmd, uint8_t *dataOut, size_t len)
{
    GPIO_resetPin(&dev->CSN);
    uint8_t status = dev->spi_sendrecv(cmd);
    for (size_t i = 0; i < len; ++i)
        dataOut[i] = dev->spi_sendrecv(DUMMY);
    GPIO_setPin(&dev->CSN);
    return status;
}

static uint8_t nRF24L01_NOP(const struct nRF24L01 *dev)
{
    return nRF24L01_writeOp(dev, NOP, NULL, 0);
}

static uint8_t nRF24L01_flushTx(const struct nRF24L01 *dev)
{
    return nRF24L01_writeOp(dev, FLUSH_TX, NULL, 0);
}

static uint8_t nRF24L01_flushRx(const struct nRF24L01 *dev)
{
    return nRF24L01_writeOp(dev, FLUSH_RX, NULL, 0);
}

static uint8_t nRF24L01_writeTxPayload(const struct nRF24L01 *dev, const uint8_t *data, size_t len)
{
    assert(len <= 32);
    return nRF24L01_writeOp(dev, W_TX_PAYLOAD, data, len);
}

static uint8_t nRF24L01_getRxPayload(const struct nRF24L01 *dev, uint8_t *data, size_t len)
{
    assert(len <= 32);
    return nRF24L01_readOp(dev, R_RX_PAYLOAD, data, len);
}

/**
 * @brief Helper function to write to longer-than-8-bit registers on the nRF24L01
 *
 * @return STATUS reg contents (as returned on SPI MISO)
 */
static uint8_t nRF24L01_setRegister(const struct nRF24L01 *dev, enum nRF24L01_Register reg,
                                    const uint8_t *buf, size_t len)
{
    return nRF24L01_writeOp(dev, W_REGISTER | (reg & REGISTER_MASK), buf, len);
}

/**
 * @brief Helper function to read from longer-than-8-bit registers on the nRF24L01
 *
 * @return STATUS reg contents (as returned on SPI MISO)
 */
static uint8_t nRF24L01_getRegister(const struct nRF24L01 *dev, enum nRF24L01_Register reg,
                                    const uint8_t *buf, size_t len)
{
    return nRF24L01_readOp(dev, R_REGISTER | (reg & REGISTER_MASK), buf, len);
}

/**
 * @brief Helper function to write to 8 bit registers on the nRF24L01
 *
 * @return STATUS reg contents (as returned on SPI MISO)
 */
static uint8_t nRF24L01_setRegister8(const struct nRF24L01 *dev, enum nRF24L01_Register reg, uint8_t val)
{
    return nRF24L01_setRegister(dev, reg, &val, sizeof(val));
}

/**
 * @brief Helper function to read 8 bit registers on the nRF24L01
 *
 * @return STATUS reg contents (as returned on SPI MISO)
 */
static uint8_t nRF24L01_getRegister8(const struct nRF24L01 *dev, enum nRF24L01_Register reg, uint8_t *result)
{
    return nRF24L01_getRegister(dev, reg, result, sizeof(*result));
}

/**
 * @brief Helper functions for read-modify-write on 8 bit nRF24L01 register
 *
 * @return STATUS reg contents (as returned on SPI MISO)
 */
__attribute__((unused))
static uint8_t nRF24L01_modifyRegister8(const struct nRF24L01 *dev, enum nRF24L01_Register reg,
                                        uint8_t mask, uint8_t flags)
{
    uint8_t value;
    nRF24L01_getRegister8(dev, reg, &value);
    value = (value & mask) | flags;
    return nRF24L01_setRegister8(dev, reg, value);
}

static void nRF24L01_setLongAddress_impl(const struct nRF24L01 *dev,
                                         enum nRF24L01_Register reg, const uint8_t *addr)
{
    size_t len =
        (dev->addressWidth == nRF24L01_AddressWidth_3bytes) ? 3 :
        (dev->addressWidth == nRF24L01_AddressWidth_4bytes) ? 4 : 5;
    nRF24L01_setRegister(dev, reg, addr, len);
}

void nRF24L01_setTxAddress(const struct nRF24L01 *dev, const uint8_t *addr)
{
    // TODO: Automatically set RX_P0 addr to the same value when in Enhanced Shockburst mode?
    nRF24L01_setLongAddress_impl(dev, TX_ADDR_Reg, addr);
}

void nRF24L01_setRxP0Address(const struct nRF24L01 *dev, const uint8_t *addr)
{
    nRF24L01_setLongAddress_impl(dev, RX_ADDR_P0_Reg, addr);
}

void nRF24L01_setRxP1Address(const struct nRF24L01 *dev, const uint8_t *addr)
{
    nRF24L01_setLongAddress_impl(dev, RX_ADDR_P1_Reg, addr);
}

void nRF24L01_setRxP2Address(const struct nRF24L01 *dev, uint8_t addr)
{
    nRF24L01_setRegister8(dev, RX_ADDR_P2_Reg, addr);
}

void nRF24L01_setRxP3Address(const struct nRF24L01 *dev, uint8_t addr)
{
    nRF24L01_setRegister8(dev, RX_ADDR_P3_Reg, addr);
}

void nRF24L01_setRxP4Address(const struct nRF24L01 *dev, uint8_t addr)
{
    nRF24L01_setRegister8(dev, RX_ADDR_P4_Reg, addr);
}

void nRF24L01_setRxP5Address(const struct nRF24L01 *dev, uint8_t addr)
{
    nRF24L01_setRegister8(dev, RX_ADDR_P5_Reg, addr);
}

/**
 * Helper function for nRF24L01_init
 */
static uint8_t nRF24L01_init_getRetransmitFlags(struct nRF24L01 const * const conf)
{
    const uint8_t count = conf->retransmit.count & 0x0f;
    const uint8_t delay = conf->retransmit.delay & 0x0f;
    return (delay << SETUP_RETR_ARD_Pos) | (count << SETUP_RETR_ARC_Pos);
}

// TODO: * add option to select TX addr
//       * special mode where we keep spraying the same number until the next send operation, or we
//         request to turn it off
bool nRF24L01_init(const struct nRF24L01 *dev)
{
    assert(dev->channel <= 127);

    assert(dev->retransmit.count <= 15);
    assert(nRF24L01_Retransmit_Delay_250us <= dev->retransmit.delay &&
           dev->retransmit.delay <= nRF24L01_Retransmit_Delay_4000us);
    assert(dev->spi_sendrecv != NULL);

    // Setup the GPIO outputs
    GPIO_setMODE_setCNF(&dev->CE,  GPIO_MODE_Output_50MHz, GPIO_Output_CNF_GPPushPull);
    GPIO_setMODE_setCNF(&dev->CSN, GPIO_MODE_Output_50MHz, GPIO_Output_CNF_GPPushPull);
    GPIO_resetPin(&dev->CE); // Active high
    delay_us(4);
    GPIO_setPin(&dev->CSN);  // Active low

    EXTI_enableInterrupt(&dev->IRQ, EXTI_FALLING);
    GPIO_setMODE_setCNF(&dev->IRQ, GPIO_MODE_Input, GPIO_Input_CNF_Floating);

    delay_us(10300); // 10.3 ms for Power on reset (when powering on for the first time) (TODO: move out of this fn)

    nRF24L01_setRegister8(dev, CONFIG_Reg, 0);              // Ensure nRF24L01 is in power down state

    // Set channel
    nRF24L01_setRegister8(dev, RF_CH_Reg, dev->channel & 0x7f);

    // Set address width
    const uint8_t addressWidth_flg =
        (dev->addressWidth == nRF24L01_AddressWidth_3bytes) ? SETUP_AW_AW_3bytes :
        (dev->addressWidth == nRF24L01_AddressWidth_4bytes) ? SETUP_AW_AW_4bytes : SETUP_AW_AW_5bytes;
    nRF24L01_setRegister8(dev, SETUP_AW_Reg, addressWidth_flg);

    // Explicitly set RX & TX addresses to defaults (useful in reset situations, or with clone chips
    // with different default values). For anything but the default addresses the user will have to
    // call nRF24L01_setTxAddress, nRF24L01_setRxP0Address, etc. themselves (for now)
    nRF24L01_setTxAddress(dev, (uint8_t[]){0xE7, 0xE7, 0xE7, 0xE7, 0xE7});
    nRF24L01_setRxP0Address(dev, (uint8_t[]){0xE7, 0xE7, 0xE7, 0xE7, 0xE7});
    nRF24L01_setRxP1Address(dev, (uint8_t[]){0xC2, 0xC2, 0xC2, 0xC2, 0xC2});
    nRF24L01_setRxP2Address(dev, 0xC3);
    nRF24L01_setRxP3Address(dev, 0xC4);
    nRF24L01_setRxP4Address(dev, 0xC5);
    nRF24L01_setRxP5Address(dev, 0xC6);

    // Set up RX pipes
    uint8_t pipeEnable = 0;
    uint8_t pipeAutoAck = 0;
    static_assert(ARRAYLEN(dev->pipe) <= 6, "Too many pipes in nRF24L01 settings");
    for (unsigned i = 0; i < ARRAYLEN(dev->pipe); ++i)
    {
        assert(0 <= dev->pipe[i].payloadWidth && dev->pipe[i].payloadWidth <= 32);
        // Set payloadWidth for pipe i
        uint8_t width = 0;
        if (dev->pipe[i].enable == true)
        {
            width = dev->pipe[i].payloadWidth & 0x1f;
            width = (width == 0) ? 1 : width; // If not specified (0) set to 1 byte
            pipeEnable |= EN_RXADDR_ERX_Px(i);
        }
        nRF24L01_setRegister8(dev, RX_PW_Px_Reg(i), width);

        if (dev->pipe[i].autoAck == true)
            pipeAutoAck |= EN_AA_ENAA_Px(i);
    }
    nRF24L01_setRegister8(dev, EN_RXADDR_Reg, pipeEnable);
    nRF24L01_setRegister8(dev, EN_AA_Reg, pipeAutoAck);

    // Set retransmit flags
    uint8_t retransmit_flags = nRF24L01_init_getRetransmitFlags(dev);
    nRF24L01_setRegister8(dev, SETUP_RETR_Reg, retransmit_flags);

    // RF setup (transmit power and data rate)
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

    // Flush FIFOs here and reset any pending interrupts
    nRF24L01_flushTx(dev);
    nRF24L01_flushRx(dev);
    nRF24L01_setRegister8(dev, STATUS_Reg, STATUS_RX_DR | STATUS_TX_DS | STATUS_MAX_RT);

    uint8_t config = CONFIG_PWR_UP;
    if (dev->useCRC == nRF24L01_CRC)
        config |= CONFIG_EN_CRC;

    if (dev->mode == nRF24L01_RX)
    {
        nRF24L01_setRegister8(dev, CONFIG_Reg, config | CONFIG_MASK_TX_DS | CONFIG_PRIM_RX);
        delay_us(1500);                  // 1.5 ms to power up
        GPIO_setPin(&dev->CE);     // In 103 us we are monitoring (but no need to delay)
    }
    else if (dev->mode == nRF24L01_TX)
    {
        nRF24L01_setRegister8(dev, CONFIG_Reg, (config | CONFIG_MASK_RX_DR) & ~CONFIG_PRIM_RX);
        delay_us(1500);                  // 1.5 ms to power up
    }
    else
    {
        die("nRF24L01: Bad mode!");
    }

    return true;
}

void nRF24L01_send(const struct nRF24L01 *dev, const void *payload, size_t len)
{
    assert(dev->mode == nRF24L01_TX);
    assert(1 <= len && len <= 32);

    // TODO: check if TX FIFO full, and fail/block in this case
    irq_lock_t lock;
    LOCK_IRQ(lock);

    nRF24L01_writeTxPayload(dev, payload, len);

    // Toggle CE pin to send TX FIFO
    GPIO_setPin(&dev->CE);
    delay_us(10);
    GPIO_resetPin(&dev->CE);

    UNLOCK_IRQ(lock);
}

void nRF24L01_rxDispatchFIFO(const struct nRF24L01 *dev)
{
    assert(dev->mode == nRF24L01_RX, "RX mode only function");

    while (true)
    {
        // Until RX FIFO is empty
        uint8_t fifo_status;
        const uint8_t status = nRF24L01_getRegister8(dev, FIFO_STATUS_Reg, &fifo_status);
        if (fifo_status & FIFO_STATUS_RX_EMPTY)
            break;

        // get the pipe number and send it on to rx_cb
        const uint8_t pipeNo = ((status & STATUS_RX_P_NO) >> STATUS_RX_P_NO_Pos);
        assert(pipeNo <= 5, "recv from bad pipe?");
        uint8_t recv[dev->pipe[pipeNo].payloadWidth]; // TODO: read payload width from RX_PW register instead?
        nRF24L01_getRxPayload(dev, &recv[0], sizeof(recv));
        if (dev->rx_cb != NULL)
            dev->rx_cb(dev, pipeNo, &recv[0], sizeof(recv));
    }
}

static void nRF24L01_RX_DR_handler(const struct nRF24L01 *dev)
{
    assert(dev->mode == nRF24L01_RX, "Got unexpected RX_DR interrupt (not in RX mode)");

    nRF24L01_rxDispatchFIFO(dev);

    // Clear RX_DR flag
    nRF24L01_setRegister8(dev, STATUS_Reg, STATUS_RX_DR);
}

static void nRF24L01_TX_DS_handler(const struct nRF24L01 *dev)
{
    assert(dev->mode == nRF24L01_TX, "Got unexpected TX_DS interrupt (not in TX mode)");

    // TODO: atomically set flag in device struct to notify that we are now free to send more?

    if (dev->tx_ds_cb != NULL)
        dev->tx_ds_cb(dev);

    // Clear TX_DS flag
    nRF24L01_setRegister8(dev, STATUS_Reg, STATUS_TX_DS);
}

static void nRF24L01_MAX_RT_handler(const struct nRF24L01 *dev)
{
    assert(dev->mode == nRF24L01_TX, "Got unexpected MAX_RT interrupt (not in TX mode)");
    assert(dev->retransmit.count == 0, "Got unexpected MAX_RT interrupt (retransmit disabled)");

    // Call callback if any
    if (dev->max_rt_cb != NULL)
        dev->max_rt_cb(dev);

    // Clear MAX_RT flag
    nRF24L01_setRegister8(dev, STATUS_Reg, STATUS_MAX_RT);
}

void nRF24L01_interrupt(const struct nRF24L01 *dev)
{
    const uint8_t status = nRF24L01_NOP(dev);

    // Interrupt handler dispatch
    if (status & STATUS_RX_DR)  nRF24L01_RX_DR_handler(dev);
    if (status & STATUS_TX_DS)  nRF24L01_TX_DS_handler(dev);
    if (status & STATUS_MAX_RT) nRF24L01_MAX_RT_handler(dev);
}

void nRF24L01_printAddresses(const struct nRF24L01 *dev)
{
    const size_t len =
        (dev->addressWidth == nRF24L01_AddressWidth_3bytes) ? 3 :
        (dev->addressWidth == nRF24L01_AddressWidth_4bytes) ? 4 : 5;
    uint8_t tmp[5] = {};
    print("TX_ADDR: ");
    nRF24L01_getRegister(dev, TX_ADDR_Reg, &tmp[0], len);
    for (unsigned i = 0; i < len; ++i)
        println_u32_hex(tmp[i]);

    print("RX ADDR P0: ");
    nRF24L01_getRegister(dev, RX_ADDR_P0_Reg, &tmp[0], len);
    for (unsigned i = 0; i < len; ++i)
        println_u32_hex(tmp[i]);

    print("RX ADDR P1: ");
    nRF24L01_getRegister(dev, RX_ADDR_P1_Reg, &tmp[0], len);
    for (unsigned i = 0; i < len; ++i)
        println_u32_hex(tmp[i]);

    print("RX ADDR P2: ");
    nRF24L01_getRegister8(dev, RX_ADDR_P2_Reg, &tmp[0]);
    for (unsigned i = 0; i < len; ++i)
        println_u32_hex(tmp[i]);

    print("RX ADDR P3: ");
    nRF24L01_getRegister8(dev, RX_ADDR_P3_Reg, &tmp[0]);
    for (unsigned i = 0; i < len; ++i)
        println_u32_hex(tmp[i]);

    print("RX ADDR P4: ");
    nRF24L01_getRegister8(dev, RX_ADDR_P4_Reg, &tmp[0]);
    for (unsigned i = 0; i < len; ++i)
        println_u32_hex(tmp[i]);

    print("RX ADDR P5: ");
    nRF24L01_getRegister8(dev, RX_ADDR_P5_Reg, &tmp[0]);
    for (unsigned i = 0; i < len; ++i)
        println_u32_hex(tmp[i]);
}
