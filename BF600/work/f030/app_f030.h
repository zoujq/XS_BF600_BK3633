/**
 ****************************************************************************************
 *
 * @file app_f030.c
 *
 * @brief f030 Application Module entry point
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
#ifndef APP_F030_H_
#define APP_F030_H_
/**
 ****************************************************************************************
 * @addtogroup APP
 * @ingroup RICOW
 *
 * @brief f030 Application Module entry point
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

/// f030s Application Module Environment Structure
struct app_f030_env_tag
{
    /// Connection handle
    uint8_t conidx;
    /// Current Battery Level
    uint8_t f031_lvl;
	uint8_t f033_lvl;
};
/*
 * GLOBAL VARIABLES DECLARATIONS
 ****************************************************************************************
 */

/// f030s Application environment
extern struct app_f030_env_tag app_f030_env;

/// Table of message handlers
extern const struct app_subtask_handlers app_f030_handler;

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
void app_f030_init(void);
/**
 ****************************************************************************************
 * @brief Add a f030 Service instance in the DB
 ****************************************************************************************
 */
void app_f030_add_f030s(void);
/**
 ****************************************************************************************
 * @brief Enable the f030 Service
 ****************************************************************************************
 */
void app_f034_send_ntf(uint8_t conidx,uint16_t len,uint8_t* buf);
/**
 ****************************************************************************************
 * @brief Send a f035  value
 ****************************************************************************************
 */
void app_f035_send_ind(uint8_t conidx,uint16_t len,uint8_t* buf);


#endif // APP_F030_H_
