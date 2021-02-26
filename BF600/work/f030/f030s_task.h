/**
 ****************************************************************************************
 *
 * @file f030s_task.h
 *
 * @brief Header file - Battery Service Server Role Task.
 *
 * Copyright (C) RivieraWaves 2009-2015
 *
 *
 ****************************************************************************************
 */


#ifndef _F030S_TASK_H_
#define _F030S_TASK_H_


#include "rwprf_config.h"
#if (BLE_F030_SERVER)
#include <stdint.h>
#include "rwip_task.h" // Task definitions
/*
 * DEFINES
 ****************************************************************************************
 */

///Maximum number of FFF0 Server task instances
#define F030S_IDX_MAX     0x01
///Maximal number of F030 that can be added in the DB

#define  F030_CHAR_DATA_LEN  30 
#define  F030_F032_DATA_LEN  12 
#define  F030_F031_DATA_LEN  8 
#define  F030_F034_DATA_LEN  1 
#define  F030_F033_DATA_LEN  1 
#define  F030_F035_DATA_LEN  4 

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

/// Possible states of the FFF0S task
enum f030s_state
{
    /// Idle state
    F030S_IDLE,
    /// busy state
    F030S_BUSY,
    /// Number of defined states.
    F030S_STATE_MAX
};

/// Messages for FFF0 Server
enum f030s_msg_id
{
    /// Start the FFF0 Server - at connection used to restore bond data
	F030S_CREATE_DB_REQ   = TASK_FIRST_MSG(TASK_ID_F030S),
	
    F030S_F031_VALUE_READ_IND,
    F030S_F035_WRITER_CMD_IND,
	F030S_F033_WRITER_REQ_IND,

    
    /// FFF1 char Value Update Request
    F030S_F032_VALUE_UPD_REQ,
    /// Inform APP the F032 value has been send to stack
    F030S_F032_VALUE_UPD_RSP,
    /// Inform APP that FFF1 Level Notification Configuration has been changed - use to update bond data
    F030S_F032_VALUE_NTF_CFG_IND,

	/// F034 Level Value Update Request
    F030S_F034_VALUE_UPD_REQ,
    /// Inform APP if FFF1 Level value has been notified or not
    F030S_F034_VALUE_UPD_RSP,

	F030S_F034_VALUE_IND_CFG_IND,
	


};

/// Features Flag Masks
enum f030s_features
{
    /// F03X  Characteristic doesn't support notifications
    F030_F03X_VLUAE_NTF_NOT_SUP,
    /// F032 Value Characteristic support notifications
    F030_F032_VAL_NTF_SUP,
    /// F034 Value Characteristic support indication
    F030_F034_VAL_IND_SUP,
};


/*
 * APIs Structures
 ****************************************************************************************
 */

/// Parameters for the database creation
struct f030s_db_cfg
{
    /// Number of F030 to add
    uint8_t f030_nb;
    /// Features of each F030 instance
    uint8_t features;
    
    uint16_t f031_desc_len;
    uint8_t f031_desc[F030_CHAR_DATA_LEN];
};

/// Parameters of the @ref F030S_CREATE_DB_REQ message
struct f030s_enable_req
{
    /// connection index
    uint8_t  conidx;
    /// Notification Configuration
    uint16_t  ntf_cfg;

};


///Parameters of the @ref F030S_F03x_VALUE_UPD_REQ message
struct f030s_f0325_val_upd_req
{
    ///  instance
    uint8_t conidx;
	
	uint16_t length;
    /// char value
    uint8_t value[F030_CHAR_DATA_LEN];
};


///Parameters of the @ref F030S_F032_VALUE_UPD_RSP message
struct f030s_f0325_val_upd_rsp
{
    /// connection index
    uint8_t  conidx;
    ///status
    uint8_t status;
};

///Parameters of the @ref F030S_F032_VALUE_NTF_CFG_IND message
struct f030s_f032_val_ntf_cfg_ind
{
    /// connection index
    uint8_t  conidx;
    ///Notification Configuration
    uint16_t  ntf_cfg;
};


///Parameters of the @ref F030S_F034_VALUE_IND_CFG_IND message
struct f030s_f034_val_ind_cfg_ind
{
    /// connection index
    uint8_t  conidx;
    ///Ind Configuration
    uint16_t  ind_cfg;
};

/// Parameters of the @ref F030S_F035_WRITER_CMD_IND message
struct f030s_f0353_writer_ind
{
  /// Connection index
    uint8_t conidx;
    uint16_t length;
    ///  value
    uint8_t value[F030_CHAR_DATA_LEN];

};


/*
 * TASK DESCRIPTOR DECLARATIONS
 ****************************************************************************************
 */

extern const struct ke_state_handler f030s_default_handler;
#endif // BLE_F030_SERVER


#endif /* _F030S_TASK_H_ */

