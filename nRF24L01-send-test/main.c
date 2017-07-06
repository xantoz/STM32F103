#include "lib/types.h"
#include "lib/utils.h"
#include "lib/rcc.h"
#include "lib/clock.h"
#include "lib/spi.h"
#include "lib/debug.h"

#include "lib/nRF24L01/nRF24L01.h"

#define MAX_BAUDRATE 8000000
#define DELAY 200000

// TODO: A0 receives IRQ when recv

static void spi_setup()
{
    __disable_irq();

    SPI1.CR1 &= SPI_CR1_SPE; // Ensure SPI not enabled

    // Disable:
    // * Bidirectional
    // * CRC
    // * Software slave management
    // * 16 bit data frame format (use 8bit data frame format)
    // * LSBFISRT (MSB transmitted first)
    // * RXONLY
    // + Tx buffer empty interrupt
    // + Rx buffer not empty interrupt
    // + Error interrupt
    // + Tx DMA
    // + Rx DMA
    // + SS output
    SPI1.CR1 &= ~(SPI_CR1_BIDIMODE | SPI_CR1_CRCEN | SPI_CR1_SSM |
                   SPI_CR1_DFF | SPI_CR1_LSBFIRST | SPI_CR1_RXONLY);
    SPI1.CR2 &= ~(SPI_CR2_TXEIE | SPI_CR2_RXNEIE | SPI_CR2_ERRIE |
                   SPI_CR2_TXDMAEN | SPI_CR2_RXDMAEN | SPI_CR2_SSOE);

    // Enable:
    // * Master mode
    SPI1.CR1 |= SPI_CR1_MSTR;

    // Clock phase settings. CPOL = 0, CPHA = 0
    SPI1.CR1 &= ~(SPI_CR1_CPOL | SPI_CR1_CPHA);

    // Setup GPIO pins for SPI1
    SPI1_SetupGpio(AFIO_DEFAULT, SPI_PushPull, SPI_PullDown, false);

    // Set baudrate to maximum possible speed less than or equal to MAX_BAUDRATE
    print("spi_getBaudRateDivisorFromMaxFreq\n");
    uint16_t flag;
    uint32_t actualFreq;
    if (!spi_getBaudRateDivisorFromMaxFreq(&SPI1, MAX_BAUDRATE, &flag, &actualFreq))
        die("Requested SPI baudrate not available");
    SPI1.CR1 &= ~SPI_CR1_BR;
    SPI1.CR1 |= flag;

    print("Set baudrate to ");
    print_u32_dec(actualFreq);
    print(" flag: ");
    print_u32_hex(flag);
    print(" (Requested " TOSTRING(MAX_BAUDRATE) ")\n");

    // Enable SPI
    SPI1.CR1 |= SPI_CR1_SPE;

    __enable_irq();
}

static void spi_send(uint16_t data)
{
    while (!(SPI1.SR & SPI_SR_TXE)); // Wait until transmit buffer empty

    SPI1.DR = data;
}

static uint8_t spi_recv()
{
    while (!(SPI1.SR & SPI_SR_RXNE)); // Block until we have data

    return SPI1.DR;
}

static uint8_t spi_sendrecv(uint8_t data)
{
    spi_send(data);
    return spi_recv(data);
}

// Settings for the nRF24L01
static const struct nRF24L01_Options rfDev_opts = {
    .CSN = {&GPIOA, 4}, // OK since we set up SPI1 so that this is usable as GPIO
    .CE  = {&GPIOA, 1},

    .airDataRate    = nRF24L01_2Mbps,
    .power          = nRF24L01_TXPower_Minus0dBm,
    .useACK         = nRF24L01_NoACK,
    .useCRC         = nRF24L01_NoCRC,
    .retransmission = nRF24L01_NoRetransmission,
    .mode           = nRF24L01_TX,

    .payloadWidth   = 2,
    .channel        = 33,

    .spi_sendrecv   = &spi_sendrecv,
    .rx_cb          = NULL
};

struct nRF24L01 rfDev;

void main()
{
    clock_setSysClockHSE_24MHz();

    // Enable clock to all GPIO:s
    RCC.APB2ENR |= RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC;

    RCC.APB2ENR |= RCC_APB2Periph_SPI1;    // Enable clock to SPI1

    GPIO_setMODE_setCNF(&GPIOC, 13, GPIO_MODE_Output_10MHz, GPIO_Output_CNF_GPPushPull);

    spi_setup();
    nRF24L01_init(&rfDev_opts, &rfDev);

    uint16_t msg = 110;
    while (true)
    {
        GPIO_resetPin(&GPIOC, 13);
        delay_us(DELAY);

        nRF24L01_send(&rfDev, &msg);

        GPIO_setPin(&GPIOC, 13);
        delay_us(DELAY);
        ++msg;
    }
}
