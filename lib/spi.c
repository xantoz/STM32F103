#include "spi.h"

#include "clock.h"

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

void SPI1_MapGpio(enum AF_Mapping mapping, enum SPI_OutputMode outputMode, enum SPI_InputMode inputMode)
{
    uint32_t outputCNF = (outputMode == SPI_PushPull) ? GPIO_Output_CNF_AFPushPull : GPIO_Output_CNF_AFOpenDrain;
    uint32_t inputCNF  = (inputMode == SPI_Floating) ? GPIO_Input_CNF_Floating : GPIO_Input_CNF_PullupPulldown;

    // TODO: Rewrite using GPIO_PortPin structs instead, for clearer, less repeated code?
    if (mapping == DEFAULT)
    {
        GPIO_setMODE_setCNF(&GPIOA, 4, GPIO_MODE_Output_50MHz, outputCNF); // NSS
        GPIO_setMODE_setCNF(&GPIOA, 5, GPIO_MODE_Output_50MHz, outputCNF); // SCK
        GPIO_setMODE_setCNF(&GPIOA, 7, GPIO_MODE_Output_50MHz, outputCNF); // MOSI

        GPIO_setMODE_setCNF(&GPIOA, 6, GPIO_MODE_Input, inputCNF);         // MISO
        if (inputMode == SPI_PullUp)
            GPIO_setPin(&GPIOA, 6);
        else if (inputMode == SPI_PullDown)
            GPIO_resetPin(&GPIOA, 6);

        AFIO.MAPR &= ~(AFIO_MAPR_SPI1_REMAP);
    }
    else if (mapping == ALTERNATE)
    {
        GPIO_setMODE_setCNF(&GPIOA, 15, GPIO_MODE_Output_50MHz, outputCNF); // NSS
        GPIO_setMODE_setCNF(&GPIOB, 3,  GPIO_MODE_Output_50MHz, outputCNF); // SCK
        GPIO_setMODE_setCNF(&GPIOB, 5,  GPIO_MODE_Output_50MHz, outputCNF); // MOSI

        GPIO_setMODE_setCNF(&GPIOB, 4,  GPIO_MODE_Input, inputCNF);         // MISO
        if (inputMode == SPI_PullUp)
            GPIO_setPin(&GPIOB, 4);
        else if (inputMode == SPI_PullDown)
            GPIO_resetPin(&GPIOB, 4);

        AFIO.MAPR |= AFIO_MAPR_SPI1_REMAP;
    }
}
