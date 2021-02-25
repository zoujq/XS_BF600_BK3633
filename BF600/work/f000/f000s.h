/**
 ****************************************************************************************
 *
 * @file f000s.h
 *
 * @brief Header file - F000 Service Server Role
 *
 * Copyright (C) beken 2019-2022
 *
 *
 ****************************************************************************************
 */
#ifndef _F000S_H_
#define _F000S_H_

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

#if (BLE_F000_SERVER)

#include "f000s_task.h"
#include "atts.h"
#include "prf_types.h"
#include "prf.h"

/*
 * DEFINES
 ****************************************************************************************
 */

#define F000S_CFG_FLAG_MANDATORY_MASK       (0x3F)

enum
{		
	ATT_USER_SERVER_F000 						= ATT_UUID_16(0x181D),	// service 
    ATT_USER_SERVER_CHAR_F001                   = ATT_UUID_16(0x2a9e), // ind   
    ATT_USER_SERVER_CHAR_F002					= ATT_UUID_16(0x2a9d), // ind		
};

/// F000S Service Attributes Indexes
enum
{
	F000S_IDX_SVC,

    F000S_IDX_F001_VAL_CHAR,
	F000S_IDX_F001_VAL_VAL,

	F000S_IDX_F002_VAL_CHAR,
	F000S_IDX_F002_VAL_VAL,
	F000S_IDX_F002_VAL_IND_CFG,

	F000S_IDX_NB,
};

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */


/// F000 'Profile' Server environment variable
struct f000s_env_tag
{
    /// profile environment
    prf_env_t prf_env;
   
    /// On-going operation
    struct ke_msg * operation;   
    /// List of values set by application
    struct co_list values;
    /// FFF0 Services Start Handle
    uint16_t start_hdl;
    /// value of the F00x
    uint8_t f001_val[F000_F001_CHAR_DATA_LEN];
    uint8_t f007_val[F000_CHAR_DATA_LEN];
    uint8_t f003_val[F000_CHAR_DATA_LEN];
    uint8_t f004_val[F000_CHAR_DATA_LEN];
    uint8_t f002_val[F000_F002_CHAR_DATA_LEN];
    uint8_t f001_desc[F000_CHAR_DATA_LEN];
    uint8_t f001_desc_len;
    /// BASS task state
    ke_state_t state[F000S_IDX_MAX];
    /// Notification configuration of peer devices.
    uint16_t ntf_cfg[BLE_CONNECTION_MAX];
    /// Notification configuration of peer devices.
    uint16_t ind_cfg[BLE_CONNECTION_MAX];
    /// Database features
    uint8_t features;

};



/**
 ****************************************************************************************
 * @brief Retrieve f000 service profile interface
 *
 * @return f000 service profile interface
 ****************************************************************************************
 */
const struct prf_task_cbs* f000s_prf_itf_get(void);

uint16_t f000s_get_att_handle(uint8_t att_idx);

uint8_t  f000s_get_att_idx(uint16_t handle, uint8_t *att_idx);

void f000s_notify_f004_val(uint8_t conidx,struct f000s_env_tag* f000s_env, struct f000s_f0045_val_upd_req const *param);

void f000s_indicate_f002_val(uint8_t conidx,struct f000s_env_tag* f000s_env, struct f000s_f0045_val_upd_req const *param);
void f000s_task_init(struct ke_task_desc *task_desc);

#endif /* #if (BLE_F000_SERVERs) */



#endif /*  _F000_H_ */



