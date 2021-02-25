/**
 ****************************************************************************************
 *
 * @file f010s_task.h
 *
 * @brief Header file - Battery Service Server Role Task.
 *
 * Copyright (C) RivieraWaves 2009-2015
 *
 *
 ****************************************************************************************
 */


#ifndef _F010S_TASK_H_
#define _F010S_TASK_H_


#include "rwprf_config.h"
#if (BLE_F010_SERVER)
#include <stdint.h>
#include "rwip_task.h" // Task definitions
/*
 * DEFINES
 ****************************************************************************************
 */

///Maximum number of FFF0 Server task instances
#define F010S_IDX_MAX     0x01
///Maximal number of F010 that can be added in the DB

#define  F010_CHAR_DATA_LEN  1
#define  F010_F011_DATA_LEN  4
#define  F010_F012_DATA_LEN  14
/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

/// Possible states of the FFF0S task
enum f010s_state
{
    /// Idle state
    F010S_IDLE,
    /// busy state
    F010S_BUSY,
    /// Number of defined states.
    F010S_STATE_MAX
};

/// Messages for FFF0 Server
enum f010s_msg_id
{
    /// Start the FFF0 Server - at connection used to restore bond data
	F010S_CREATE_DB_REQ   = TASK_FIRST_MSG(TASK_ID_F010S),
	
    F010S_F011_VALUE_READ_IND,
    F010S_F017_WRITER_CMD_IND,
	F010S_F013_WRITER_REQ_IND,

    
    /// FFF1 char Value Update Request
    F010S_F014_VALUE_UPD_REQ,
    /// Inform APP the F014 value has been send to stack
    F010S_F014_VALUE_UPD_RSP,
    /// Inform APP that FFF1 Level Notification Configuration has been changed - use to update bond data
    F010S_F014_VALUE_NTF_CFG_IND,

	/// F012 Level Value Update Request
    F010S_F012_VALUE_UPD_REQ,
    /// Inform APP if FFF1 Level value has been notified or not
    F010S_F012_VALUE_UPD_RSP,

	F010S_F012_VALUE_IND_CFG_IND,
	


};

/// Features Flag Masks
enum f010s_features
{
    /// F01X  Characteristic doesn't support notifications
    F010_F01X_VLUAE_NTF_NOT_SUP,
    /// F014 Value Characteristic support notifications
    F010_F014_VAL_NTF_SUP,
    /// F012 Value Characteristic support indication
    F010_F012_VAL_IND_SUP,
};


/*
 * APIs Structures
 ****************************************************************************************
 */

/// Parameters for the database creation
struct f010s_db_cfg
{
    /// Number of F010 to add
    uint8_t f010_nb;
    /// Features of each F010 instance
    uint8_t features;
    
    uint16_t f011_desc_len;
    uint8_t f011_desc[F010_CHAR_DATA_LEN];
};

/// Parameters of the @ref F010S_CREATE_DB_REQ message
struct f010s_enable_req
{
    /// connection index
    uint8_t  conidx;
    /// Notification Configuration
    uint16_t  ntf_cfg;

};


///Parameters of the @ref F010S_F01x_VALUE_UPD_REQ message
struct f010s_f0145_val_upd_req
{
    ///  instance
    uint8_t conidx;
	
	uint16_t length;
    /// char value
    uint8_t value[F010_CHAR_DATA_LEN];
};


///Parameters of the @ref F010S_F014_VALUE_UPD_RSP message
struct f010s_f0145_val_upd_rsp
{
    /// connection index
    uint8_t  conidx;
    ///status
    uint8_t status;
};

///Parameters of the @ref F010S_F014_VALUE_NTF_CFG_IND message
struct f010s_f014_val_ntf_cfg_ind
{
    /// connection index
    uint8_t  conidx;
    ///Notification Configuration
    uint16_t  ntf_cfg;
};


///Parameters of the @ref F010S_F012_VALUE_IND_CFG_IND message
struct f010s_f012_val_ind_cfg_ind
{
    /// connection index
    uint8_t  conidx;
    ///Ind Configuration
    uint16_t  ind_cfg;
};

/// Parameters of the @ref F010S_F017_WRITER_CMD_IND message
struct f010s_f0173_writer_ind
{
  /// Connection index
    uint8_t conidx;
    uint16_t length;
    ///  value
    uint8_t value[F010_CHAR_DATA_LEN];

};


/*
 * TASK DESCRIPTOR DECLARATIONS
 ****************************************************************************************
 */

extern const struct ke_state_handler f010s_default_handler;
#endif // BLE_F010_SERVER


#endif /* _F010S_TASK_H_ */

