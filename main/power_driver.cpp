/**
 * @file      power_driver.cpp
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2024  Shenzhen Xinyuan Electronic Technology Co., Ltd
 * @date      2024-01-08
 *
 */
#include <stdio.h>
#include <cstring>
#include "sdkconfig.h"
#include "esp_log.h"
#include "esp_err.h"
#include "i2c_driver.h"
#include "product_pins.h"
#include "driver/gpio.h"

static const char *TAG = "POWER";


#if CONFIG_PMU_AXP2101

#include "XPowersAXP2101.tpp"


XPowersAXP2101 PMU;

bool power_driver_init()
{
    if (PMU.begin(bus_handle, AXP2101_SLAVE_ADDRESS)) {
        ESP_LOGI(TAG, "Init PMU SUCCESS!");
    } else {
        ESP_LOGE(TAG, "Init PMU FAILED!");
        return false;
    }

    PMU.clearIrqStatus();


#if defined(CONFIG_LILYGO_T_WATCH_S3)
    // ! ESP32S3 VDD, Don't change
    // setDC1Voltage(3300);

    //! RTC VBAT , Don't change
    PMU.setALDO1Voltage(3300);

    //! TFT BACKLIGHT VDD , Don't change
    PMU.setALDO2Voltage(3300);

    //!Screen touch VDD , Don't change
    PMU.setALDO3Voltage(3300);

    //! Radio VDD , Don't change
    PMU.setALDO4Voltage(3300);

    //!DRV2605 enable
    PMU.setBLDO2Voltage(3300);

    //! GPS Power
    PMU.setDC3Voltage(3300);
    PMU.enableDC3();

    //! No use
    PMU.disableDC2();
    // PMU.disableDC3();
    PMU.disableDC4();
    PMU.disableDC5();
    PMU.disableBLDO1();
    PMU.disableCPUSLDO();
    PMU.disableDLDO1();
    PMU.disableDLDO2();


    PMU.enableALDO1();  //! RTC VBAT
    PMU.enableALDO2();  //! TFT BACKLIGHT   VDD
    PMU.enableALDO3();  //! Screen touch VDD
    PMU.enableALDO4();  //! Radio VDD
    PMU.enableBLDO2();  //! drv2605 enable

#else

    PMU.setChargingLedMode(XPOWERS_CHG_LED_BLINK_4HZ);

    // ALDO1 = AMOLED logic power & Sensor Power voltage
    PMU.setALDO1Voltage(1800);
    PMU.enableALDO1();

    // ALDO3 = Level conversion enable and AMOLED power supply
    PMU.setALDO3Voltage(3300);
    PMU.enableALDO3();

    // BLDO1 = AMOLED LOGIC POWER 1.8V
    PMU.setBLDO1Voltage(1800);
    PMU.enableBLDO1();

    // No use power channel
    PMU.disableDC2();
    PMU.disableDC3();
    PMU.disableDC4();
    PMU.disableDC5();
    PMU.disableCPUSLDO();

    // Enable PMU ADC
    PMU.enableBattDetection();
    PMU.enableVbusVoltageMeasure();
    PMU.enableBattVoltageMeasure();
#endif

    ESP_LOGI(TAG, "DCDC========================");
    ESP_LOGI(TAG, "DC1  : %s   Voltage:%u mV ",  PMU.isEnableDC1()  ? "+" : "-", PMU.getDC1Voltage());
    ESP_LOGI(TAG, "DC2  : %s   Voltage:%u mV ",  PMU.isEnableDC2()  ? "+" : "-", PMU.getDC2Voltage());
    ESP_LOGI(TAG, "DC3  : %s   Voltage:%u mV ",  PMU.isEnableDC3()  ? "+" : "-", PMU.getDC3Voltage());
    ESP_LOGI(TAG, "DC4  : %s   Voltage:%u mV ",  PMU.isEnableDC4()  ? "+" : "-", PMU.getDC4Voltage());
    ESP_LOGI(TAG, "DC5  : %s   Voltage:%u mV ",  PMU.isEnableDC5()  ? "+" : "-", PMU.getDC5Voltage());
    ESP_LOGI(TAG, "ALDO========================");
    ESP_LOGI(TAG, "ALDO1: %s   Voltage:%u mV",  PMU.isEnableALDO1()  ? "+" : "-", PMU.getALDO1Voltage());
    ESP_LOGI(TAG, "ALDO2: %s   Voltage:%u mV",  PMU.isEnableALDO2()  ? "+" : "-", PMU.getALDO2Voltage());
    ESP_LOGI(TAG, "ALDO3: %s   Voltage:%u mV",  PMU.isEnableALDO3()  ? "+" : "-", PMU.getALDO3Voltage());
    ESP_LOGI(TAG, "ALDO4: %s   Voltage:%u mV",  PMU.isEnableALDO4()  ? "+" : "-", PMU.getALDO4Voltage());
    ESP_LOGI(TAG, "BLDO========================");
    ESP_LOGI(TAG, "BLDO1: %s   Voltage:%u mV",  PMU.isEnableBLDO1()  ? "+" : "-", PMU.getBLDO1Voltage());
    ESP_LOGI(TAG, "BLDO2: %s   Voltage:%u mV",  PMU.isEnableBLDO2()  ? "+" : "-", PMU.getBLDO2Voltage());
    ESP_LOGI(TAG, "CPUSLDO========================");
    ESP_LOGI(TAG, "CPUSLDO: %s Voltage:%u mV",  PMU.isEnableCPUSLDO() ? "+" : "-", PMU.getCPUSLDOVoltage());
    ESP_LOGI(TAG, "DLDO========================");
    ESP_LOGI(TAG, "DLDO1: %s   Voltage:%u mV",  PMU.isEnableDLDO1()  ? "+" : "-", PMU.getDLDO1Voltage());
    ESP_LOGI(TAG, "DLDO2: %s   Voltage:%u mV",  PMU.isEnableDLDO2()  ? "+" : "-", PMU.getDLDO2Voltage());
    ESP_LOGI(TAG, "============================");


    return true;
}

#elif CONFIG_PMU_SY6970

#include "PowersSY6970.tpp"

PowersSY6970 PMU;

bool power_driver_init()
{
    if (PMU.begin(bus_handle, SY6970_SLAVE_ADDRESS)) {
        ESP_LOGI(TAG, "Init PMU SUCCESS!");
    } else {
        ESP_LOGE(TAG, "Init PMU FAILED!");
        return false;
    }
    PMU.enableADCMeasure();
    PMU.disableOTG();

    return true;
}
#else

bool power_driver_init()
{
#ifdef BOARD_POWERON
    ESP_LOGI(TAG, "Turn on board power pin");
    gpio_config_t poweron_gpio_config = {0};
    poweron_gpio_config.pin_bit_mask = 1ULL << BOARD_POWERON;
    poweron_gpio_config.mode = GPIO_MODE_OUTPUT;
    ESP_ERROR_CHECK(gpio_config(&poweron_gpio_config));
    gpio_set_level(BOARD_POWERON, 1);
#endif

#ifdef BOARD_POWERON1
    ESP_LOGI(TAG, "Turn on board power pin");
    gpio_config_t poweron1_gpio_config = {0};
    poweron1_gpio_config.pin_bit_mask = 1ULL << BOARD_POWERON1;
    poweron1_gpio_config.mode = GPIO_MODE_OUTPUT;
    ESP_ERROR_CHECK(gpio_config(&poweron1_gpio_config));
    gpio_set_level(BOARD_POWERON1, 1);
#endif

    return true;
}
#endif