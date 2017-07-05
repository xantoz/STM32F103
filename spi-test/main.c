#include "lib/types.h"
#include "lib/utils.h"
#include "lib/rcc.h"
#include "lib/clock.h"
#include "lib/spi.h"

#include "lib/debug.h"

// Allow to easily change which SPI peripheral is used in this code
#define mySPI (SPI1)
#define MAX_BAUDRATE 8000000

static void spi_setup()
{
    __disable_irq();

    mySPI.CR1 &= SPI_CR1_SPE; // Ensure SPI not enabled

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
    mySPI.CR1 &= ~(SPI_CR1_BIDIMODE | SPI_CR1_CRCEN | SPI_CR1_SSM |
                   SPI_CR1_DFF | SPI_CR1_LSBFIRST | SPI_CR1_RXONLY);
    mySPI.CR2 &= ~(SPI_CR2_TXEIE | SPI_CR2_RXNEIE | SPI_CR2_ERRIE |
                   SPI_CR2_TXDMAEN | SPI_CR2_RXDMAEN);

    // Enable:
    // * Master mode
    // + SS output
    mySPI.CR1 |= SPI_CR1_MSTR;
    mySPI.CR2 |= SPI_CR2_SSOE;

    // Clock phase settings. CPOL = 0, CPHA = 0
    mySPI.CR1 &= ~(SPI_CR1_CPOL | SPI_CR1_CPHA);

    // Setup GPIO pins for SPI
#if mySPI == SPI1
    print("SPI1_SetupGpio\n");
    SPI1_SetupGpio(AFIO_DEFAULT, SPI_PushPull, SPI_PullDown, true);
#elif mySPI == SPI2
    print("SPI2_SetupGpio\n");
    SPI2_SetupGpio(SPI_PushPull, SPI_PullDown, true);
#else
#error "No such SPI"
#endif

    // Set baudrate to maximum possible speed less than or equal to MAX_BAUDRATE
    print("spi_getBaudRateDivisorFromMaxFreq\n");
    uint16_t flag;
    uint32_t actualFreq;
    if (!spi_getBaudRateDivisorFromMaxFreq(&mySPI, MAX_BAUDRATE, &flag, &actualFreq))
        die("Requested SPI baudrate not available");
    mySPI.CR1 &= ~SPI_CR1_BR;
    mySPI.CR1 |= flag;

    print("Set baudrate to ");
    print_u32_dec(actualFreq);
    print(" flag: ");
    print_u32_hex(flag);
    print(" (Requested " TOSTRING(MAX_BAUDRATE) ")\n");

    // Enable SPI
    mySPI.CR1 |= SPI_CR1_SPE;

    __enable_irq();
}

static void spi_send(uint16_t data)
{
    while (!(mySPI.SR & SPI_SR_TXE)); // Wait until transmit buffer empty

    mySPI.DR = data;
}

static uint16_t spi_recv()
{
    while (!(mySPI.SR & SPI_SR_RXNE)); // Block until we have data

    return mySPI.DR;
}

static uint16_t spi_send_recv(uint16_t data)
{
    spi_send(data);
    return spi_recv(data);
}

void main(void)
{
    clock_setSysClockHSE_24MHz();

    // Enable clock to all GPIO:s
    RCC.APB2ENR |= RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC;

    RCC.APB2ENR |= RCC_APB2Periph_SPI1;    // Enable clock to SPI1
    RCC.APB1ENR |= RCC_APB1Periph_SPI2;    // Enable clock to SPI2

    GPIO_setMODE_setCNF(&GPIOC, 11, GPIO_MODE_Output_10MHz, GPIO_Output_CNF_GPPushPull);
    GPIO_setMODE_setCNF(&GPIOC, 13, GPIO_MODE_Output_10MHz, GPIO_Output_CNF_GPPushPull);

    GPIO_setPin(&GPIOC, 11);
    spi_setup();
    GPIO_resetPin(&GPIOC, 11);

    while (true)
    {
        GPIO_resetPin(&GPIOC, 13);
        delay_us(100000);
        spi_send(0b01101110);
        GPIO_setPin(&GPIOC, 13);
        delay_us(100000);
    }
}
