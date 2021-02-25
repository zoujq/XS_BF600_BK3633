/**
 ****************************************************************************************
 *
 * @file f030s.h
 *
 * @brief Header file - F030 Service Server Role
 *
 * Copyright (C) beken 2019-2022
 *
 *
 ****************************************************************************************
 */
#ifndef _F030S_H_
#define _F030S_H_

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

#if (BLE_F030_SERVER)

#include "f030s_task.h"
#include "atts.h"
#include "prf_types.h"
#include "prf.h"

/*
 * DEFINES
 ****************************************************************************************
 */

#define F030S_CFG_FLAG_MANDATORY_MASK       (0x3FFF)

enum
{		
	ATT_USER_SERVER_F030 						= ATT_UUID_16(0xFFF0),	// service 
	ATT_USER_SERVER_CHAR_F031                   = ATT_UUID_16(0xfff1), // read		
	ATT_USER_SERVER_CHAR_F035					= ATT_UUID_16(0xfff5),// write cmd
	ATT_USER_SERVER_CHAR_F033					= ATT_UUID_16(0xfff3),// write req
    ATT_USER_SERVER_CHAR_F032					= ATT_UUID_16(0xfff2), // ntf
    ATT_USER_SERVER_CHAR_F034					= ATT_UUID_16(0xfff4), // ind		
};

/// F030S Service Attributes Indexes
enum
{
	F030S_IDX_SVC,

    F030S_IDX_F031_VAL_CHAR,
	F030S_IDX_F031_VAL_VAL,
    
	F030S_IDX_F035_VAL_CHAR,
	F030S_IDX_F035_VAL_VAL,

    F030S_IDX_F033_VAL_CHAR,
	F030S_IDX_F033_VAL_VAL,
    
	F030S_IDX_F032_VAL_CHAR,
	F030S_IDX_F032_VAL_VAL,
	F030S_IDX_F032_VAL_NTF_CFG,

	F030S_IDX_F034_VAL_CHAR,
	F030S_IDX_F034_VAL_VAL,
	F030S_IDX_F034_VAL_NTF_CFG,

	F030S_IDX_NB,
};

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */


/// F030 'Profile' Server environment variable
struct f030s_env_tag
{
    /// profile environment
    prf_env_t prf_env;
   
    /// On-going operation
    struct ke_msg * operation;   
    /// List of values set by application
    struct co_list values;
    /// FFF0 Services Start Handle
    uint16_t start_hdl;
    /// value of the F03x
    uint8_t f031_val[F030_F031_DATA_LEN];
    uint8_t f035_val[F030_F035_DATA_LEN];
    uint8_t f033_val[F030_F033_DATA_LEN];
    uint8_t f032_val[F030_F032_DATA_LEN];
    uint8_t f034_val[F030_F034_DATA_LEN];
    uint8_t f031_desc[F030_CHAR_DATA_LEN];
    uint8_t f031_desc_len;
    /// BASS task state
    ke_state_t state[F030S_IDX_MAX];
    /// Notification configuration of peer devices.
    uint16_t ntf_cfg[BLE_CONNECTION_MAX];
    /// Notification configuration of peer devices.
    uint16_t ind_cfg[BLE_CONNECTION_MAX];
    /// Database features
    uint8_t features;

};



/**
 ****************************************************************************************
 * @brief Retrieve f030 service profile interface
 *
 * @return f030 service profile interface
 ****************************************************************************************
 */
const struct prf_task_cbs* f030s_prf_itf_get(void);

uint16_t f030s_get_att_handle(uint8_t att_idx);

uint8_t  f030s_get_att_idx(uint16_t handle, uint8_t *att_idx);

void f030s_notify_f032_val(uint8_t conidx,struct f030s_env_tag* f030s_env, struct f030s_f0325_val_upd_req const *param);

void f030s_notify_f034_val(uint8_t conidx,struct f030s_env_tag* f030s_env, struct f030s_f0325_val_upd_req const *param);
void f030s_task_init(struct ke_task_desc *task_desc);

#endif /* #if (BLE_F030_SERVERs) */



#endif /*  _F030_H_ */



