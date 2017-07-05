#include "spi.h"

#include "clock.h"
#include "utils.h"

bool spi_getBaudRateDivisorFromMaxFreq(volatile struct SPI_Regs const * const spi, uint32_t maxFreq,
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
        uint32_t freq = (baseFreq >> i);
        if (freq <= maxFreq)
        {
            if (flag != NULL)       *flag = flags[i-1];
            if (actualFreq != NULL) *actualFreq = freq;
            return true;
        }
    }
    return false;
}

struct SPI_Pins
{
    struct GPIO_PortPin NSS, SCK, MISO, MOSI;
};

// TODO: Check the various pin modes in Table 25. in the reference manual, and parametrize on that.
// TODO: Assumes master mode (SCK is an output, so is NSS), but might work in slave mode as well, as
//       SCK might as well be a bi-directional pin from the view of GPIO w.r.t. different
//       configurations of the peripheral, but this will of course not let us do pull-ups or
//       pull-downs on a SCK input, for instance.
// TODO: NSS pin is always configured as an AF output, this might not be what we always want
static void spi_setupGpioHelper(enum SPI_OutputMode outputMode,
                                enum SPI_InputMode inputMode,
                                struct SPI_Pins const * const pins)
{
    const uint8_t outputCNF = (outputMode == SPI_PushPull) ? GPIO_Output_CNF_AFPushPull : GPIO_Output_CNF_AFOpenDrain;
    const uint8_t inputCNF  = (inputMode == SPI_Floating) ? GPIO_Input_CNF_Floating : GPIO_Input_CNF_PullupPulldown;

    GPIO_setMODE_setCNF(&pins->NSS,  GPIO_MODE_Output_50MHz, outputCNF); // NSS
    GPIO_setMODE_setCNF(&pins->SCK,  GPIO_MODE_Output_50MHz, outputCNF); // SCK
    GPIO_setMODE_setCNF(&pins->MOSI, GPIO_MODE_Output_50MHz, outputCNF); // MOSI

    // TODO: consider having a gpio init function which also boxes pullup/pulldown (this would've then been a one-liner)
    GPIO_setMODE_setCNF(&pins->MISO, GPIO_MODE_Input, inputCNF);         // MISO
    if (inputMode == SPI_PullUp)
        GPIO_setPin(&pins->MISO);
    else if (inputMode == SPI_PullDown)
        GPIO_resetPin(&pins->MISO);
}

// TODO: mold this into a more fully encompassing SPI init function (handle freq, clock phase &c.,
// and do not set up port bits for NSS when not applicable)
void SPI1_SetupGpio(enum AF_Mapping mapping, enum SPI_OutputMode outputMode, enum SPI_InputMode inputMode)
{
    irq_lock_t lock;
    LOCK_IRQ(lock);

    if (mapping == AFIO_ALTERNATE)
        AFIO.MAPR |= AFIO_MAPR_SPI1_REMAP;
    else
        AFIO.MAPR &= ~(AFIO_MAPR_SPI1_REMAP);

    static const struct SPI_Pins SPI1_Default_Mapping = {
        .NSS  = {&GPIOA, 4},
        .SCK  = {&GPIOA, 5},
        .MISO = {&GPIOA, 6},
        .MOSI = {&GPIOA, 7}
    };
    static const struct SPI_Pins SPI1_Alternate_Mapping = {
        .NSS  = {&GPIOA, 15},
        .SCK  = {&GPIOB, 3},
        .MISO = {&GPIOB, 4},
        .MOSI = {&GPIOB, 5}
    };

    const struct SPI_Pins *pinMap = (mapping == AFIO_ALTERNATE) ? &SPI1_Alternate_Mapping : &SPI1_Default_Mapping;
    spi_setupGpioHelper(outputMode, inputMode, pinMap);

    UNLOCK_IRQ(lock);
}

void SPI2_SetupGpio(enum SPI_OutputMode outputMode, enum SPI_InputMode inputMode)
{
    irq_lock_t lock;
    LOCK_IRQ(lock);

    static const struct SPI_Pins SPI2_Mapping = {
        .NSS  = {&GPIOB, 12},
        .SCK  = {&GPIOB, 13},
        .MISO = {&GPIOB, 14},
        .MOSI = {&GPIOB, 15},
    };

    spi_setupGpioHelper(outputMode, inputMode, &SPI2_Mapping);

    UNLOCK_IRQ(lock);
}
