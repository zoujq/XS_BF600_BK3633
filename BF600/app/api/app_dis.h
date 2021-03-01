/**
 ****************************************************************************************
 *
 * @file app_dis.h
 *
 * @brief Device Information Application Module Entry point
 *
 * Copyright (C) RivieraWaves 2009-2015
 *
 *
 ****************************************************************************************
 */

#ifndef APP_DIS_H_
#define APP_DIS_H_

/**
 ****************************************************************************************
 * @addtogroup APP
 *
 * @brief Device Information Application Module Entry point.
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"     // SW Configuration
#include "rwapp_config.h"
#if (BLE_APP_DIS)

#include <stdint.h>

/*
 * DEFINES
 ****************************************************************************************
 */

/// Manufacturer Name Value
#define APP_DIS_MANUFACTURER_NAME       ("Beurer GmbH")
#define APP_DIS_MANUFACTURER_NAME_LEN   (11)

/// Model Number String Value
#define APP_DIS_MODEL_NB_STR            ("BF600")
#define APP_DIS_MODEL_NB_STR_LEN        (5)

/// Serial Number
#define APP_DIS_SERIAL_NB_STR           ("S1.00")
#define APP_DIS_SERIAL_NB_STR_LEN       (5)

/// Firmware Revision
#define APP_DIS_FIRM_REV_STR            ("6.1")
#define APP_DIS_FIRM_REV_STR_LEN        (3)

/// System ID Value - LSB -> MSB
#define APP_DIS_SYSTEM_ID               ("\x10\x00\x00\x11\x25\x76")
#define APP_DIS_SYSTEM_ID_LEN           (6)

/// Hardware Revision String
#define APP_DIS_HARD_REV_STR           ("2.0")
#define APP_DIS_HARD_REV_STR_LEN       (3)

/// Software Revision String
#define APP_DIS_SW_REV_STR              ("2.0")
#define APP_DIS_SW_REV_STR_LEN          (3)

/// IEEE
#define APP_DIS_IEEE                    ("\xFF\xEE\xDD\xCC\xBB\xAA")
#define APP_DIS_IEEE_LEN                (6)

/**
 * PNP ID Value - LSB -> MSB
 *      Vendor ID Source : 0x02 (USB Implementer’s Forum assigned Vendor ID value)
 *      Vendor ID : 0x045E      (Microsoft Corp)
 *      Product ID : 0x0040
 *      Product Version : 0x0300
 */
#define APP_DIS_PNP_ID               ("\x02\x5E\x04\x40\x00\x00\x03")
#define APP_DIS_PNP_ID_LEN           (7)

#if (BLE_APP_HID)
#define APP_DIS_FEATURES             (DIS_MANUFACTURER_NAME_CHAR_SUP |\
                                      DIS_MODEL_NB_STR_CHAR_SUP      |\
                                      DIS_SYSTEM_ID_CHAR_SUP         |\
                                      DIS_PNP_ID_CHAR_SUP)

#elif (BLE_APP_AM0)
#define APP_DIS_FEATURES             (DIS_MANUFACTURER_NAME_CHAR_SUP |\
                                      DIS_MODEL_NB_STR_CHAR_SUP      |\
                                      DIS_FIRM_REV_STR_CHAR_SUP         |\
                                      DIS_SW_REV_STR_CHAR_SUP)
#else
#define APP_DIS_FEATURES             (DIS_ALL_FEAT_SUP)
#endif //(BLE_APP_HID)


/*
 * GLOBAL VARIABLES DECLARATION
 ****************************************************************************************
 */

/// Table of message handlers
extern const struct app_subtask_handlers app_dis_handlers;

/*
 * GLOBAL FUNCTIONS DECLARATION
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Initialize Device Information Service Application
 ****************************************************************************************
 */
void app_dis_init(void);

/**
 ****************************************************************************************
 * @brief Add a Device Information Service instance in the DB
 ****************************************************************************************
 */
void app_dis_add_dis(void);

/**
 ****************************************************************************************
 * @brief Enable the Device Information Service
 ****************************************************************************************
 */
void app_dis_enable_prf(uint16_t conhdl);

#endif //BLE_APP_DIS

/// @} APP

#endif //APP_DIS_H_
