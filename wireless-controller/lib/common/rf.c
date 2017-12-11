#include "rf.h"
#include "config.h"

#include <exti.h>
#include <IRQn.h>
#include <spi.h>
#include <nvic.h>
#include <debug.h>

static const struct SPI_Options spi_opts = {
    .maxFreq = nRF24L01_SPI_MAXFREQ,
    .mapping = AFIO_DEFAULT,
    .outputMode = SPI_PushPull,
    .inputMode  = SPI_PullDown,
    .nss = SPI_SoftwareNSS,
    .cpol = 0,
    .cpha = 0,
};

static uint8_t spi_sendrecv(const uint8_t data)
{
    SPI_send(&nRF24L01_SPI, data);
    return SPI_recv(&nRF24L01_SPI);
}

static struct nRF24L01 rfDev = {
    .CSN = nRF24L01_CSN_PortPin,
    .CE  = nRF24L01_CE_PortPin,
    .IRQ = nRF24L01_IRQ_PortPin,

    .airDataRate      = nRF24L01_2Mbps,
    .power            = nRF24L01_TXPower_Minus0dBm,
    .useCRC           = nRF24L01_CRC,
    .retransmit.count = 0,

    .channel = 33,

    .spi_sendrecv = &spi_sendrecv,
};

// List of 16 addresses which we can choose between. One for each DIP setting.
// The suffix (last 4 bytes) must be the same for all addresses, due to how the
// nRF24L01 handles addresses for pipes 2-5 (You can only set the first byte,
// while the last 4 bytes are taken directly from the address for P1)
#define ADDRESS_SUFFIX 0xAA, 0xBB, 0xCC, 0xDD
#define DEFAULT_ADDRESS 0
static const uint8_t address[16][5] = {
    {0xF0, ADDRESS_SUFFIX},
    {0xE1, ADDRESS_SUFFIX},
    {0xD2, ADDRESS_SUFFIX},
    {0xC3, ADDRESS_SUFFIX},
    {0xB4, ADDRESS_SUFFIX},
    {0xA5, ADDRESS_SUFFIX},
    {0x96, ADDRESS_SUFFIX},
    {0x87, ADDRESS_SUFFIX},
    {0x78, ADDRESS_SUFFIX},
    {0x69, ADDRESS_SUFFIX},
    {0x5A, ADDRESS_SUFFIX},
    {0x4B, ADDRESS_SUFFIX},
    {0x3C, ADDRESS_SUFFIX},
    {0x2D, ADDRESS_SUFFIX},
    {0x1E, ADDRESS_SUFFIX},
    {0x0F, ADDRESS_SUFFIX},
};

void rf_setTxAddress(uint8_t addr)
{
    assert(addr < 16);
    nRF24L01_setTxAddress(&rfDev, address[addr]);
}

void rf_setRxAddress(uint8_t addr, uint8_t pipe)
{
    assert(addr < 16);
    assert(pipe < 6);

    switch (pipe)
    {
        case 0:
            nRF24L01_setRxP0Address(&rfDev, address[addr]);
            break;
        case 1:
            nRF24L01_setRxP1Address(&rfDev, address[addr]);
            break;
        case 2:
            nRF24L01_setRxP2Address(&rfDev, address[addr][0]);
            break;
        case 3:
            nRF24L01_setRxP3Address(&rfDev, address[addr][0]);
            break;
        case 4:
            nRF24L01_setRxP4Address(&rfDev, address[addr][0]);
            break;
        case 5:
            nRF24L01_setRxP5Address(&rfDev, address[addr][0]);
            break;
    }
}

void rf_init(enum rf_TxRx txrx,
             void (*recv_message)(const struct nRF24L01*, uint8_t pipeNo,
                                  const void *data, size_t len),
             size_t payloadWidth,
             size_t nrRxPipes)
{
    assert(nrRxPipes <= 6);

    SPI_initAsMaster(&nRF24L01_SPI, &spi_opts);
    NVIC_setInterruptPriority(nRF24L01_IRQn, nRF24L01_IRQ_Priority);

    rfDev.rx_cb = recv_message;
    rfDev.mode = (txrx == rf_Tx) ? nRF24L01_TX : nRF24L01_RX;
    for (size_t i = 0; i < nrRxPipes; ++i)
    {
        rfDev.pipe[i].enable = true;
        rfDev.pipe[i].payloadWidth = payloadWidth;
        rfDev.pipe[i].autoAck = false;
    }

    nRF24L01_init(&rfDev);

    // Set addresses to their default values (differs from the nRF24L01 defaults)
    rf_setTxAddress(0);
    for (size_t i = 0; i < 6; ++i)
        rf_setRxAddress(i, i);
}

void rf_send(const void *data, size_t len)
{
    nRF24L01_send(&rfDev, data, len);
}

// Implement the IRQ handler
void nRF24L01_IRQHandler(void)
{
    GPIO_resetPin(&LED);
    nRF24L01_interrupt(&rfDev);
    EXTI.PR = 0x1 << nRF24L01_IRQ_PortPin.pin;
    GPIO_setPin(&LED);
}
