/**
 ****************************************************************************************
 *
 * @file f010s.h
 *
 * @brief Header file - F010 Service Server Role
 *
 * Copyright (C) beken 2019-2022
 *
 *
 ****************************************************************************************
 */
#ifndef _F010S_H_
#define _F010S_H_

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

#if (BLE_F010_SERVER)

#include "f010s_task.h"
#include "atts.h"
#include "prf_types.h"
#include "prf.h"

/*
 * DEFINES
 ****************************************************************************************
 */

#define F010S_CFG_FLAG_MANDATORY_MASK       (0x3F)

enum
{		
	ATT_USER_SERVER_F010 						= ATT_UUID_16(0x181B),	// service 
	ATT_USER_SERVER_CHAR_F011                   = ATT_UUID_16(0x2a9b), // read		
    ATT_USER_SERVER_CHAR_F012					= ATT_UUID_16(0x2a9c), // ind		
};

/// F010S Service Attributes Indexes
enum
{
	F010S_IDX_SVC,

    F010S_IDX_F011_VAL_CHAR,
	F010S_IDX_F011_VAL_VAL,

	F010S_IDX_F012_VAL_CHAR,
	F010S_IDX_F012_VAL_VAL,
	F010S_IDX_F012_VAL_IND_CFG,

	F010S_IDX_NB,
};

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */


/// F010 'Profile' Server environment variable
struct f010s_env_tag
{
    /// profile environment
    prf_env_t prf_env;
   
    /// On-going operation
    struct ke_msg * operation;   
    /// List of values set by application
    struct co_list values;
    /// FFF0 Services Start Handle
    uint16_t start_hdl;
    /// value of the F01x
    uint8_t f011_val[F010_F011_DATA_LEN];
    uint8_t f017_val[F010_CHAR_DATA_LEN];
    uint8_t f013_val[F010_CHAR_DATA_LEN];
    uint8_t f014_val[F010_CHAR_DATA_LEN];
    uint8_t f012_val[F010_F012_DATA_LEN];
    uint8_t f011_desc[F010_CHAR_DATA_LEN];
    uint8_t f011_desc_len;
    /// BASS task state
    ke_state_t state[F010S_IDX_MAX];
    /// Notification configuration of peer devices.
    uint16_t ntf_cfg[BLE_CONNECTION_MAX];
    /// Notification configuration of peer devices.
    uint16_t ind_cfg[BLE_CONNECTION_MAX];
    /// Database features
    uint8_t features;

};



/**
 ****************************************************************************************
 * @brief Retrieve f010 service profile interface
 *
 * @return f010 service profile interface
 ****************************************************************************************
 */
const struct prf_task_cbs* f010s_prf_itf_get(void);

uint16_t f010s_get_att_handle(uint8_t att_idx);

uint8_t  f010s_get_att_idx(uint16_t handle, uint8_t *att_idx);

void f010s_notify_f014_val(uint8_t conidx,struct f010s_env_tag* f010s_env, struct f010s_f0145_val_upd_req const *param);

void f010s_indicate_f012_val(uint8_t conidx,struct f010s_env_tag* f010s_env, struct f010s_f0145_val_upd_req const *param);
void f010s_task_init(struct ke_task_desc *task_desc);

#endif /* #if (BLE_F010_SERVERs) */



#endif /*  _F010_H_ */



