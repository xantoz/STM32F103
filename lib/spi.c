#include "spi.h"

#include "clock.h"
#include "utils.h"

const struct SPI_Pins SPI1_Default_Pins = {
    .NSS  = {&GPIOA, 4},
    .SCK  = {&GPIOA, 5},
    .MISO = {&GPIOA, 6},
    .MOSI = {&GPIOA, 7}
};

const struct SPI_Pins SPI1_Alternate_Pins = {
    .NSS  = {&GPIOA, 15},
    .SCK  = {&GPIOB, 3},
    .MISO = {&GPIOB, 4},
    .MOSI = {&GPIOB, 5}
};

const struct SPI_Pins SPI2_Pins = {
    .NSS  = {&GPIOB, 12},
    .SCK  = {&GPIOB, 13},
    .MISO = {&GPIOB, 14},
    .MOSI = {&GPIOB, 15},
};

bool SPI_getBaudRateDivisorFromMaxFreq(volatile struct SPI_Regs const * const spi, uint32_t maxFreq,
                                       uint16_t *flag, uint32_t *actualFreq)
{
    static const uint16_t flags[8] = {
        SPI_CR1_BR_PCLK_Div2,
        SPI_CR1_BR_PCLK_Div4,
        SPI_CR1_BR_PCLK_Div8,
        SPI_CR1_BR_PCLK_Div16,
        SPI_CR1_BR_PCLK_Div32,
        SPI_CR1_BR_PCLK_Div64,
        SPI_CR1_BR_PCLK_Div128,
        SPI_CR1_BR_PCLK_Div256,
    };

    const uint32_t baseFreq = (spi == &SPI1) ? g_clock.pclk2Freq : g_clock.pclk1Freq;
    for (uint8_t i = 1; i <= 8; ++i)
    {
        const uint32_t freq = (baseFreq >> i);
        if (freq <= maxFreq)
        {
            if (flag != NULL)       *flag = flags[i-1];
            if (actualFreq != NULL) *actualFreq = freq;
            return true;
        }
    }
    return false;
}

// TODO: Check the various pin modes in Table 25. in the reference manual, and parametrize on that.
// TODO: Assumes master mode (SCK is an output, so is NSS), but might work in slave mode as well, as
//       SCK might as well be a bi-directional pin from the view of GPIO w.r.t. different
//       configurations of the peripheral, but this will of course not let us do pull-ups or
//       pull-downs on a SCK input, for instance.
static void SPI_setupGPIOHelper(enum SPI_OutputMode outputMode,
                                enum SPI_InputMode inputMode,
                                struct SPI_Pins const * const pins,
                                bool hardwareNSS)
{
    const uint8_t outputCNF = (outputMode == SPI_PushPull) ? GPIO_Output_CNF_AFPushPull : GPIO_Output_CNF_AFOpenDrain;
    const uint8_t inputCNF  = (inputMode == SPI_Floating) ? GPIO_Input_CNF_Floating : GPIO_Input_CNF_PullupPulldown;

    if (hardwareNSS)
        GPIO_setMODE_setCNF(&pins->NSS,  GPIO_MODE_Output_50MHz, outputCNF);
    else
        GPIO_setMODE_setCNF(&pins->NSS,
                            GPIO_MODE_Output_50MHz,
                            (outputMode == SPI_PushPull) ? GPIO_Output_CNF_GPPushPull : GPIO_Output_CNF_GPOpenDrain);

    GPIO_setMODE_setCNF(&pins->SCK,  GPIO_MODE_Output_50MHz, outputCNF); // SCK
    GPIO_setMODE_setCNF(&pins->MOSI, GPIO_MODE_Output_50MHz, outputCNF); // MOSI

    // TODO: consider having a gpio init function which also boxes pullup/pulldown (this would've then been a one-liner)
    GPIO_setMODE_setCNF(&pins->MISO, GPIO_MODE_Input, inputCNF);         // MISO
    if (inputMode == SPI_PullUp)
        GPIO_setPin(&pins->MISO);
    else if (inputMode == SPI_PullDown)
        GPIO_resetPin(&pins->MISO);
}

static void SPI1_setupGPIO(enum AF_Mapping mapping,
                           enum SPI_OutputMode outputMode,
                           enum SPI_InputMode inputMode,
                           bool hardwareNSS)
{
    irq_lock_t lock;
    LOCK_IRQ(lock);

    if (mapping == AFIO_ALTERNATE)
        AFIO.MAPR |= AFIO_MAPR_SPI1_REMAP;
    else
        AFIO.MAPR &= ~(AFIO_MAPR_SPI1_REMAP);

    const struct SPI_Pins *pinMap = (mapping == AFIO_ALTERNATE) ? &SPI1_Alternate_Pins : &SPI1_Default_Pins;
    SPI_setupGPIOHelper(outputMode, inputMode, pinMap, hardwareNSS);

    UNLOCK_IRQ(lock);
}

static void SPI2_setupGPIO(enum SPI_OutputMode outputMode,
                           enum SPI_InputMode inputMode,
                           bool hardwareNSS)
{
    irq_lock_t lock;
    LOCK_IRQ(lock);

    SPI_setupGPIOHelper(outputMode, inputMode, &SPI2_Pins, hardwareNSS);

    UNLOCK_IRQ(lock);
}

void SPI_initAsMaster(volatile struct SPI_Regs * spi, const struct SPI_Options * const options)
{
    irq_lock_t lock;
    LOCK_IRQ(lock);

    spi->CR1 &= ~(SPI_CR1_SPE); // Ensure SPI not enabled

    // Setup GPIO pins for SPI
    if (spi == &SPI1)
        SPI1_setupGPIO(options->mapping,
                       options->outputMode,
                       options->inputMode,
                       options->nss != SPI_SoftwareNSS);
    else if (spi == &SPI2)
        SPI2_setupGPIO(options->outputMode,
                       options->inputMode,
                       options->nss != SPI_SoftwareNSS);
    else
        die("No such SPI");

    spi->CR1 = SPI_CR1_MSTR; // Set as master
    spi->CR2 = 0;
    if (options->nss == SPI_SoftwareNSS)
    {
        // SSM = 1, SSI = 1, SSOE = 0
        spi->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;
        spi->CR2 &= ~(SPI_CR2_SSOE);
    }
    else if (options->nss == SPI_HardwareNSSOutput)
    {
        // SSM = 0, SSOE = 1
        spi->CR1 &= ~(SPI_CR1_SSM);
        spi->CR2 |= SPI_CR2_SSOE;
    }
    else if (options->nss == SPI_HardwareNSSInput)
    {
        // SSM = 0, SSOE = 0
        spi->CR1 &= ~(SPI_CR1_SSM);
        spi->CR2 &= ~(SPI_CR2_SSOE);
    }
    else
    {
        die("bad option for NSS");
    }

    // Clock phase settings
    if (options->cpol)
        spi->CR1 |= SPI_CR1_CPOL;
    else
        spi->CR1 &= ~(SPI_CR1_CPOL);

    if (options->cpha)
        spi->CR1 |= SPI_CR1_CPHA;
    else
        spi->CR1 &= ~(SPI_CR1_CPHA);

    // Set bitrate to maximum possible speed less than or equal to maxFreq
    uint16_t flag;
    uint32_t actualFreq;
    if (!SPI_getBaudRateDivisorFromMaxFreq(spi, options->maxFreq, &flag, &actualFreq))
        die("Requested SPI bitrate not available");
    spi->CR1 &= ~SPI_CR1_BR;
    spi->CR1 |= flag;

    print("SPI: Set bitrate to ");
    print_u32_dec(actualFreq);
    print(" flag: ");
    print_u32_hex(flag);
    print(" (Requested ");
    print_u32_dec(options->maxFreq);
    print(")\n");

    // Enable
    spi->CR1 |= SPI_CR1_SPE;

    UNLOCK_IRQ(lock);
}
