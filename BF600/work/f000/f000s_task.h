/**
 ****************************************************************************************
 *
 * @file f000s_task.h
 *
 * @brief Header file - Battery Service Server Role Task.
 *
 * Copyright (C) RivieraWaves 2009-2015
 *
 *
 ****************************************************************************************
 */


#ifndef _F000S_TASK_H_
#define _F000S_TASK_H_


#include "rwprf_config.h"
#if (BLE_F000_SERVER)
#include <stdint.h>
#include "rwip_task.h" // Task definitions
/*
 * DEFINES
 ****************************************************************************************
 */

///Maximum number of FFF0 Server task instances
#define F000S_IDX_MAX     0x01
///Maximal number of F000 that can be added in the DB

#define  F000_CHAR_DATA_LEN  16
#define  F000_F001_CHAR_DATA_LEN  4
#define  F000_F002_CHAR_DATA_LEN  16

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

/// Possible states of the FFF0S task
enum f000s_state
{
    /// Idle state
    F000S_IDLE,
    /// busy state
    F000S_BUSY,
    /// Number of defined states.
    F000S_STATE_MAX
};

/// Messages for FFF0 Server
enum f000s_msg_id
{
    /// Start the FFF0 Server - at connection used to restore bond data
	F000S_CREATE_DB_REQ   = TASK_FIRST_MSG(TASK_ID_F000S),
	
    F000S_F001_VALUE_READ_IND,
    F000S_F007_WRITER_CMD_IND,
	F000S_F003_WRITER_REQ_IND,

    
    /// FFF1 char Value Update Request
    F000S_F004_VALUE_UPD_REQ,
    /// Inform APP the F004 value has been send to stack
    F000S_F004_VALUE_UPD_RSP,
    /// Inform APP that FFF1 Level Notification Configuration has been changed - use to update bond data
    F000S_F004_VALUE_NTF_CFG_IND,

	/// F002 Level Value Update Request
    F000S_F002_VALUE_UPD_REQ,
    /// Inform APP if FFF1 Level value has been notified or not
    F000S_F002_VALUE_UPD_RSP,

	F000S_F002_VALUE_IND_CFG_IND,
	


};

/// Features Flag Masks
enum f000s_features
{
    /// F00X  Characteristic doesn't support notifications
    F000_F00X_VLUAE_NTF_NOT_SUP,
    /// F004 Value Characteristic support notifications
    F000_F004_VAL_NTF_SUP,
    /// F002 Value Characteristic support indication
    F000_F002_VAL_IND_SUP,
};


/*
 * APIs Structures
 ****************************************************************************************
 */

/// Parameters for the database creation
struct f000s_db_cfg
{
    /// Number of F000 to add
    uint8_t f000_nb;
    /// Features of each F000 instance
    uint8_t features;
    
    uint16_t f001_desc_len;
    uint8_t f001_desc[F000_CHAR_DATA_LEN];
};

/// Parameters of the @ref F000S_CREATE_DB_REQ message
struct f000s_enable_req
{
    /// connection index
    uint8_t  conidx;
    /// Notification Configuration
    uint16_t  ntf_cfg;

};


///Parameters of the @ref F000S_F00x_VALUE_UPD_REQ message
struct f000s_f0045_val_upd_req
{
    ///  instance
    uint8_t conidx;
	
	uint16_t length;
    /// char value
    uint8_t value[F000_CHAR_DATA_LEN];
};


///Parameters of the @ref F000S_F004_VALUE_UPD_RSP message
struct f000s_f0045_val_upd_rsp
{
    /// connection index
    uint8_t  conidx;
    ///status
    uint8_t status;
};

///Parameters of the @ref F000S_F004_VALUE_NTF_CFG_IND message
struct f000s_f004_val_ntf_cfg_ind
{
    /// connection index
    uint8_t  conidx;
    ///Notification Configuration
    uint16_t  ntf_cfg;
};


///Parameters of the @ref F000S_F002_VALUE_IND_CFG_IND message
struct f000s_f002_val_ind_cfg_ind
{
    /// connection index
    uint8_t  conidx;
    ///Ind Configuration
    uint16_t  ind_cfg;
};

/// Parameters of the @ref F000S_F007_WRITER_CMD_IND message
struct f000s_f0073_writer_ind
{
  /// Connection index
    uint8_t conidx;
    uint16_t length;
    ///  value
    uint8_t value[F000_CHAR_DATA_LEN];

};


/*
 * TASK DESCRIPTOR DECLARATIONS
 ****************************************************************************************
 */

extern const struct ke_state_handler f000s_default_handler;
#endif // BLE_F000_SERVER


#endif /* _F000S_TASK_H_ */

