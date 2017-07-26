#include "spi-setup.h"
#include "spi.h"

#define mySPI SPI1

static const struct SPI_Options spi_opts = {
    .maxFreq = 8000000,
    .mapping = AFIO_DEFAULT,
    .outputMode = SPI_PushPull,
    .inputMode  = SPI_PullDown,
    .nss = SPI_SoftwareNSS,
    .cpol = 0,
    .cpha = 0,
};

void spi_setup()
{
    SPI_initAsMaster(&mySPI, &spi_opts);
}

uint8_t spi_sendrecv(const uint8_t data)
{
    SPI_send(&mySPI, data);
    return SPI_recv(&mySPI);
}
