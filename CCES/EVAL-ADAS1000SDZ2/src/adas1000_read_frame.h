/*******************************************************************************
 * adas1000_read_frame.h
 * Заголовочный файл для функции чтения кадров из ADAS1000
 ******************************************************************************/

#ifndef ADAS1000_READ_FRAME_H
#define ADAS1000_READ_FRAME_H

#include "adas1000_data.h"

/* Адреса регистров ADAS1000 */
#define ADAS1000_FRAMES     0x40   /* Заголовок кадра */
#define ADAS1000_LADATA     0x11   /* Данные LA канала */
#define ADAS1000_LLDATA     0x12   /* Данные LL канала */
#define ADAS1000_RADATA     0x13   /* Данные RA канала */
#define ADAS1000_V1DATA     0x14   /* Данные V1 канала */
#define ADAS1000_V2DATA     0x15   /* Данные V2 канала */

/* Битовые маски заголовка кадра */
#define ADAS_FRAMES_READY_BIT       (1ul << 30)  /* 0 = данные готовы */
//#define ADAS_FRAMES_OVERFLOW_MASK   (3ul << 28)  /* Пропущенные кадры */
#define ADAS_FRAMES_FAULT_BIT       (1ul << 27)  /* Ошибка устройства */
#define ADAS_FRAMES_PACE_BITS       (7ul << 24)  /* Детекция стимуляции */
#define ADAS_FRAMES_LOFF_BIT        (1ul << 22)  /* Отрыв электрода */
#define ADAS_FRAMES_DC_LOFF_BIT     (1ul << 21)  /* DC отрыв */

/* Прототип функции */
#ifdef __cplusplus
extern "C" {
#endif

/* Чтение полного кадра данных из ADAS1000 */
void adas1000_read_frame(adas_frame_t *frame);

#ifdef __cplusplus
}
#endif

#endif /* ADAS1000_READ_FRAME_H */
