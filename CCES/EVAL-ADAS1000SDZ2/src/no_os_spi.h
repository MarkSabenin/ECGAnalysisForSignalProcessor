#ifndef NO_OS_SPI_H
#define NO_OS_SPI_H

#include <stdint.h>
#include <stdbool.h>
#include <drivers/spi/adi_spi.h>

/* Типы SPI */
typedef enum {
    NO_OS_SPI_MODE_0, /* CPOL=0, CPHA=0 */
    NO_OS_SPI_MODE_1, /* CPOL=0, CPHA=1 */
    NO_OS_SPI_MODE_2, /* CPOL=1, CPHA=0 */
    NO_OS_SPI_MODE_3  /* CPOL=1, CPHA=1 */
} no_os_spi_mode;

/* Дескриптор SPI */
struct no_os_spi_desc {
    uint32_t device_id;
    uint32_t max_speed_hz;
    uint8_t mode;
    uint8_t chip_select;
    void *extra; /* Для ADI_SPI_HANDLE */
};

/* Параметры инициализации SPI */
struct no_os_spi_init_param {
    uint32_t max_speed_hz;
    uint8_t mode;
    uint8_t chip_select;
    void *extra; /* Для ADI_SPI_CONFIG */
};

/* Функции драйвера */
int32_t no_os_spi_init(struct no_os_spi_desc **desc,
                       const struct no_os_spi_init_param *param);

int32_t no_os_spi_write_and_read(struct no_os_spi_desc *desc,
                                 uint8_t *data,
                                 uint16_t bytes_number);

int32_t no_os_spi_remove(struct no_os_spi_desc *desc);

#endif /* NO_OS_SPI_H */
