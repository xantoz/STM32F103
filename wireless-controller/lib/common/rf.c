#include "rf.h"
#include "config.h"

#include <exti.h>
#include <IRQn.h>
#include <spi.h>
#include <nvic.h>
#include <debug.h>

#include <protocol/common.h>

struct nRF24L01 g_rfDev;

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

static void __recv_message_unimpl()
{
    die("recv message not implemented!");
}

// This function needs to be defined in another module somewhere to receive payloads from nRF24L01
void recv_message(const struct nRF24L01 *dev,
                  uint8_t pipeNo,
                  const void *data, size_t len) DEFAULTS_TO(__recv_message_unimpl);

//TODO: have just one definition of this in RAM instead, and simply change the relevant fields in rf_init. rf_init should take a function pointer for recv_message as well. This makes for a cleaner API at the expense of RAM usage
static const struct nRF24L01_Options rfDev_opts_tx =
    nRF24L01_Options(
        .mode             = nRF24L01_TX,
        .spi_sendrecv     = &spi_sendrecv);

static const struct nRF24L01_Options rfDev_opts_rx =
    nRF24L01_Options(
        .mode             = nRF24L01_RX,
        .spi_sendrecv     = &spi_sendrecv,
        .rx_cb            = &recv_message);

void rf_init(enum rf_TxRx txrx)
{
    SPI_initAsMaster(&nRF24L01_SPI, &spi_opts);
    NVIC_setInterruptPriority(nRF24L01_IRQn, 4);
    nRF24L01_init((txrx == rf_Tx) ? &rfDev_opts_tx : &rfDev_opts_rx, &g_rfDev);
}

void nRF24L01_IRQHandler(void)
{
    GPIO_resetPin(&LED);
    nRF24L01_interrupt(&g_rfDev);
    EXTI.PR = 0x1 << nRF24L01_IRQ_PortPin.pin;
    GPIO_setPin(&LED);
}
