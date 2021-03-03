/**
 ****************************************************************************************
 *
 * @file f010s.c
 *
 * @brief f010 Server Implementation.
 *
 * Copyright (C) beken 2019-2022
 *
 *
 ****************************************************************************************
 */

#include "rwip_config.h"

#if (BLE_F010_SERVER)
#include "attm.h"
#include "f010s.h"
#include "f010s_task.h"
#include "prf_utils.h"
#include "prf.h"
#include "ke_mem.h"

#include "uart.h"



/*
 * F010 ATTRIBUTES DEFINITION
 ****************************************************************************************
 */
 
/// Full F010 Database Description - Used to add attributes into the database
const struct attm_desc f010_att_db[F010S_IDX_NB] =
{
    // F010 Service Declaration
    [F010S_IDX_SVC]            =   {ATT_DECL_PRIMARY_SERVICE, PERM(RD, ENABLE), 0, 0},

	[F010S_IDX_F011_VAL_CHAR]  =   {ATT_DECL_CHARACTERISTIC, PERM(RD, ENABLE), 0, 0}, 
    //  Characteristic Value
    [F010S_IDX_F011_VAL_VAL]   =   {ATT_USER_SERVER_CHAR_F011,PERM(RD, ENABLE), PERM(RI, ENABLE), F010_CHAR_DATA_LEN *sizeof(uint8_t)},

       	// f011 Level Characteristic Declaration
	[F010S_IDX_F012_VAL_CHAR]  =   {ATT_DECL_CHARACTERISTIC, PERM(RD, ENABLE), 0, 0},
    // f011 Level Characteristic Value
    [F010S_IDX_F012_VAL_VAL]   =   {ATT_USER_SERVER_CHAR_F012, PERM(IND, ENABLE), PERM(RI, ENABLE), F010_CHAR_DATA_LEN * sizeof(uint8_t)},

	// f011 Level Characteristic - Client Characteristic Configuration Descriptor
	[F010S_IDX_F012_VAL_IND_CFG] = {ATT_DESC_CLIENT_CHAR_CFG,  PERM(RD, ENABLE)|PERM(WRITE_REQ, ENABLE), 0, 0},
    
	
};/// Macro used to retrieve permission value from access and rights on attribute.


static uint8_t f010s_init (struct prf_task_env* env, uint16_t* start_hdl, uint16_t app_task, uint8_t sec_lvl,  struct f010s_db_cfg* params)
{
    uint16_t shdl;
    struct f010s_env_tag* f010s_env = NULL;
    // Status
    uint8_t status = GAP_ERR_NO_ERROR;
    
    //-------------------- allocate memory required for the profile  ---------------------
    f010s_env = (struct f010s_env_tag* ) ke_malloc(sizeof(struct f010s_env_tag), KE_MEM_ATT_DB);
    memset(f010s_env, 0 , sizeof(struct f010s_env_tag));

    // Service content flag
    uint16_t cfg_flag = F010S_CFG_FLAG_MANDATORY_MASK;

    // Save database configuration
    f010s_env->features |= (params->features) ;
    
    f010s_env->f011_desc_len = params->f011_desc_len;
    memcpy(f010s_env->f011_desc,params->f011_desc,params->f011_desc_len);
    shdl = *start_hdl;

    //Create F010 in the DB
    //------------------ create the attribute database for the profile -------------------
    status = attm_svc_create_db(&(shdl), ATT_USER_SERVER_F010, (uint8_t *)&cfg_flag,
            F010S_IDX_NB, NULL, env->task, &f010_att_db[0],
            (sec_lvl & (PERM_MASK_SVC_DIS | PERM_MASK_SVC_AUTH | PERM_MASK_SVC_EKS)));

	//Set optional permissions
	
    //-------------------- Update profile task information  ---------------------
    if (status == ATT_ERR_NO_ERROR)
    {

        // allocate  required environment variable
        env->env = (prf_env_t*) f010s_env;
        *start_hdl = shdl;
        f010s_env->start_hdl = *start_hdl;
        f010s_env->prf_env.app_task = app_task
                | (PERM_GET(sec_lvl, SVC_MI) ? PERM(PRF_MI, ENABLE) : PERM(PRF_MI, DISABLE));
        f010s_env->prf_env.prf_task = env->task | PERM(PRF_MI, DISABLE);

        // initialize environment variable
        env->id                     = TASK_ID_F010S;
        env->desc.idx_max           = F010S_IDX_MAX;
        env->desc.state             = f010s_env->state;
        
        f010s_task_init(&(env->desc));
        co_list_init(&(f010s_env->values));



        // service is ready, go into an Idle state
        ke_state_set(env->task, F010S_IDLE);
    }
    else if(f010s_env != NULL)
    {
        ke_free(f010s_env);
    }
     
    return (status);
}


static void f010s_destroy(struct prf_task_env* env)
{
    struct f010s_env_tag* f010s_env = (struct f010s_env_tag*) env->env;

    // clear on-going operation
    if(f010s_env->operation != NULL)
    {
        ke_free(f010s_env->operation);
    }

    // free profile environment variables
    env->env = NULL;
    ke_free(f010s_env);
}

static void f010s_create(struct prf_task_env* env, uint8_t conidx)
{
    struct f010s_env_tag* f010s_env = (struct f010s_env_tag*) env->env;
    ASSERT_ERR(conidx < BLE_CONNECTION_MAX);

    // force notification config to zero when peer device is connected
    f010s_env->ntf_cfg[conidx] = 0;
}


static void f010s_cleanup(struct prf_task_env* env, uint8_t conidx, uint8_t reason)
{
    struct f010s_env_tag* f010s_env = (struct f010s_env_tag*) env->env;

    ASSERT_ERR(conidx < BLE_CONNECTION_MAX);
    // force notification config to zero when peer device is disconnected
    f010s_env->ntf_cfg[conidx] = 0;
}

void f010s_notify_f014_val(uint8_t conidx,struct f010s_env_tag* f010s_env, struct f010s_f0145_val_upd_req const *param)
{
    // Allocate the GATT notification message
    struct gattc_send_evt_cmd *val = KE_MSG_ALLOC_DYN(GATTC_SEND_EVT_CMD,
            KE_BUILD_ID(TASK_GATTC, conidx), prf_src_task_get(&(f010s_env->prf_env),0),
            gattc_send_evt_cmd, sizeof(uint8_t)* (param->length));

    // Fill in the parameter structure
    val->operation = GATTC_NOTIFY;
//    val->handle = f010s_get_att_handle(F010S_IDX_F014_VAL_VAL);

    // pack measured value in database
    val->length = param->length;
	memcpy(&val->value[0],&param->value[0],param->length);
    // send notification to peer device
    ke_msg_send(val);
}


void f010s_indicate_f012_val(uint8_t conidx,struct f010s_env_tag* f010s_env, struct f010s_f0145_val_upd_req const *param)
{
    // Allocate the GATT notification message
    struct gattc_send_evt_cmd *val = KE_MSG_ALLOC_DYN(GATTC_SEND_EVT_CMD,
            KE_BUILD_ID(TASK_GATTC, conidx), prf_src_task_get(&(f010s_env->prf_env),0),
            gattc_send_evt_cmd, sizeof(uint8_t)* (param->length));

    // Fill in the parameter structure
    val->operation = GATTC_INDICATE;
    val->handle = f010s_get_att_handle(F010S_IDX_F012_VAL_VAL);
    // pack measured value in database
    val->length = param->length;
	memcpy(&val->value[0],&param->value[0],param->length);
    // send notification to peer device
    ke_msg_send(val);
}



/// BASS Task interface required by profile manager
const struct prf_task_cbs f010s_itf =
{
        (prf_init_fnct) f010s_init,
        f010s_destroy,
        f010s_create,
        f010s_cleanup,
};


const struct prf_task_cbs* f010s_prf_itf_get(void)
{
   return &f010s_itf;
}


uint16_t f010s_get_att_handle( uint8_t att_idx)
{
		
    struct f010s_env_tag *f010s_env = PRF_ENV_GET(F010S, f010s);
    uint16_t handle = ATT_INVALID_HDL;
   
    handle = f010s_env->start_hdl;

    // increment index according to expected index
    if(att_idx <= F010S_IDX_F012_VAL_IND_CFG)
    {
        handle += att_idx;
    }
	else
    {
        handle = ATT_INVALID_HDL;
    }
    

    return handle;
}

uint8_t f010s_get_att_idx(uint16_t handle, uint8_t *att_idx)
{
    struct f010s_env_tag* f010s_env = PRF_ENV_GET(F010S, f010s);
    uint16_t hdl_cursor1 = f010s_env->start_hdl;	
    uint8_t status = PRF_APP_ERROR;

    // Browse list of services
    // handle must be greater than current index 
    // check if it's a mandatory index
    if(handle <= (hdl_cursor1 + F010S_IDX_F012_VAL_IND_CFG))
    {
        *att_idx = handle -hdl_cursor1;
        status = GAP_ERR_NO_ERROR;
        
    }
    
    return (status);
}


#endif // (BLE_F010_SERVER)


 
