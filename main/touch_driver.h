/**
 * @file      touch_driver.h
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2024  Shenzhen Xinyuan Electronic Technology Co., Ltd
 * @date      2024-01-07
 *
 */

#pragma once
#include <stdbool.h>
#include <sdkconfig.h>
#include "product_pins.h"

#if defined(BOARD_HAS_TOUCH)

bool touch_init();
uint8_t touch_get_data(int16_t *x, int16_t *y, uint8_t point_num);

#else
bool touch_init();
#endif








