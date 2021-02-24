/**
 ****************************************************************************************
 *
 * @file f020s.c
 *
 * @brief f020 Server Implementation.
 *
 * Copyright (C) beken 2019-2022
 *
 *
 ****************************************************************************************
 */

#include "rwip_config.h"

#if (BLE_F020_SERVER)
#include "attm.h"
#include "f020s.h"
#include "f020s_task.h"
#include "prf_utils.h"
#include "prf.h"
#include "ke_mem.h"

#include "uart.h"



/*
 * F020 ATTRIBUTES DEFINITION
 ****************************************************************************************
 */
 
/// Full F020 Database Description - Used to add attributes into the database
const struct attm_desc f020_att_db[F020S_IDX_NB] =
{
    // F020 Service Declaration
    [F020S_IDX_SVC]            =   {ATT_DECL_PRIMARY_SERVICE, PERM(RD, ENABLE), 0, 0},

	[F020S_IDX_F021_VAL_CHAR]  =   {ATT_DECL_CHARACTERISTIC, PERM(RD, ENABLE), 0, 0}, 
    //  Characteristic Value
    [F020S_IDX_F021_VAL_VAL]   =   {ATT_USER_SERVER_CHAR_F021,PERM(RD, ENABLE), PERM(RI, ENABLE), F020_CHAR_DATA_LEN *sizeof(uint8_t)},
    
	// f022 value Characteristic Declaration
	[F020S_IDX_F022_VAL_CHAR]  =   {ATT_DECL_CHARACTERISTIC, PERM(RD, ENABLE), 0, 0},
    // f021 Level Characteristic Value
    [F020S_IDX_F022_VAL_VAL]   =   {ATT_USER_SERVER_CHAR_F022, PERM(WRITE_COMMAND, ENABLE), PERM(RI, ENABLE), F020_CHAR_DATA_LEN * sizeof(uint8_t)},

    	// f022 value Characteristic Declaration
	[F020S_IDX_F023_VAL_CHAR]  =   {ATT_DECL_CHARACTERISTIC, PERM(RD, ENABLE), 0, 0},
    // f021 Level Characteristic Value
    [F020S_IDX_F023_VAL_VAL]   =   {ATT_USER_SERVER_CHAR_F023, PERM(WRITE_REQ, ENABLE), PERM(RI, ENABLE), F020_CHAR_DATA_LEN * sizeof(uint8_t)},
	
	// f021 Level Characteristic Declaration
	[F020S_IDX_F024_VAL_CHAR]  =   {ATT_DECL_CHARACTERISTIC, PERM(RD, ENABLE), 0, 0},
    // f021 Level Characteristic Value
    [F020S_IDX_F024_VAL_VAL]   =   {ATT_USER_SERVER_CHAR_F024, PERM(WRITE_COMMAND, ENABLE), PERM(RI, ENABLE), F020_CHAR_DATA_LEN * sizeof(uint8_t)},

	// f021 Level Characteristic - Client Characteristic Configuration Descriptor
	[F020S_IDX_F024_VAL_NTF_CFG] = {ATT_DESC_CLIENT_CHAR_CFG,  PERM(RD, ENABLE)|PERM(WRITE_REQ, ENABLE), 0, 0},
    
    // f022 value Characteristic Declaration
    [F020S_IDX_F025_VAL_CHAR]  =   {ATT_DECL_CHARACTERISTIC, PERM(RD, ENABLE), 0, 0},
    // f021 Level Characteristic Value
    [F020S_IDX_F025_VAL_VAL]   =   {ATT_USER_SERVER_CHAR_F025, PERM(WRITE_COMMAND, ENABLE), PERM(RI, ENABLE), F020_CHAR_DATA_LEN * sizeof(uint8_t)},

    	// f021 Level Characteristic Declaration
	[F020S_IDX_F026_VAL_CHAR]  =   {ATT_DECL_CHARACTERISTIC, PERM(RD, ENABLE), 0, 0},
    // f021 Level Characteristic Value
    [F020S_IDX_F026_VAL_VAL]   =   {ATT_USER_SERVER_CHAR_F026, PERM(WRITE_COMMAND, ENABLE), PERM(RI, ENABLE), F020_CHAR_DATA_LEN * sizeof(uint8_t)},

	// f021 Level Characteristic - Client Characteristic Configuration Descriptor
	[F020S_IDX_F026_VAL_IND_CFG] = {ATT_DESC_CLIENT_CHAR_CFG,  PERM(RD, ENABLE)|PERM(WRITE_REQ, ENABLE), 0, 0},
    
	
};/// Macro used to retrieve permission value from access and rights on attribute.


static uint8_t f020s_init (struct prf_task_env* env, uint16_t* start_hdl, uint16_t app_task, uint8_t sec_lvl,  struct f020s_db_cfg* params)
{
    uint16_t shdl;
    struct f020s_env_tag* f020s_env = NULL;
    // Status
    uint8_t status = GAP_ERR_NO_ERROR;
    
    //-------------------- allocate memory required for the profile  ---------------------
    f020s_env = (struct f020s_env_tag* ) ke_malloc(sizeof(struct f020s_env_tag), KE_MEM_ATT_DB);
    memset(f020s_env, 0 , sizeof(struct f020s_env_tag));

    // Service content flag
    uint16_t cfg_flag = F020S_CFG_FLAG_MANDATORY_MASK;

    // Save database configuration
    f020s_env->features |= (params->features) ;
    
    f020s_env->f021_desc_len = params->f021_desc_len;
    memcpy(f020s_env->f021_desc,params->f021_desc,params->f021_desc_len);
    shdl = *start_hdl;

    //Create F020 in the DB
    //------------------ create the attribute database for the profile -------------------
    status = attm_svc_create_db(&(shdl), ATT_USER_SERVER_F020, (uint8_t *)&cfg_flag,
            F020S_IDX_NB, NULL, env->task, &f020_att_db[0],
            (sec_lvl & (PERM_MASK_SVC_DIS | PERM_MASK_SVC_AUTH | PERM_MASK_SVC_EKS)));

	//Set optional permissions
    if (status == GAP_ERR_NO_ERROR)
    {
        //Set optional permissions
       // if((params->features & 0x01) == F020_F024_VAL_NTF_SUP)
        {
            // Battery Level characteristic value permissions
            uint16_t perm = PERM(NTF, ENABLE);//PERM(RD, ENABLE) | 

            attm_att_set_permission(shdl + F020S_IDX_F024_VAL_VAL, perm, 0);
        }
		//if((params->features & 0x02) == F020_F023_LVL_NTF_SUP)
		{
			uint16_t perm = PERM(IND, ENABLE);//PERM(RD, ENABLE) | 

            attm_att_set_permission(shdl + F020S_IDX_F026_VAL_VAL, perm, 0);
		}
    }
	
    //-------------------- Update profile task information  ---------------------
    if (status == ATT_ERR_NO_ERROR)
    {

        // allocate  required environment variable
        env->env = (prf_env_t*) f020s_env;
        *start_hdl = shdl;
        f020s_env->start_hdl = *start_hdl;
        f020s_env->prf_env.app_task = app_task
                | (PERM_GET(sec_lvl, SVC_MI) ? PERM(PRF_MI, ENABLE) : PERM(PRF_MI, DISABLE));
        f020s_env->prf_env.prf_task = env->task | PERM(PRF_MI, DISABLE);

        // initialize environment variable
        env->id                     = TASK_ID_F020S;
        env->desc.idx_max           = F020S_IDX_MAX;
        env->desc.state             = f020s_env->state;
        
        f020s_task_init(&(env->desc));
        co_list_init(&(f020s_env->values));



        // service is ready, go into an Idle state
        ke_state_set(env->task, F020S_IDLE);
    }
    else if(f020s_env != NULL)
    {
        ke_free(f020s_env);
    }
     
    return (status);
}


static void f020s_destroy(struct prf_task_env* env)
{
    struct f020s_env_tag* f020s_env = (struct f020s_env_tag*) env->env;

    // clear on-going operation
    if(f020s_env->operation != NULL)
    {
        ke_free(f020s_env->operation);
    }

    // free profile environment variables
    env->env = NULL;
    ke_free(f020s_env);
}

static void f020s_create(struct prf_task_env* env, uint8_t conidx)
{
    struct f020s_env_tag* f020s_env = (struct f020s_env_tag*) env->env;
    ASSERT_ERR(conidx < BLE_CONNECTION_MAX);

    // force notification config to zero when peer device is connected
    f020s_env->ntf_cfg[conidx] = 0;
}


static void f020s_cleanup(struct prf_task_env* env, uint8_t conidx, uint8_t reason)
{
    struct f020s_env_tag* f020s_env = (struct f020s_env_tag*) env->env;

    ASSERT_ERR(conidx < BLE_CONNECTION_MAX);
    // force notification config to zero when peer device is disconnected
    f020s_env->ntf_cfg[conidx] = 0;
}

void f020s_notify_f024_val(uint8_t conidx,struct f020s_env_tag* f020s_env, struct f020s_f0245_val_upd_req const *param)
{
    // Allocate the GATT notification message
    struct gattc_send_evt_cmd *val = KE_MSG_ALLOC_DYN(GATTC_SEND_EVT_CMD,
            KE_BUILD_ID(TASK_GATTC, conidx), prf_src_task_get(&(f020s_env->prf_env),0),
            gattc_send_evt_cmd, sizeof(uint8_t)* (param->length));

    // Fill in the parameter structure
    val->operation = GATTC_NOTIFY;
    val->handle = f020s_get_att_handle(F020S_IDX_F024_VAL_VAL);

    // pack measured value in database
    val->length = param->length;
	memcpy(&val->value[0],&param->value[0],param->length);
    // send notification to peer device
    ke_msg_send(val);
}


void f020s_indicate_f026_val(uint8_t conidx,struct f020s_env_tag* f020s_env, struct f020s_f0245_val_upd_req const *param)
{
    // Allocate the GATT notification message
    struct gattc_send_evt_cmd *val = KE_MSG_ALLOC_DYN(GATTC_SEND_EVT_CMD,
            KE_BUILD_ID(TASK_GATTC, conidx), prf_src_task_get(&(f020s_env->prf_env),0),
            gattc_send_evt_cmd, sizeof(uint8_t)* (param->length));

    // Fill in the parameter structure
    val->operation = GATTC_INDICATE;
    val->handle = f020s_get_att_handle(F020S_IDX_F026_VAL_VAL);
    // pack measured value in database
    val->length = param->length;
	memcpy(&val->value[0],&param->value[0],param->length);
    // send notification to peer device
    ke_msg_send(val);
}



/// BASS Task interface required by profile manager
const struct prf_task_cbs f020s_itf =
{
        (prf_init_fnct) f020s_init,
        f020s_destroy,
        f020s_create,
        f020s_cleanup,
};


const struct prf_task_cbs* f020s_prf_itf_get(void)
{
   return &f020s_itf;
}


uint16_t f020s_get_att_handle( uint8_t att_idx)
{
		
    struct f020s_env_tag *f020s_env = PRF_ENV_GET(F020S, f020s);
    uint16_t handle = ATT_INVALID_HDL;
   
    handle = f020s_env->start_hdl;

    // increment index according to expected index
    if(att_idx <= F020S_IDX_F026_VAL_IND_CFG)
    {
        handle += att_idx;
    }
	else
    {
        handle = ATT_INVALID_HDL;
    }
    

    return handle;
}

uint8_t f020s_get_att_idx(uint16_t handle, uint8_t *att_idx)
{
    struct f020s_env_tag* f020s_env = PRF_ENV_GET(F020S, f020s);
    uint16_t hdl_cursor1 = f020s_env->start_hdl;	
    uint8_t status = PRF_APP_ERROR;

    // Browse list of services
    // handle must be greater than current index 
    // check if it's a mandatory index
    if(handle <= (hdl_cursor1 + F020S_IDX_F026_VAL_IND_CFG))
    {
        *att_idx = handle -hdl_cursor1;
        status = GAP_ERR_NO_ERROR;
        
    }
    
    return (status);
}


#endif // (BLE_F020_SERVER)


 
