/**
 * @file      i2c_driver.h
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2024  Shenzhen Xinyuan Electronic Technology Co., Ltd
 * @date      2024-01-07
 *
 */
#pragma once
#include "esp_err.h"
#include "driver/i2c_master.h"

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t i2c_driver_init(void);
void i2c_drv_scan();

extern i2c_master_bus_handle_t bus_handle;
#ifdef __cplusplus
}
#endif