/**
 ****************************************************************************************
 *
 * @file f000s.c
 *
 * @brief f000 Server Implementation.
 *
 * Copyright (C) beken 2019-2022
 *
 *
 ****************************************************************************************
 */

#include "rwip_config.h"

#if (BLE_F000_SERVER)
#include "attm.h"
#include "f000s.h"
#include "f000s_task.h"
#include "prf_utils.h"
#include "prf.h"
#include "ke_mem.h"

#include "uart.h"



/*
 * F000 ATTRIBUTES DEFINITION
 ****************************************************************************************
 */
 
/// Full F000 Database Description - Used to add attributes into the database
const struct attm_desc f000_att_db[F000S_IDX_NB] =
{
    // F000 Service Declaration
    [F000S_IDX_SVC]            =   {ATT_DECL_PRIMARY_SERVICE, PERM(RD, ENABLE), 0, 0},

	[F000S_IDX_F001_VAL_CHAR]  =   {ATT_DECL_CHARACTERISTIC, PERM(RD, ENABLE), 0, 0}, 
    //  Characteristic Value
    [F000S_IDX_F001_VAL_VAL]   =   {ATT_USER_SERVER_CHAR_F001,PERM(RD, ENABLE), PERM(RI, ENABLE), F000_CHAR_DATA_LEN *sizeof(uint8_t)},

    [F000S_IDX_F001_USER_DESC] =   {ATT_DESC_CHAR_USER_DESCRIPTION,PERM(RD, ENABLE), PERM(RI, ENABLE), F000_CHAR_DATA_LEN *sizeof(uint8_t)},

    
	// f002 value Characteristic Declaration
	[F000S_IDX_F002_VAL_CHAR]  =   {ATT_DECL_CHARACTERISTIC, PERM(RD, ENABLE), 0, 0},
    // f001 Level Characteristic Value
    [F000S_IDX_F002_VAL_VAL]   =   {ATT_USER_SERVER_CHAR_F002, PERM(WRITE_COMMAND, ENABLE), PERM(RI, ENABLE), F000_CHAR_DATA_LEN * sizeof(uint8_t)},

    	// f002 value Characteristic Declaration
	[F000S_IDX_F003_VAL_CHAR]  =   {ATT_DECL_CHARACTERISTIC, PERM(RD, ENABLE), 0, 0},
    // f001 Level Characteristic Value
    [F000S_IDX_F003_VAL_VAL]   =   {ATT_USER_SERVER_CHAR_F003, PERM(WRITE_REQ, ENABLE), PERM(RI, ENABLE), F000_CHAR_DATA_LEN * sizeof(uint8_t)},
	
	// f001 Level Characteristic Declaration
	[F000S_IDX_F004_VAL_CHAR]  =   {ATT_DECL_CHARACTERISTIC, PERM(RD, ENABLE), 0, 0},
    // f001 Level Characteristic Value
    [F000S_IDX_F004_VAL_VAL]   =   {ATT_USER_SERVER_CHAR_F004, PERM(WRITE_COMMAND, ENABLE), PERM(RI, ENABLE), F000_CHAR_DATA_LEN * sizeof(uint8_t)},

	// f001 Level Characteristic - Client Characteristic Configuration Descriptor
	[F000S_IDX_F004_VAL_NTF_CFG] = {ATT_DESC_CLIENT_CHAR_CFG,  PERM(RD, ENABLE)|PERM(WRITE_REQ, ENABLE), 0, 0},
    
    	// f001 Level Characteristic Declaration
	[F000S_IDX_F005_VAL_CHAR]  =   {ATT_DECL_CHARACTERISTIC, PERM(RD, ENABLE), 0, 0},
    // f001 Level Characteristic Value
    [F000S_IDX_F005_VAL_VAL]   =   {ATT_USER_SERVER_CHAR_F005, PERM(WRITE_COMMAND, ENABLE), PERM(RI, ENABLE), F000_CHAR_DATA_LEN * sizeof(uint8_t)},

	// f001 Level Characteristic - Client Characteristic Configuration Descriptor
	[F000S_IDX_F005_VAL_IND_CFG] = {ATT_DESC_CLIENT_CHAR_CFG,  PERM(RD, ENABLE)|PERM(WRITE_REQ, ENABLE), 0, 0},
    
	
};/// Macro used to retrieve permission value from access and rights on attribute.


static uint8_t f000s_init (struct prf_task_env* env, uint16_t* start_hdl, uint16_t app_task, uint8_t sec_lvl,  struct f000s_db_cfg* params)
{
    uint16_t shdl;
    struct f000s_env_tag* f000s_env = NULL;
    // Status
    uint8_t status = GAP_ERR_NO_ERROR;
    
    //-------------------- allocate memory required for the profile  ---------------------
    f000s_env = (struct f000s_env_tag* ) ke_malloc(sizeof(struct f000s_env_tag), KE_MEM_ATT_DB);
    memset(f000s_env, 0 , sizeof(struct f000s_env_tag));

    // Service content flag
    uint16_t cfg_flag = F000S_CFG_FLAG_MANDATORY_MASK;

    // Save database configuration
    f000s_env->features |= (params->features) ;
    
    f000s_env->f001_desc_len = params->f001_desc_len;
    memcpy(f000s_env->f001_desc,params->f001_desc,params->f001_desc_len);
    shdl = *start_hdl;

    //Create F000 in the DB
    //------------------ create the attribute database for the profile -------------------
    status = attm_svc_create_db(&(shdl), ATT_USER_SERVER_F000, (uint8_t *)&cfg_flag,
            F000S_IDX_NB, NULL, env->task, &f000_att_db[0],
            (sec_lvl & (PERM_MASK_SVC_DIS | PERM_MASK_SVC_AUTH | PERM_MASK_SVC_EKS)));

	//Set optional permissions
    if (status == GAP_ERR_NO_ERROR)
    {
        //Set optional permissions
       // if((params->features & 0x01) == F000_F004_VAL_NTF_SUP)
        {
            // Battery Level characteristic value permissions
            uint16_t perm = PERM(NTF, ENABLE);//PERM(RD, ENABLE) | 

            attm_att_set_permission(shdl + F000S_IDX_F004_VAL_VAL, perm, 0);
        }
		//if((params->features & 0x02) == F000_F003_LVL_NTF_SUP)
		{
			uint16_t perm = PERM(IND, ENABLE);//PERM(RD, ENABLE) | 

            attm_att_set_permission(shdl + F000S_IDX_F005_VAL_VAL, perm, 0);
		}
    }
	
    //-------------------- Update profile task information  ---------------------
    if (status == ATT_ERR_NO_ERROR)
    {

        // allocate  required environment variable
        env->env = (prf_env_t*) f000s_env;
        *start_hdl = shdl;
        f000s_env->start_hdl = *start_hdl;
        f000s_env->prf_env.app_task = app_task
                | (PERM_GET(sec_lvl, SVC_MI) ? PERM(PRF_MI, ENABLE) : PERM(PRF_MI, DISABLE));
        f000s_env->prf_env.prf_task = env->task | PERM(PRF_MI, DISABLE);

        // initialize environment variable
        env->id                     = TASK_ID_F000S;
        env->desc.idx_max           = F000S_IDX_MAX;
        env->desc.state             = f000s_env->state;
        
        f000s_task_init(&(env->desc));
        co_list_init(&(f000s_env->values));



        // service is ready, go into an Idle state
        ke_state_set(env->task, F000S_IDLE);
    }
    else if(f000s_env != NULL)
    {
        ke_free(f000s_env);
    }
     
    return (status);
}


static void f000s_destroy(struct prf_task_env* env)
{
    struct f000s_env_tag* f000s_env = (struct f000s_env_tag*) env->env;

    // clear on-going operation
    if(f000s_env->operation != NULL)
    {
        ke_free(f000s_env->operation);
    }

    // free profile environment variables
    env->env = NULL;
    ke_free(f000s_env);
}

static void f000s_create(struct prf_task_env* env, uint8_t conidx)
{
    struct f000s_env_tag* f000s_env = (struct f000s_env_tag*) env->env;
    ASSERT_ERR(conidx < BLE_CONNECTION_MAX);

    // force notification config to zero when peer device is connected
    f000s_env->ntf_cfg[conidx] = 0;
}


static void f000s_cleanup(struct prf_task_env* env, uint8_t conidx, uint8_t reason)
{
    struct f000s_env_tag* f000s_env = (struct f000s_env_tag*) env->env;

    ASSERT_ERR(conidx < BLE_CONNECTION_MAX);
    // force notification config to zero when peer device is disconnected
    f000s_env->ntf_cfg[conidx] = 0;
}

void f000s_notify_f004_val(uint8_t conidx,struct f000s_env_tag* f000s_env, struct f000s_f0045_val_upd_req const *param)
{
    // Allocate the GATT notification message
    struct gattc_send_evt_cmd *val = KE_MSG_ALLOC_DYN(GATTC_SEND_EVT_CMD,
            KE_BUILD_ID(TASK_GATTC, conidx), prf_src_task_get(&(f000s_env->prf_env),0),
            gattc_send_evt_cmd, sizeof(uint8_t)* (param->length));

    // Fill in the parameter structure
    val->operation = GATTC_NOTIFY;
    val->handle = f000s_get_att_handle(F000S_IDX_F004_VAL_VAL);

    // pack measured value in database
    val->length = param->length;
	memcpy(&val->value[0],&param->value[0],param->length);
    // send notification to peer device
    ke_msg_send(val);
}


void f000s_indicate_f005_val(uint8_t conidx,struct f000s_env_tag* f000s_env, struct f000s_f0045_val_upd_req const *param)
{
    // Allocate the GATT notification message
    struct gattc_send_evt_cmd *val = KE_MSG_ALLOC_DYN(GATTC_SEND_EVT_CMD,
            KE_BUILD_ID(TASK_GATTC, conidx), prf_src_task_get(&(f000s_env->prf_env),0),
            gattc_send_evt_cmd, sizeof(uint8_t)* (param->length));

    // Fill in the parameter structure
    val->operation = GATTC_INDICATE;
    val->handle = f000s_get_att_handle(F000S_IDX_F005_VAL_VAL);
    // pack measured value in database
    val->length = param->length;
	memcpy(&val->value[0],&param->value[0],param->length);
    // send notification to peer device
    ke_msg_send(val);
}



/// BASS Task interface required by profile manager
const struct prf_task_cbs f000s_itf =
{
        (prf_init_fnct) f000s_init,
        f000s_destroy,
        f000s_create,
        f000s_cleanup,
};


const struct prf_task_cbs* f000s_prf_itf_get(void)
{
   return &f000s_itf;
}


uint16_t f000s_get_att_handle( uint8_t att_idx)
{
		
    struct f000s_env_tag *f000s_env = PRF_ENV_GET(F000S, f000s);
    uint16_t handle = ATT_INVALID_HDL;
   
    handle = f000s_env->start_hdl;

    // increment index according to expected index
    if(att_idx <= F000S_IDX_F005_VAL_IND_CFG)
    {
        handle += att_idx;
    }
	else
    {
        handle = ATT_INVALID_HDL;
    }
    

    return handle;
}

uint8_t f000s_get_att_idx(uint16_t handle, uint8_t *att_idx)
{
    struct f000s_env_tag* f000s_env = PRF_ENV_GET(F000S, f000s);
    uint16_t hdl_cursor1 = f000s_env->start_hdl;	
    uint8_t status = PRF_APP_ERROR;

    // Browse list of services
    // handle must be greater than current index 
    // check if it's a mandatory index
    if(handle <= (hdl_cursor1 + F000S_IDX_F005_VAL_IND_CFG))
    {
        *att_idx = handle -hdl_cursor1;
        status = GAP_ERR_NO_ERROR;
        
    }
    
    return (status);
}


#endif // (BLE_F000_SERVER)


 
