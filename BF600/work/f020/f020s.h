/**
 ****************************************************************************************
 *
 * @file f020s.h
 *
 * @brief Header file - F020 Service Server Role
 *
 * Copyright (C) beken 2019-2022
 *
 *
 ****************************************************************************************
 */
#ifndef _F020S_H_
#define _F020S_H_

/**
 ****************************************************************************************
 * @addtogroup  FFF0 'Profile' Server
 * @ingroup FFF0
 * @brief FFF0 'Profile' Server
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"
#include "rwprf_config.h"

#if (BLE_F020_SERVER)

#include "f020s_task.h"
#include "atts.h"
#include "prf_types.h"
#include "prf.h"

/*
 * DEFINES
 ****************************************************************************************
 */

#define F020S_CFG_FLAG_MANDATORY_MASK       (0x3FFF)

enum
{		
	ATT_USER_SERVER_F020 						= ATT_UUID_16(0xF020),	// service 
	ATT_USER_SERVER_CHAR_F021                   = ATT_UUID_16(0xF021), // read		
	ATT_USER_SERVER_CHAR_F022					= ATT_UUID_16(0xF022),// write cmd
	ATT_USER_SERVER_CHAR_F023					= ATT_UUID_16(0xF023),// write req
    ATT_USER_SERVER_CHAR_F024					= ATT_UUID_16(0xF024), // ntf
    ATT_USER_SERVER_CHAR_F025					= ATT_UUID_16(0xF025), // ind		
};

/// F020S Service Attributes Indexes
enum
{
	F020S_IDX_SVC,

    F020S_IDX_F021_VAL_CHAR,
	F020S_IDX_F021_VAL_VAL,
    F020S_IDX_F021_USER_DESC,
    
	F020S_IDX_F022_VAL_CHAR,
	F020S_IDX_F022_VAL_VAL,

    F020S_IDX_F023_VAL_CHAR,
	F020S_IDX_F023_VAL_VAL,
    
	F020S_IDX_F024_VAL_CHAR,
	F020S_IDX_F024_VAL_VAL,
	F020S_IDX_F024_VAL_NTF_CFG,

	F020S_IDX_F025_VAL_CHAR,
	F020S_IDX_F025_VAL_VAL,
	F020S_IDX_F025_VAL_IND_CFG,

	F020S_IDX_NB,
};

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */


/// F020 'Profile' Server environment variable
struct f020s_env_tag
{
    /// profile environment
    prf_env_t prf_env;
   
    /// On-going operation
    struct ke_msg * operation;   
    /// List of values set by application
    struct co_list values;
    /// FFF0 Services Start Handle
    uint16_t start_hdl;
    /// value of the F02x
    uint8_t f021_val[F020_CHAR_DATA_LEN];
    uint8_t f022_val[F020_CHAR_DATA_LEN];
    uint8_t f023_val[F020_CHAR_DATA_LEN];
    uint8_t f024_val[F020_CHAR_DATA_LEN];
    uint8_t f025_val[F020_CHAR_DATA_LEN];
    uint8_t f021_desc[F020_CHAR_DATA_LEN];
    uint8_t f021_desc_len;
    /// BASS task state
    ke_state_t state[F020S_IDX_MAX];
    /// Notification configuration of peer devices.
    uint16_t ntf_cfg[BLE_CONNECTION_MAX];
    /// Notification configuration of peer devices.
    uint16_t ind_cfg[BLE_CONNECTION_MAX];
    /// Database features
    uint8_t features;

};



/**
 ****************************************************************************************
 * @brief Retrieve f020 service profile interface
 *
 * @return f020 service profile interface
 ****************************************************************************************
 */
const struct prf_task_cbs* f020s_prf_itf_get(void);

uint16_t f020s_get_att_handle(uint8_t att_idx);

uint8_t  f020s_get_att_idx(uint16_t handle, uint8_t *att_idx);

void f020s_notify_f024_val(uint8_t conidx,struct f020s_env_tag* f020s_env, struct f020s_f0245_val_upd_req const *param);

void f020s_indicate_f025_val(uint8_t conidx,struct f020s_env_tag* f020s_env, struct f020s_f0245_val_upd_req const *param);
void f020s_task_init(struct ke_task_desc *task_desc);

#endif /* #if (BLE_F020_SERVERs) */



#endif /*  _F020_H_ */



