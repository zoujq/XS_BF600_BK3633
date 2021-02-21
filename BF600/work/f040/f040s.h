/**
 ****************************************************************************************
 *
 * @file f040s.h
 *
 * @brief Header file - F040 Service Server Role
 *
 * Copyright (C) beken 2019-2022
 *
 *
 ****************************************************************************************
 */
#ifndef _F040S_H_
#define _F040S_H_

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

#if (BLE_F040_SERVER)

#include "f040s_task.h"
#include "atts.h"
#include "prf_types.h"
#include "prf.h"

/*
 * DEFINES
 ****************************************************************************************
 */

#define F040S_CFG_FLAG_MANDATORY_MASK       (0x3FFF)

enum
{		
	ATT_USER_SERVER_F040 						= ATT_UUID_16(0xF040),	// service 
	ATT_USER_SERVER_CHAR_F041                   = ATT_UUID_16(0xF041), // read		
	ATT_USER_SERVER_CHAR_F042					= ATT_UUID_16(0xF042),// write cmd
	ATT_USER_SERVER_CHAR_F043					= ATT_UUID_16(0xF043),// write req
    ATT_USER_SERVER_CHAR_F044					= ATT_UUID_16(0xF044), // ntf
    ATT_USER_SERVER_CHAR_F045					= ATT_UUID_16(0xF045), // ind		
};

/// F040S Service Attributes Indexes
enum
{
	F040S_IDX_SVC,

    F040S_IDX_F041_VAL_CHAR,
	F040S_IDX_F041_VAL_VAL,
    F040S_IDX_F041_USER_DESC,
    
	F040S_IDX_F042_VAL_CHAR,
	F040S_IDX_F042_VAL_VAL,

    F040S_IDX_F043_VAL_CHAR,
	F040S_IDX_F043_VAL_VAL,
    
	F040S_IDX_F044_VAL_CHAR,
	F040S_IDX_F044_VAL_VAL,
	F040S_IDX_F044_VAL_NTF_CFG,

	F040S_IDX_F045_VAL_CHAR,
	F040S_IDX_F045_VAL_VAL,
	F040S_IDX_F045_VAL_IND_CFG,

	F040S_IDX_NB,
};

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */


/// F040 'Profile' Server environment variable
struct f040s_env_tag
{
    /// profile environment
    prf_env_t prf_env;
   
    /// On-going operation
    struct ke_msg * operation;   
    /// List of values set by application
    struct co_list values;
    /// FFF0 Services Start Handle
    uint16_t start_hdl;
    /// value of the F04x
    uint8_t f041_val[F040_CHAR_DATA_LEN];
    uint8_t f042_val[F040_CHAR_DATA_LEN];
    uint8_t f043_val[F040_CHAR_DATA_LEN];
    uint8_t f044_val[F040_CHAR_DATA_LEN];
    uint8_t f045_val[F040_CHAR_DATA_LEN];
    uint8_t f041_desc[F040_CHAR_DATA_LEN];
    uint8_t f041_desc_len;
    /// BASS task state
    ke_state_t state[F040S_IDX_MAX];
    /// Notification configuration of peer devices.
    uint16_t ntf_cfg[BLE_CONNECTION_MAX];
    /// Notification configuration of peer devices.
    uint16_t ind_cfg[BLE_CONNECTION_MAX];
    /// Database features
    uint8_t features;

};



/**
 ****************************************************************************************
 * @brief Retrieve f040 service profile interface
 *
 * @return f040 service profile interface
 ****************************************************************************************
 */
const struct prf_task_cbs* f040s_prf_itf_get(void);

uint16_t f040s_get_att_handle(uint8_t att_idx);

uint8_t  f040s_get_att_idx(uint16_t handle, uint8_t *att_idx);

void f040s_notify_f044_val(uint8_t conidx,struct f040s_env_tag* f040s_env, struct f040s_f0445_val_upd_req const *param);

void f040s_indicate_f045_val(uint8_t conidx,struct f040s_env_tag* f040s_env, struct f040s_f0445_val_upd_req const *param);
void f040s_task_init(struct ke_task_desc *task_desc);

#endif /* #if (BLE_F040_SERVERs) */



#endif /*  _F040_H_ */



