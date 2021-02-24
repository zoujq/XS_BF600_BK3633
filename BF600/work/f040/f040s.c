/**
 ****************************************************************************************
 *
 * @file f040s.c
 *
 * @brief f040 Server Implementation.
 *
 * Copyright (C) beken 2019-2022
 *
 *
 ****************************************************************************************
 */

#include "rwip_config.h"

#if (BLE_F040_SERVER)
#include "attm.h"
#include "f040s.h"
#include "f040s_task.h"
#include "prf_utils.h"
#include "prf.h"
#include "ke_mem.h"

#include "uart.h"



/*
 * F040 ATTRIBUTES DEFINITION
 ****************************************************************************************
 */
 
/// Full F040 Database Description - Used to add attributes into the database
const struct attm_desc f040_att_db[F040S_IDX_NB] =
{
    // F040 Service Declaration
    [F040S_IDX_SVC]            =   {ATT_DECL_PRIMARY_SERVICE, PERM(RD, ENABLE), 0, 0},

	// [F040S_IDX_F041_VAL_CHAR]  =   {ATT_DECL_CHARACTERISTIC, PERM(RD, ENABLE), 0, 0}, 
 //    //  Characteristic Value
 //    [F040S_IDX_F041_VAL_VAL]   =   {ATT_USER_SERVER_CHAR_F041,PERM(RD, ENABLE), PERM(RI, ENABLE), F040_CHAR_DATA_LEN *sizeof(uint8_t)},

 //    [F040S_IDX_F041_USER_DESC] =   {ATT_DESC_CHAR_USER_DESCRIPTION,PERM(RD, ENABLE), PERM(RI, ENABLE), F040_CHAR_DATA_LEN *sizeof(uint8_t)},

    
	// // f042 value Characteristic Declaration
	// [F040S_IDX_F042_VAL_CHAR]  =   {ATT_DECL_CHARACTERISTIC, PERM(RD, ENABLE), 0, 0},
 //    // f041 Level Characteristic Value
 //    [F040S_IDX_F042_VAL_VAL]   =   {ATT_USER_SERVER_CHAR_F042, PERM(WRITE_COMMAND, ENABLE), PERM(RI, ENABLE), F040_CHAR_DATA_LEN * sizeof(uint8_t)},

 //    	// f042 value Characteristic Declaration
	// [F040S_IDX_F043_VAL_CHAR]  =   {ATT_DECL_CHARACTERISTIC, PERM(RD, ENABLE), 0, 0},
 //    // f041 Level Characteristic Value
 //    [F040S_IDX_F043_VAL_VAL]   =   {ATT_USER_SERVER_CHAR_F043, PERM(WRITE_REQ, ENABLE), PERM(RI, ENABLE), F040_CHAR_DATA_LEN * sizeof(uint8_t)},
	
	// f041 Level Characteristic Declaration
	[F040S_IDX_F044_VAL_CHAR]  =   {ATT_DECL_CHARACTERISTIC, PERM(RD, ENABLE), 0, 0},
    // f041 Level Characteristic Value
    [F040S_IDX_F044_VAL_VAL]   =   {ATT_USER_SERVER_CHAR_F044, PERM(NTF, ENABLE)|PERM(RD, ENABLE)|PERM(WRITE_REQ, ENABLE), PERM(RI, ENABLE), F040_CHAR_DATA_LEN * sizeof(uint8_t)},

	// f041 Level Characteristic - Client Characteristic Configuration Descriptor
	[F040S_IDX_F044_VAL_NTF_CFG] = {ATT_DESC_CLIENT_CHAR_CFG,  PERM(RD, ENABLE)|PERM(WRITE_REQ, ENABLE), 0, 0},
    
 //    	// f041 Level Characteristic Declaration
	// [F040S_IDX_F045_VAL_CHAR]  =   {ATT_DECL_CHARACTERISTIC, PERM(RD, ENABLE), 0, 0},
 //    // f041 Level Characteristic Value
 //    [F040S_IDX_F045_VAL_VAL]   =   {ATT_USER_SERVER_CHAR_F045, PERM(WRITE_COMMAND, ENABLE), PERM(RI, ENABLE), F040_CHAR_DATA_LEN * sizeof(uint8_t)},

	// // f041 Level Characteristic - Client Characteristic Configuration Descriptor
	// [F040S_IDX_F045_VAL_IND_CFG] = {ATT_DESC_CLIENT_CHAR_CFG,  PERM(RD, ENABLE)|PERM(WRITE_REQ, ENABLE), 0, 0},
    
	
};/// Macro used to retrieve permission value from access and rights on attribute.


static uint8_t f040s_init (struct prf_task_env* env, uint16_t* start_hdl, uint16_t app_task, uint8_t sec_lvl,  struct f040s_db_cfg* params)
{
    uint16_t shdl;
    struct f040s_env_tag* f040s_env = NULL;
    // Status
    uint8_t status = GAP_ERR_NO_ERROR;
    
    //-------------------- allocate memory required for the profile  ---------------------
    f040s_env = (struct f040s_env_tag* ) ke_malloc(sizeof(struct f040s_env_tag), KE_MEM_ATT_DB);
    memset(f040s_env, 0 , sizeof(struct f040s_env_tag));

    // Service content flag
    uint16_t cfg_flag = F040S_CFG_FLAG_MANDATORY_MASK;

    // Save database configuration
    f040s_env->features |= (params->features) ;
    
    f040s_env->f041_desc_len = params->f041_desc_len;
    memcpy(f040s_env->f041_desc,params->f041_desc,params->f041_desc_len);
    shdl = *start_hdl;

    //Create F040 in the DB
    //------------------ create the attribute database for the profile -------------------
    status = attm_svc_create_db(&(shdl), ATT_USER_SERVER_F040, (uint8_t *)&cfg_flag,
            F040S_IDX_NB, NULL, env->task, &f040_att_db[0],
            (sec_lvl & (PERM_MASK_SVC_DIS | PERM_MASK_SVC_AUTH | PERM_MASK_SVC_EKS)));

	//Set optional permissions
  //   if (status == GAP_ERR_NO_ERROR)
  //   {
  //       //Set optional permissions
  //      // if((params->features & 0x01) == F040_F044_VAL_NTF_SUP)
  //       {
  //           // Battery Level characteristic value permissions
  //           uint16_t perm = PERM(NTF, ENABLE);//PERM(RD, ENABLE) | 

  //           attm_att_set_permission(shdl + F040S_IDX_F044_VAL_VAL, perm, 0);
  //       }
		// //if((params->features & 0x02) == F040_F043_LVL_NTF_SUP)
		// {
		// 	uint16_t perm = PERM(IND, ENABLE);//PERM(RD, ENABLE) | 

  //           attm_att_set_permission(shdl + F040S_IDX_F045_VAL_VAL, perm, 0);
		// }
  //   }
	
    //-------------------- Update profile task information  ---------------------
    if (status == ATT_ERR_NO_ERROR)
    {

        // allocate  required environment variable
        env->env = (prf_env_t*) f040s_env;
        *start_hdl = shdl;
        f040s_env->start_hdl = *start_hdl;
        f040s_env->prf_env.app_task = app_task
                | (PERM_GET(sec_lvl, SVC_MI) ? PERM(PRF_MI, ENABLE) : PERM(PRF_MI, DISABLE));
        f040s_env->prf_env.prf_task = env->task | PERM(PRF_MI, DISABLE);

        // initialize environment variable
        env->id                     = TASK_ID_F040S;
        env->desc.idx_max           = F040S_IDX_MAX;
        env->desc.state             = f040s_env->state;
        
        f040s_task_init(&(env->desc));
        co_list_init(&(f040s_env->values));



        // service is ready, go into an Idle state
        ke_state_set(env->task, F040S_IDLE);
    }
    else if(f040s_env != NULL)
    {
        ke_free(f040s_env);
    }
     
    return (status);
}


static void f040s_destroy(struct prf_task_env* env)
{
    struct f040s_env_tag* f040s_env = (struct f040s_env_tag*) env->env;

    // clear on-going operation
    if(f040s_env->operation != NULL)
    {
        ke_free(f040s_env->operation);
    }

    // free profile environment variables
    env->env = NULL;
    ke_free(f040s_env);
}

static void f040s_create(struct prf_task_env* env, uint8_t conidx)
{
    struct f040s_env_tag* f040s_env = (struct f040s_env_tag*) env->env;
    ASSERT_ERR(conidx < BLE_CONNECTION_MAX);

    // force notification config to zero when peer device is connected
    f040s_env->ntf_cfg[conidx] = 0;
}


static void f040s_cleanup(struct prf_task_env* env, uint8_t conidx, uint8_t reason)
{
    struct f040s_env_tag* f040s_env = (struct f040s_env_tag*) env->env;

    ASSERT_ERR(conidx < BLE_CONNECTION_MAX);
    // force notification config to zero when peer device is disconnected
    f040s_env->ntf_cfg[conidx] = 0;
}

void f040s_notify_f044_val(uint8_t conidx,struct f040s_env_tag* f040s_env, struct f040s_f0445_val_upd_req const *param)
{
    // Allocate the GATT notification message
    struct gattc_send_evt_cmd *val = KE_MSG_ALLOC_DYN(GATTC_SEND_EVT_CMD,
            KE_BUILD_ID(TASK_GATTC, conidx), prf_src_task_get(&(f040s_env->prf_env),0),
            gattc_send_evt_cmd, sizeof(uint8_t)* (param->length));

    // Fill in the parameter structure
    val->operation = GATTC_NOTIFY;
    val->handle = f040s_get_att_handle(F040S_IDX_F044_VAL_VAL);

    // pack measured value in database
    val->length = param->length;
	memcpy(&val->value[0],&param->value[0],param->length);
    // send notification to peer device
    ke_msg_send(val);
}


void f040s_indicate_f045_val(uint8_t conidx,struct f040s_env_tag* f040s_env, struct f040s_f0445_val_upd_req const *param)
{
    // Allocate the GATT notification message
    struct gattc_send_evt_cmd *val = KE_MSG_ALLOC_DYN(GATTC_SEND_EVT_CMD,
            KE_BUILD_ID(TASK_GATTC, conidx), prf_src_task_get(&(f040s_env->prf_env),0),
            gattc_send_evt_cmd, sizeof(uint8_t)* (param->length));

    // Fill in the parameter structure
    val->operation = GATTC_INDICATE;
//    val->handle = f040s_get_att_handle(F040S_IDX_F045_VAL_VAL);
    // pack measured value in database
    val->length = param->length;
	memcpy(&val->value[0],&param->value[0],param->length);
    // send notification to peer device
    ke_msg_send(val);
}



/// BASS Task interface required by profile manager
const struct prf_task_cbs f040s_itf =
{
        (prf_init_fnct) f040s_init,
        f040s_destroy,
        f040s_create,
        f040s_cleanup,
};


const struct prf_task_cbs* f040s_prf_itf_get(void)
{
   return &f040s_itf;
}


uint16_t f040s_get_att_handle( uint8_t att_idx)
{
		
    struct f040s_env_tag *f040s_env = PRF_ENV_GET(F040S, f040s);
    uint16_t handle = ATT_INVALID_HDL;
   
    handle = f040s_env->start_hdl;

    // increment index according to expected index
//    if(att_idx <= F040S_IDX_F045_VAL_IND_CFG)
//    {
//        handle += att_idx;
//    }
//		else
//    {
//        handle = ATT_INVALID_HDL;
//    }
//    

    return handle;
}

uint8_t f040s_get_att_idx(uint16_t handle, uint8_t *att_idx)
{
    struct f040s_env_tag* f040s_env = PRF_ENV_GET(F040S, f040s);
    uint16_t hdl_cursor1 = f040s_env->start_hdl;	
    uint8_t status = PRF_APP_ERROR;

    // Browse list of services
    // handle must be greater than current index 
    // check if it's a mandatory index
//    if(handle <= (hdl_cursor1 + F040S_IDX_F045_VAL_IND_CFG))
//    {
//        *att_idx = handle -hdl_cursor1;
//        status = GAP_ERR_NO_ERROR;
//        
//    }
    
    return (status);
}


#endif // (BLE_F040_SERVER)


 
