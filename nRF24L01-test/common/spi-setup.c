#include "spi-setup.h"
#include "spi.h"

// #define MAX_BAUDRATE 8000000
#define MAX_BAUDRATE 2000000

void spi_setup()
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
                  SPI_CR2_TXDMAEN | SPI_CR2_RXDMAEN);

    // Enable:
    // * Master mode
    // + SS output
    SPI1.CR1 |= SPI_CR1_MSTR;
    SPI1.CR2 |= SPI_CR2_SSOE;

    // Clock phase settings. CPOL = 0, CPHA = 0
    SPI1.CR1 &= ~(SPI_CR1_CPOL | SPI_CR1_CPHA);

    // Setup GPIO pins for SPI1
    SPI1_SetupGpio(AFIO_DEFAULT, SPI_PushPull, SPI_PullDown, true);

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

static void spi_send(uint8_t data)
{
    while (!(SPI1.SR & SPI_SR_TXE)); // Wait until transmit buffer empty

    SPI1.DR = (uint16_t)data;
}

static uint8_t spi_recv()
{
    while (!(SPI1.SR & SPI_SR_RXNE)); // Block until we have data

    return (uint8_t)SPI1.DR;
}

uint8_t spi_sendrecv(uint8_t data)
{
    spi_send(data);
    return spi_recv(data);
}
