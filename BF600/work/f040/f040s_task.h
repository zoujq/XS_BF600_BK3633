/**
 ****************************************************************************************
 *
 * @file f040s_task.h
 *
 * @brief Header file - Battery Service Server Role Task.
 *
 * Copyright (C) RivieraWaves 2009-2015
 *
 *
 ****************************************************************************************
 */


#ifndef _F040S_TASK_H_
#define _F040S_TASK_H_


#include "rwprf_config.h"
#if (BLE_F040_SERVER)
#include <stdint.h>
#include "rwip_task.h" // Task definitions
/*
 * DEFINES
 ****************************************************************************************
 */

///Maximum number of FFF0 Server task instances
#define F040S_IDX_MAX     0x01
///Maximal number of F040 that can be added in the DB

#define  F040_CHAR_DATA_LEN  628

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

/// Possible states of the FFF0S task
enum f040s_state
{
    /// Idle state
    F040S_IDLE,
    /// busy state
    F040S_BUSY,
    /// Number of defined states.
    F040S_STATE_MAX
};

/// Messages for FFF0 Server
enum f040s_msg_id
{
    /// Start the FFF0 Server - at connection used to restore bond data
	F040S_CREATE_DB_REQ   = TASK_FIRST_MSG(TASK_ID_F040S),
	
    F040S_F041_VALUE_READ_IND,
    F040S_F042_WRITER_CMD_IND,
	F040S_F043_WRITER_REQ_IND,

    
    /// FFF1 char Value Update Request
    F040S_F044_VALUE_UPD_REQ,
    /// Inform APP the F044 value has been send to stack
    F040S_F044_VALUE_UPD_RSP,
    /// Inform APP that FFF1 Level Notification Configuration has been changed - use to update bond data
    F040S_F044_VALUE_NTF_CFG_IND,

	/// F045 Level Value Update Request
    F040S_F045_VALUE_UPD_REQ,
    /// Inform APP if FFF1 Level value has been notified or not
    F040S_F045_VALUE_UPD_RSP,

	F040S_F045_VALUE_IND_CFG_IND,
	


};

/// Features Flag Masks
enum f040s_features
{
    /// F04X  Characteristic doesn't support notifications
    F040_F04X_VLUAE_NTF_NOT_SUP,
    /// F044 Value Characteristic support notifications
    F040_F044_VAL_NTF_SUP,
    /// F045 Value Characteristic support indication
    F040_F045_VAL_IND_SUP,
};


/*
 * APIs Structures
 ****************************************************************************************
 */

/// Parameters for the database creation
struct f040s_db_cfg
{
    /// Number of F040 to add
    uint8_t f040_nb;
    /// Features of each F040 instance
    uint8_t features;
    
    uint16_t f041_desc_len;
    uint8_t f041_desc[F040_CHAR_DATA_LEN];
};

/// Parameters of the @ref F040S_CREATE_DB_REQ message
struct f040s_enable_req
{
    /// connection index
    uint8_t  conidx;
    /// Notification Configuration
    uint16_t  ntf_cfg;

};


///Parameters of the @ref F040S_F04x_VALUE_UPD_REQ message
struct f040s_f0445_val_upd_req
{
    ///  instance
    uint8_t conidx;
	
	uint16_t length;
    /// char value
    uint8_t value[F040_CHAR_DATA_LEN];
};


///Parameters of the @ref F040S_F044_VALUE_UPD_RSP message
struct f040s_f0445_val_upd_rsp
{
    /// connection index
    uint8_t  conidx;
    ///status
    uint8_t status;
};

///Parameters of the @ref F040S_F044_VALUE_NTF_CFG_IND message
struct f040s_f044_val_ntf_cfg_ind
{
    /// connection index
    uint8_t  conidx;
    ///Notification Configuration
    uint16_t  ntf_cfg;
};


///Parameters of the @ref F040S_F045_VALUE_IND_CFG_IND message
struct f040s_f045_val_ind_cfg_ind
{
    /// connection index
    uint8_t  conidx;
    ///Ind Configuration
    uint16_t  ind_cfg;
};

/// Parameters of the @ref F040S_F042_WRITER_CMD_IND message
struct f040s_f0423_writer_ind
{
  /// Connection index
    uint8_t conidx;
    uint16_t length;
    ///  value
    uint8_t value[F040_CHAR_DATA_LEN];

};


/*
 * TASK DESCRIPTOR DECLARATIONS
 ****************************************************************************************
 */

extern const struct ke_state_handler f040s_default_handler;
#endif // BLE_F040_SERVER


#endif /* _F040S_TASK_H_ */

