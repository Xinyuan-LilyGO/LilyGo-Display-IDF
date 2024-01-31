/**
 * @file      touch_driver.cpp
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2024  Shenzhen Xinyuan Electronic Technology Co., Ltd
 * @date      2024-01-07
 *
 */
#include <sdkconfig.h>
#include "esp_err.h"
#include "esp_log.h"
#include "i2c_driver.h"
#include "product_pins.h"
#include "freertos/FreeRTOS.h"

#if BOARD_HAS_TOUCH

static const char *TAG = "touch";

#if defined(CONFIG_LILYGO_T_AMOLED_LITE_147)
#include "TouchDrvCHSC5816.hpp"
TouchDrvCHSC5816 touch;
#define TOUCH_ADDRESS   CHSC5816_SLAVE_ADDRESS

#elif defined(CONFIG_LILYGO_T_DISPLAY_S3_AMOLED_TOUCH) || defined(CONFIG_LILYGO_T_DISPLAY_S3) || defined(CONFIG_LILYGO_T_QT_C6)
#include "REG/CSTxxxConstants.h"
#include "touch/TouchClassCST816.h"
TouchClassCST816 touch;
#define TOUCH_ADDRESS   CST816_SLAVE_ADDRESS

#elif defined(CONFIG_LILYGO_T4_S3_241) || defined(CONFIG_LILYGO_T_DISPLAY_S3_PRO)
#include "REG/CSTxxxConstants.h"
#include "touch/TouchClassCST226.h"
TouchClassCST226 touch;
#define TOUCH_ADDRESS   CST226SE_SLAVE_ADDRESS

#elif defined(CONFIG_LILYGO_T_DISPLAY_LONG)

#include "driver/i2c.h"

#define AXS_GET_POINT_NUM(buf) buf[1]
#define AXS_GET_GESTURE_TYPE(buf)  buf[0]
#define AXS_GET_POINT_X(buf,point_index) (((uint16_t)(buf[6*point_index+2] & 0x0F) <<8) + (uint16_t)buf[6*point_index+3])
#define AXS_GET_POINT_Y(buf,point_index) (((uint16_t)(buf[6*point_index+4] & 0x0F) <<8) + (uint16_t)buf[6*point_index+5])
#define AXS_GET_POINT_EVENT(buf,point_index) (buf[6*point_index+2] >> 6)
#define TOUCH_ADDRESS               0x3B
i2c_master_dev_handle_t             i2c_device;

#elif defined(CONFIG_LILYGO_T_HMI)
extern "C" {
    void board_hmi_touch_init();
    uint8_t board_hmi_get_point(uint16_t *x, uint16_t *y);
}


#elif defined(CONFIG_LILYGO_T_RGB)

extern "C" {
    void board_rgb_touch_init();
    uint8_t board_rgb_get_point(uint16_t *x, uint16_t *y);
}

#endif

static bool _init_success = false;


void touch_home_button_callback(void *args)
{
    ESP_LOGI(TAG, "Pressed Home button");
}

bool touch_init()
{
    ESP_LOGI(TAG, "Initialize Touchpanle");


#if defined(CONFIG_LILYGO_T_DISPLAY_LONG)

    extern i2c_master_bus_handle_t bus_handle;
    i2c_device_config_t i2c_dev_conf = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = TOUCH_ADDRESS,
        .scl_speed_hz = 100000,
    };
    i2c_master_bus_add_device(bus_handle, &i2c_dev_conf, &i2c_device);
#elif defined(CONFIG_LILYGO_T_HMI)

    board_hmi_touch_init();

#elif defined(CONFIG_LILYGO_T_RGB)

    board_rgb_touch_init();

#else
    touch.setPins(BOARD_TOUCH_RST, BOARD_TOUCH_IRQ);

    if (!touch.begin(bus_handle, TOUCH_ADDRESS)) {
        ESP_LOGE(TAG, "Touch init failed!");
        return false;
    }

    touch.setMaxCoordinates(AMOLED_HEIGHT, AMOLED_WIDTH);

#if CONFIG_LILYGO_T_AMOLED_LITE_147
    touch.setSwapXY(true);
    touch.setMirrorXY(false, true);
#elif CONFIG_LILYGO_T_DISPLAY_S3_PRO
    touch.setSwapXY(true);
    touch.setMirrorXY(false, true);

    touch.setCenterButtonCoordinate(85, 360);
    touch.setHomeButtonCallback(touch_home_button_callback, NULL);

#elif defined(CONFIG_LILYGO_T_DISPLAY_S3)
    touch.setSwapXY(true);
    touch.setMirrorXY(true, false);
    touch.disableAutoSleep();
#elif defined(CONFIG_LILYGO_T_QT_C6)
    touch.setMirrorXY(false, true);
#endif




#endif

    _init_success = true;
    return true;
}

uint8_t touch_get_data(int16_t *x, int16_t *y, uint8_t point_num)
{
    uint8_t touched = 0;

#if defined(CONFIG_LILYGO_T_DISPLAY_LONG)
    uint16_t pointX;
    uint16_t pointY;
    uint16_t type = 0;

    uint8_t cmd[11] = {0xb5, 0xab, 0xa5, 0x5a, 0x0, 0x0, 0x0, 0x8};
    uint8_t buffer[20] = {0};

    if (ESP_OK != i2c_master_transmit_receive(
                i2c_device,
                cmd,
                sizeof(cmd) / sizeof(*cmd),
                buffer,
                20,
                -1)) {
        return 0 ;
    }

    type = AXS_GET_GESTURE_TYPE(buffer);
    pointX = AXS_GET_POINT_X(buffer, 0);
    pointY = AXS_GET_POINT_Y(buffer, 0);

    if (!type && (pointX || pointY)) {
        *x = pointY;
        *y = 640 - pointX;
        ESP_LOGI(TAG, "T:%d X:%d Y:%d", type, *x, *y);
        touched = 1;
    }

#elif defined(CONFIG_LILYGO_T_HMI)

    uint16_t pointX;
    uint16_t pointY;
    touched = board_hmi_get_point(&pointX, &pointY);
    if (touched) {
        *x = pointX;
        *y = pointY;

    }
#elif defined(CONFIG_LILYGO_T_RGB)
    uint16_t pointX;
    uint16_t pointY;
    touched = board_rgb_get_point(&pointX, &pointY);
    if (touched) {
        *x = pointX;
        *y = pointY;

    }
#else

    if (!_init_success)return 0;
    touched =  touch.getPoint(x, y, point_num);
    if (touched) {
        ESP_LOGI(TAG, "X:%d Y:%d touched:%d\n", *x, *y, touched);
    }

#endif
    return touched;
}
#else

bool touch_init()
{
    return true;
}
#endif