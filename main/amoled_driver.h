/**
 * @file      amoled_driver.h
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2024  Shenzhen Xinyuan Electronic Technology Co., Ltd
 * @date      2024-01-07
 *
 */

#include <stdint.h>
#include "product_pins.h"

#ifdef __cplusplus
extern "C" {
#endif

void display_init();

uint16_t  amoled_width();

uint16_t  amoled_height();

void amoled_set_brightness(uint8_t level);

uint8_t amoled_get_brightness();

void amoled_set_window(uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye);

void amoled_push_buffer(uint16_t *data, uint32_t len);

void display_push_colors(uint16_t x, uint16_t y, uint16_t width, uint16_t hight, uint16_t *data);

#ifdef __cplusplus
}
#endif