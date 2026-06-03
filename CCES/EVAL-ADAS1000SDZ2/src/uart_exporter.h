/*******************************************************************************
 * uart_exporter.h
 * Отправка обработанных данных через UART (использует ранее реализованные функции)
 ******************************************************************************/

#ifndef UART_EXPORTER_H
#define UART_EXPORTER_H

#include "adas1000_data.h"
#include "ecg_processing.h"

/* Режимы экспорта */
typedef enum {
    EXPORT_RAW_CSV,      /* Сырые данные в CSV */
    EXPORT_FILTERED_CSV, /* Отфильтрованные данные в CSV */
    EXPORT_BINARY,       /* Бинарный режим (компактный) */
    EXPORT_DEBUG         /* Отладочный вывод */
} export_mode_t;

/* Функции уже реализованы в bf527_uart.c, поэтому только объявления */
extern void uart_send_byte(uint8_t c);
extern void uart_send_string(const char *str);

/* Функции экспорта */
void uart_exporter_init(export_mode_t mode);
void uart_export_frame(const adas_frame_t *frame,
                       const ecg_processor_t *proc);
void uart_export_stats(const ecg_processor_t *proc);

#endif /* UART_EXPORTER_H */
