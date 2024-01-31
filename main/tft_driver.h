/**
 * @file      tft_driver.h
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2024  Shenzhen Xinyuan Electronic Technology Co., Ltd
 * @date      2024-01-08
 *
 */
#pragma once
#include "esp_err.h"

#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_vendor.h"
#include "lvgl.h"
#ifdef __cplusplus
extern "C" {
#endif

void display_init();
void display_push_colors(uint16_t x, uint16_t y, uint16_t width, uint16_t hight, uint16_t *data);
#ifdef __cplusplus
}
#endif












