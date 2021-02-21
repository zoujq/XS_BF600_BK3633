/**
 ****************************************************************************************
 *
 * @file app_f020.c
 *
 * @brief f020 Application Module entry point
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
#ifndef APP_F020_H_
#define APP_F020_H_
/**
 ****************************************************************************************
 * @addtogroup APP
 * @ingroup RICOW
 *
 * @brief f020 Application Module entry point
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

/// f020s Application Module Environment Structure
struct app_f020_env_tag
{
    /// Connection handle
    uint8_t conidx;
    /// Current Battery Level
    uint8_t f021_lvl;
	uint8_t f023_lvl;
};
/*
 * GLOBAL VARIABLES DECLARATIONS
 ****************************************************************************************
 */

/// f020s Application environment
extern struct app_f020_env_tag app_f020_env;

/// Table of message handlers
extern const struct app_subtask_handlers app_f020_handler;

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
void app_f020_init(void);
/**
 ****************************************************************************************
 * @brief Add a f020 Service instance in the DB
 ****************************************************************************************
 */
void app_f020_add_f020s(void);
/**
 ****************************************************************************************
 * @brief Enable the f020 Service
 ****************************************************************************************
 */
void app_f024_send_ntf(uint8_t conidx,uint16_t len,uint8_t* buf);
/**
 ****************************************************************************************
 * @brief Send a f025  value
 ****************************************************************************************
 */
void app_f025_send_ind(uint8_t conidx,uint16_t len,uint8_t* buf);


#endif // APP_F020_H_
