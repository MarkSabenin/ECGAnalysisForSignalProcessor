#ifndef PROJECT_CONFIG_H
#define PROJECT_CONFIG_H

/* Конфигурация системы */
#define SYSTEM_FREQUENCY_HZ     100000000UL  /* 100 MHz системная частота */
#define SPI_FREQUENCY_HZ        2000000UL    /* 2 MHz SPI частота */
#define UART_BAUDRATE           115200UL     /* Скорость UART */

/* Конфигурация ADAS1000 */
#define ECG_BUFFER_SIZE         4096         /* Размер буфера ЭКГ (4KB) */
#define MAX_ECG_FRAMES          100          /* Максимальное количество кадров */

/* Конфигурация пинов ADAS1000 */
#define PIN_CS                  (1 << 11)    /* PF11 */
#define PIN_DRDY                (1 << 12)    /* PF12 */
#define PIN_RESET               (1 << 13)    /* PF13 */

/* Режимы работы */
#define ECG_MODE_NORMAL         0
#define ECG_MODE_TEST           1
#define ECG_MODE_CALIBRATION    2

/* Структура состояния системы */
typedef struct {
    uint8_t ecg_mode;
    uint8_t is_running;
    uint32_t error_count;
    uint32_t frame_count;
} system_state_t;

#endif /* PROJECT_CONFIG_H */
