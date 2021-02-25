/**
 ****************************************************************************************
 *
 * @file app_f000.c
 *
 * @brief f000 Application Module entry point
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
#ifndef APP_F000_H_
#define APP_F000_H_
/**
 ****************************************************************************************
 * @addtogroup APP
 * @ingroup RICOW
 *
 * @brief f000 Application Module entry point
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

/// f000s Application Module Environment Structure
struct app_f000_env_tag
{
    /// Connection handle
    uint8_t conidx;
    /// Current Battery Level
    uint8_t f001_lvl;
	uint8_t f003_lvl;
};
/*
 * GLOBAL VARIABLES DECLARATIONS
 ****************************************************************************************
 */

/// f000s Application environment
extern struct app_f000_env_tag app_f000_env;

/// Table of message handlers
extern const struct app_subtask_handlers app_f000_handler;

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
void app_f000_init(void);
/**
 ****************************************************************************************
 * @brief Add a f000 Service instance in the DB
 ****************************************************************************************
 */
void app_f000_add_f000s(void);
/**
 ****************************************************************************************
 * @brief Enable the f000 Service
 ****************************************************************************************
 */
void app_f004_send_ntf(uint8_t conidx,uint16_t len,uint8_t* buf);
/**
 ****************************************************************************************
 * @brief Send a f002  value
 ****************************************************************************************
 */
void app_f002_send_ind(uint8_t conidx,uint16_t len,uint8_t* buf);


#endif // APP_F000_H_
