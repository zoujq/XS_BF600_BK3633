/**
 ****************************************************************************************
 *
 * @file app_f010.c
 *
 * @brief f010 Application Module entry point
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
#ifndef APP_F010_H_
#define APP_F010_H_
/**
 ****************************************************************************************
 * @addtogroup APP
 * @ingroup RICOW
 *
 * @brief f010 Application Module entry point
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

/// f010s Application Module Environment Structure
struct app_f010_env_tag
{
    /// Connection handle
    uint8_t conidx;
    /// Current Battery Level
    uint8_t f011_lvl;
	uint8_t f013_lvl;
};
/*
 * GLOBAL VARIABLES DECLARATIONS
 ****************************************************************************************
 */

/// f010s Application environment
extern struct app_f010_env_tag app_f010_env;

/// Table of message handlers
extern const struct app_subtask_handlers app_f010_handler;

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
void app_f010_init(void);
/**
 ****************************************************************************************
 * @brief Add a f010 Service instance in the DB
 ****************************************************************************************
 */
void app_f010_add_f010s(void);
/**
 ****************************************************************************************
 * @brief Enable the f010 Service
 ****************************************************************************************
 */
void app_f014_send_ntf(uint8_t conidx,uint16_t len,uint8_t* buf);
/**
 ****************************************************************************************
 * @brief Send a f012  value
 ****************************************************************************************
 */
void app_f012_send_ind(uint8_t conidx,uint16_t len,uint8_t* buf);


#endif // APP_F010_H_
