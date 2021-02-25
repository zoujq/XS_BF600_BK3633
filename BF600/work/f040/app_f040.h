/**
 ****************************************************************************************
 *
 * @file app_f040.c
 *
 * @brief f040 Application Module entry point
 *
 * @auth  gang.cheng
 *
 * @date  2020.03.17
 *
 * Copyright (C) Beken 2020-2022
 *
 *
 ****************************************************************************************
 */
#ifndef APP_F040_H_
#define APP_F040_H_
/**
 ****************************************************************************************
 * @addtogroup APP
 * @ingroup RICOW
 *
 * @brief f040 Application Module entry point
 *
 * @{
 ****************************************************************************************
 */
/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"     // SW configuration


#include <stdint.h>          // Standard Integer Definition
#include "ke_task.h"         // Kernel Task Definition

/*
 * STRUCTURES DEFINITION
 ****************************************************************************************
 */

/// f040s Application Module Environment Structure
struct app_f040_env_tag
{
    /// Connection handle
    uint8_t conidx;
    /// Current Battery Level
    uint8_t f047_lvl;
	uint8_t f043_lvl;
};
/*
 * GLOBAL VARIABLES DECLARATIONS
 ****************************************************************************************
 */

/// f040s Application environment
extern struct app_f040_env_tag app_f040_env;

/// Table of message handlers
extern const struct app_subtask_handlers app_f040_handler;

/*
 * FUNCTIONS DECLARATION
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 *
 * fff0s Application Functions
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Initialize fff0s Application Module
 ****************************************************************************************
 */
void app_f040_init(void);
/**
 ****************************************************************************************
 * @brief Add a f040 Service instance in the DB
 ****************************************************************************************
 */
void app_f040_add_f040s(void);
/**
 ****************************************************************************************
 * @brief Enable the f040 Service
 ****************************************************************************************
 */
void app_f044_send_ntf(uint8_t conidx,uint16_t len,uint8_t* buf);
/**
 ****************************************************************************************
 * @brief Send a f041  value
 ****************************************************************************************
 */
void app_f041_send_ntf(uint8_t conidx,uint16_t len,uint8_t* buf);


#endif // APP_F040_H_
