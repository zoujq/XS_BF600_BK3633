/**
 ****************************************************************************************
 *
 * @file f020s_task.h
 *
 * @brief Header file - Battery Service Server Role Task.
 *
 * Copyright (C) RivieraWaves 2009-2015
 *
 *
 ****************************************************************************************
 */


#ifndef _F020S_TASK_H_
#define _F020S_TASK_H_


#include "rwprf_config.h"
#if (BLE_F020_SERVER)
#include <stdint.h>
#include "rwip_task.h" // Task definitions
/*
 * DEFINES
 ****************************************************************************************
 */

///Maximum number of FFF0 Server task instances
#define F020S_IDX_MAX     0x01
///Maximal number of F020 that can be added in the DB
#define  F020_CHAR_DATA_LEN  30
#define  F020_F021_DATA_LEN  6
#define  F020_F022_DATA_LEN  2
#define  F020_F023_DATA_LEN  2
#define  F020_F024_DATA_LEN  6
#define  F020_F025_DATA_LEN  2
#define  F020_F026_DATA_LEN  30

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

/// Possible states of the FFF0S task
enum f020s_state
{
    /// Idle state
    F020S_IDLE,
    /// busy state
    F020S_BUSY,
    /// Number of defined states.
    F020S_STATE_MAX
};

/// Messages for FFF0 Server
enum f020s_msg_id
{
    /// Start the FFF0 Server - at connection used to restore bond data
	F020S_CREATE_DB_REQ   = TASK_FIRST_MSG(TASK_ID_F020S),
	
    F020S_F021_VALUE_READ_IND,
    F020S_F022_WRITER_CMD_IND,
	F020S_F023_WRITER_REQ_IND,

    
    /// FFF1 char Value Update Request
    F020S_F024_VALUE_UPD_REQ,
    /// Inform APP the F024 value has been send to stack
    F020S_F024_VALUE_UPD_RSP,
    /// Inform APP that FFF1 Level Notification Configuration has been changed - use to update bond data
    F020S_F024_VALUE_NTF_CFG_IND,

	/// F026 Level Value Update Request
    F020S_F026_VALUE_UPD_REQ,
    /// Inform APP if FFF1 Level value has been notified or not
    F020S_F026_VALUE_UPD_RSP,

	F020S_F026_VALUE_IND_CFG_IND,
	


};

/// Features Flag Masks
enum f020s_features
{
    /// F02X  Characteristic doesn't support notifications
    F020_F02X_VLUAE_NTF_NOT_SUP,
    /// F024 Value Characteristic support notifications
    F020_F024_VAL_NTF_SUP,
    /// F026 Value Characteristic support indication
    F020_F026_VAL_IND_SUP,
};


/*
 * APIs Structures
 ****************************************************************************************
 */

/// Parameters for the database creation
struct f020s_db_cfg
{
    /// Number of F020 to add
    uint8_t f020_nb;
    /// Features of each F020 instance
    uint8_t features;
    
    uint16_t f021_desc_len;
    uint8_t f021_desc[30];
};

/// Parameters of the @ref F020S_CREATE_DB_REQ message
struct f020s_enable_req
{
    /// connection index
    uint8_t  conidx;
    /// Notification Configuration
    uint16_t  ntf_cfg;

};


///Parameters of the @ref F020S_F02x_VALUE_UPD_REQ message
struct f020s_f0245_val_upd_req
{
    ///  instance
    uint8_t conidx;
	
	uint16_t length;
    /// char value
    uint8_t value[30];
};


///Parameters of the @ref F020S_F024_VALUE_UPD_RSP message
struct f020s_f0245_val_upd_rsp
{
    /// connection index
    uint8_t  conidx;
    ///status
    uint8_t status;
};

///Parameters of the @ref F020S_F024_VALUE_NTF_CFG_IND message
struct f020s_f024_val_ntf_cfg_ind
{
    /// connection index
    uint8_t  conidx;
    ///Notification Configuration
    uint16_t  ntf_cfg;
};


///Parameters of the @ref F020S_F026_VALUE_IND_CFG_IND message
struct f020s_f026_val_ind_cfg_ind
{
    /// connection index
    uint8_t  conidx;
    ///Ind Configuration
    uint16_t  ind_cfg;
};

/// Parameters of the @ref F020S_F022_WRITER_CMD_IND message
struct f020s_f0223_writer_ind
{
  /// Connection index
    uint8_t conidx;
    uint16_t length;
    ///  value
    uint8_t value[30];

};


/*
 * TASK DESCRIPTOR DECLARATIONS
 ****************************************************************************************
 */

extern const struct ke_state_handler f020s_default_handler;
#endif // BLE_F020_SERVER


#endif /* _F020S_TASK_H_ */

