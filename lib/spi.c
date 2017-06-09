#include "spi.h"

#include "clock.h"
#include "utils.h"
#include "debug.h"

bool spi_getBaudRateDivisorFromMaxFreq(SPI_Struct const * const spi, uint32_t maxFreq,
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

    uint32_t baseFreq = (spi == &SPI1) ? g_clock.pclk2Freq : g_clock.pclk1Freq;
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

static void spi_setupGpioHelper(enum SPI_OutputMode outputMode,
                                enum SPI_InputMode inputMode,
                                GPIO_PortPin const * const NSS,
                                GPIO_PortPin const * const SCK,
                                GPIO_PortPin const * const MISO,
                                GPIO_PortPin const * const MOSI)
{
    uint8_t outputCNF = (outputMode == SPI_PushPull) ? GPIO_Output_CNF_AFPushPull : GPIO_Output_CNF_AFOpenDrain;
    uint8_t inputCNF  = (inputMode == SPI_Floating) ? GPIO_Input_CNF_Floating : GPIO_Input_CNF_PullupPulldown;

    GPIO_setMODE_setCNF(NSS,  GPIO_MODE_Output_50MHz, outputCNF); // NSS
    GPIO_setMODE_setCNF(SCK,  GPIO_MODE_Output_50MHz, outputCNF); // SCK
    GPIO_setMODE_setCNF(MOSI, GPIO_MODE_Output_50MHz, outputCNF); // MOSI

    // TODO: consider having a gpio init function which also boxes pullup/pulldown (this would've then been a one-liner)
    GPIO_setMODE_setCNF(MISO, GPIO_MODE_Input, inputCNF);         // MISO
    if (inputMode == SPI_PullUp)
        GPIO_setPin(MISO);
    else if (inputMode == SPI_PullDown)
        GPIO_resetPin(MISO);
}

// TODO: mold this into a more fully encompassing SPI init function (handle freq, clock phase &c.,
// do not set up port bits for NSS when not applicable)
void SPI1_SetupGpio(enum AF_Mapping mapping, enum SPI_OutputMode outputMode, enum SPI_InputMode inputMode)
{
    if (mapping == DEFAULT)
        RESET(AFIO, MAPR, SPI1_REMAP);
    else if (mapping == ALTERNATE)
        SET(AFIO, MAPR, SPI1_REMAP);
    else
        die("SPI1_MapGpio: Invalid parameter");

    const GPIO_PortPin NSS  = (mapping == DEFAULT) ? (GPIO_PortPin){&GPIOA, 4} : (GPIO_PortPin){&GPIOA, 15};
    const GPIO_PortPin SCK  = (mapping == DEFAULT) ? (GPIO_PortPin){&GPIOA, 5} : (GPIO_PortPin){&GPIOB, 3};
    const GPIO_PortPin MISO = (mapping == DEFAULT) ? (GPIO_PortPin){&GPIOA, 6} : (GPIO_PortPin){&GPIOB, 4};
    const GPIO_PortPin MOSI = (mapping == DEFAULT) ? (GPIO_PortPin){&GPIOA, 7} : (GPIO_PortPin){&GPIOB, 5};

    spi_setupGpioHelper(outputMode, inputMode, &NSS, &SCK, &MISO, &MOSI);
}

void SPI2_SetupGpio(enum SPI_OutputMode outputMode, enum SPI_InputMode inputMode)
{
    static const GPIO_PortPin NSS  = {&GPIOA, 4};
    static const GPIO_PortPin SCK  = {&GPIOA, 5};
    static const GPIO_PortPin MISO = {&GPIOA, 6};
    static const GPIO_PortPin MOSI = {&GPIOA, 7};
    
    spi_setupGpioHelper(outputMode, inputMode, &NSS, &SCK, &MISO, &MOSI);
}
    
