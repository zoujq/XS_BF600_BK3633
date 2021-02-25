/**
 ****************************************************************************************
 *
 * @file   f000s_task.c
 *
 * @brief F000 Server Role Task Implementation.
 *
 * Copyright (C) Beken 2009-2015
 *
 *
 ****************************************************************************************
 */


/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"

#if (BLE_F000_SERVER)

#include "gap.h"
#include "gattc_task.h"
#include "attm.h"
#include "atts.h"
#include "co_utils.h"
#include "f000s.h"
#include "f000s_task.h"
#include "prf_utils.h"
#include "user_config.h"




static int f000s_f004_val_upd_req_handler(ke_msg_id_t const msgid,
                                            struct f000s_f0045_val_upd_req const *param,
                                            ke_task_id_t const dest_id,
                                            ke_task_id_t const src_id)
{
    int msg_status = KE_MSG_SAVED;
    uint8_t state = ke_state_get(dest_id);
    uint8_t conidx = KE_IDX_GET(src_id);
	uart_printf("%s,conidx:%d\r\n",__func__,conidx);
    // check state of the task
    if(state == F000S_IDLE)
    {
        struct f000s_env_tag* f000s_env = PRF_ENV_GET(F000S, f000s);

        if(f000s_env->ntf_cfg[conidx] == PRF_CLI_START_NTF)
        {
            // put task in a busy state
            ke_state_set(dest_id, F000S_BUSY);						
            f000s_notify_f004_val(conidx,f000s_env, param);
            ke_state_set(dest_id, F000S_IDLE);
        }
           
		msg_status = KE_MSG_CONSUMED;	
    }

    return (msg_status);
  }

static int f000s_f002_val_upd_req_handler(ke_msg_id_t const msgid,
                                            struct f000s_f0045_val_upd_req const *param,
                                            ke_task_id_t const dest_id,
                                            ke_task_id_t const src_id)
{
    int msg_status = KE_MSG_SAVED;
    uint8_t state = ke_state_get(dest_id);
    uint8_t conidx = KE_IDX_GET(src_id);
	uart_printf("%s,conidx:%d\r\n",__func__,conidx);
    // check state of the task
    if(state == F000S_IDLE)
    {
        struct f000s_env_tag* f000s_env = PRF_ENV_GET(F000S, f000s);

        if(f000s_env->ind_cfg[conidx] == PRF_CLI_START_IND)
        {
            // put task in a busy state
            ke_state_set(dest_id, F000S_BUSY);						
            f000s_indicate_f002_val(conidx,f000s_env, param);
            ke_state_set(dest_id, F000S_IDLE);
        }
        
		msg_status = KE_MSG_CONSUMED;	
    }

    return (msg_status);
  }

  
static int gattc_att_info_req_ind_handler(ke_msg_id_t const msgid,
        struct gattc_att_info_req_ind *param,
        ke_task_id_t const dest_id,
        ke_task_id_t const src_id)
{
    
    struct gattc_att_info_cfm * cfm;
    uint8_t  att_idx = 0;
    uint8_t conidx = KE_IDX_GET(src_id);
    // retrieve handle information
    uint8_t status = f000s_get_att_idx(param->handle, &att_idx);
    uart_printf("%s,conidx:%d\r\n",__func__,conidx);
    //Send write response
    cfm = KE_MSG_ALLOC(GATTC_ATT_INFO_CFM, src_id, dest_id, gattc_att_info_cfm);
    cfm->handle = param->handle;

    if(status == GAP_ERR_NO_ERROR)
    {
        // check if it's a client configuration char
  //       if(att_idx == F000S_IDX_F004_VAL_NTF_CFG)
  //       {
  //           // CCC attribute length = 2
  //           cfm->length = 2;
  //       }
		// else 
        if(att_idx == F000S_IDX_F002_VAL_IND_CFG)
		{
			// CCC attribute length = 2
            cfm->length = 2;
		}
        // else if(att_idx == F000S_IDX_F003_VAL_VAL)
        // {
        //     cfm->length = F000_CHAR_DATA_LEN;
        // }
        // not expected request
        else
        {
            cfm->length = 0;
            status = ATT_ERR_WRITE_NOT_PERMITTED;
        }
    }

    cfm->status = status;
    ke_msg_send(cfm);

    return (KE_MSG_CONSUMED);
}



static int gattc_write_req_ind_handler(ke_msg_id_t const msgid, struct gattc_write_req_ind const *param,
                                      ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    
    struct gattc_write_cfm * cfm;
    uint8_t att_idx = 0;
    uint8_t conidx = KE_IDX_GET(src_id);
    uart_printf("%s,conidx:%d,offset:%d,len:%d\r\n",__func__,conidx,param->offset,param->length);
    // retrieve handle information
    uint8_t status = f000s_get_att_idx(param->handle,  &att_idx);
    // If the attribute has been found, status is GAP_ERR_NO_ERROR
    if (status == GAP_ERR_NO_ERROR)
    {
        struct f000s_env_tag* f000s_env = PRF_ENV_GET(F000S, f000s);
        // Extract value before check
        uint16_t ntf_cfg = co_read16p(&param->value[0]);

        // Only update configuration if value for stop or notification enable
        // if ((att_idx == F000S_IDX_F004_VAL_NTF_CFG)
        //         && ((ntf_cfg == PRF_CLI_STOP_NTFIND) || (ntf_cfg == PRF_CLI_START_NTF)))
        // {
        //     // Conserve information in environment
        //     if (ntf_cfg == PRF_CLI_START_NTF)
        //     {
        //         // Ntf cfg bit set to 1
        //         f000s_env->ntf_cfg[conidx] = PRF_CLI_START_NTF ;
        //     }
        //     else
        //     {
        //         // Ntf cfg bit set to 0
        //         f000s_env->ntf_cfg[conidx] = PRF_CLI_STOP_NTFIND;
        //     }

        //     // Inform APP of configuration change
        //     struct f000s_f004_val_ntf_cfg_ind * ind = KE_MSG_ALLOC(F000S_F004_VALUE_NTF_CFG_IND,
        //             prf_dst_task_get(&(f000s_env->prf_env), conidx), dest_id,
        //             f000s_f004_val_ntf_cfg_ind);
        //     ind->conidx = conidx;
        //     ind->ntf_cfg = f000s_env->ntf_cfg[conidx];
						
        //     ke_msg_send(ind);			
        // }		
		if ((att_idx == F000S_IDX_F002_VAL_IND_CFG)
                && ((ntf_cfg == PRF_CLI_STOP_NTFIND) || (ntf_cfg == PRF_CLI_START_IND)))
        {
            // Conserve information in environment
            if (ntf_cfg == PRF_CLI_START_IND)
            {
                // Ntf cfg bit set to 1
                f000s_env->ind_cfg[conidx] = PRF_CLI_START_IND;
            }
            else
            {
                // Ntf cfg bit set to 0
                f000s_env->ind_cfg[conidx] = PRF_CLI_STOP_NTFIND;
            }

            // Inform APP of configuration change
            struct f000s_f002_val_ind_cfg_ind * ind = KE_MSG_ALLOC(F000S_F002_VALUE_IND_CFG_IND,
                    prf_dst_task_get(&(f000s_env->prf_env), conidx), dest_id,
                    f000s_f002_val_ind_cfg_ind);
            ind->conidx = conidx;
            ind->ind_cfg = f000s_env->ind_cfg[conidx];
						
            ke_msg_send(ind);			
        }
		// else if (att_idx == F000S_IDX_F007_VAL_VAL)
		// {
		// 	// Allocate the alert value change indication
		// 	struct f000s_f0073_writer_ind *ind = KE_MSG_ALLOC(F000S_F007_WRITER_CMD_IND,
		// 	        prf_dst_task_get(&(f000s_env->prf_env), conidx),
		// 	        dest_id, f000s_f0073_writer_ind);
			
		// 	// Fill in the parameter structure	
		// 	memcpy(ind->value,&param->value[0],param->length);
		// 	ind->conidx = conidx;
		// 	ind->length = param->length;
			
		// 	// Send the message
		// 	ke_msg_send(ind);
		// }
  //       else if (att_idx == F000S_IDX_F003_VAL_VAL)
		// {
		// 	// Allocate the alert value change indication
		// 	struct f000s_f0073_writer_ind *ind = KE_MSG_ALLOC(F000S_F003_WRITER_REQ_IND,
		// 	        prf_dst_task_get(&(f000s_env->prf_env), conidx),
		// 	        dest_id, f000s_f0073_writer_ind);
			
		// 	// Fill in the parameter structure	
		// 	memcpy(ind->value,&param->value[0],param->length);
		// 	ind->conidx = conidx;
		// 	ind->length = param->length;
			
		// 	// Send the message
		// 	ke_msg_send(ind);
		// }
        else
        {
            status = PRF_APP_ERROR;
        }

    }

    //Send write response
    cfm = KE_MSG_ALLOC(GATTC_WRITE_CFM, src_id, dest_id, gattc_write_cfm);
    cfm->handle = param->handle;
    cfm->status = status;
    ke_msg_send(cfm);


    return (KE_MSG_CONSUMED);
}   



static int gattc_read_req_ind_handler(ke_msg_id_t const msgid, struct gattc_read_req_ind const *param,
                                      ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    
    struct gattc_read_cfm * cfm;
    uint8_t  att_idx = 0;
    uint8_t conidx = KE_IDX_GET(src_id);
    // retrieve handle information
    uint8_t status = f000s_get_att_idx(param->handle, &att_idx);
    uint16_t length = 0;
    struct f000s_env_tag* f000s_env = PRF_ENV_GET(F000S, f000s);
    uart_printf("%s conidx:%d,handle:%d,att_idx:%d\r\n",__func__,conidx,param->handle,att_idx);
    // If the attribute has been found, status is GAP_ERR_NO_ERROR
    if (status == GAP_ERR_NO_ERROR)
    {
        // read notification information
        if (att_idx == F000S_IDX_F001_VAL_VAL)
        {
            length = F000_F001_CHAR_DATA_LEN * sizeof(uint8_t);
        }
//        else if (att_idx == F000S_IDX_F001_USER_DESC)
//        {
//            length = f000s_env->f001_desc_len;
//            uart_printf("read F001_USER_DESC\r\n");
//        }
//        // read notification information
//        else if (att_idx == F000S_IDX_F004_VAL_NTF_CFG)
//        {
//            length = sizeof(uint16_t);
//        }
		else if(att_idx == F000S_IDX_F002_VAL_IND_CFG)
		{
			length = sizeof(uint16_t);
		}
		
        else
        {
            status = PRF_APP_ERROR;
        }
    }

    //Send write response
    cfm = KE_MSG_ALLOC_DYN(GATTC_READ_CFM, src_id, dest_id, gattc_read_cfm, length);
    cfm->handle = param->handle;
    cfm->status = status;
    cfm->length = length;
   
    if (status == GAP_ERR_NO_ERROR)
    {
        // read notification information
        if (att_idx == F000S_IDX_F001_VAL_VAL)
        {
            memcpy(cfm->value,f000s_env->f001_val,F000_F001_CHAR_DATA_LEN);
        }
//        else if (att_idx == F000S_IDX_F001_USER_DESC)
//        {
//            memcpy(cfm->value,f000s_env->f001_desc,length);
//            uart_printf("USER_DESC:%s\r\n",f000s_env->f001_desc);
//        }
//        // retrieve notification config
//        else if (att_idx == F000S_IDX_F004_VAL_NTF_CFG)
//        {
//            uint16_t ntf_cfg = f000s_env->ntf_cfg[conidx];
//            co_write16p(cfm->value, ntf_cfg);
//        }  
		
		else if(att_idx == F000S_IDX_F002_VAL_IND_CFG)
		{
			uint16_t ind_cfg = f000s_env->ind_cfg[conidx];
            co_write16p(cfm->value, ind_cfg);
		}
        else
        {
            /* Not Possible */
        }
    }

    ke_msg_send(cfm);

    return (KE_MSG_CONSUMED);
}   

extern uint16_t g_hande;
static int gattc_cmp_evt_handler(ke_msg_id_t const msgid,  struct gattc_cmp_evt const *param,
                                 ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    uart_printf("%s\r\n",__func__);
    struct f000s_env_tag* f000s_env = PRF_ENV_GET(F000S, f000s);
    uint8_t conidx = KE_IDX_GET(src_id);
    if(param->operation == GATTC_NOTIFY)
    {	           
        struct f000s_f0045_val_upd_rsp *rsp = KE_MSG_ALLOC(F000S_F004_VALUE_UPD_RSP,
                                             prf_dst_task_get(&(f000s_env->prf_env), conidx),
                                             dest_id, f000s_f0045_val_upd_rsp);
        rsp->conidx = conidx;
        rsp->status = param->status;			
        ke_msg_send(rsp);
        
    }
    else if(param->operation == GATTC_INDICATE)
    {
        struct f000s_f0045_val_upd_rsp *rsp = KE_MSG_ALLOC(F000S_F002_VALUE_UPD_RSP,
                                             prf_dst_task_get(&(f000s_env->prf_env), conidx),
                                             dest_id, f000s_f0045_val_upd_rsp);
        rsp->conidx = conidx;
        rsp->status = param->status;			
        ke_msg_send(rsp);
    }
	// go back in to idle mode
    ke_state_set(dest_id, ke_state_get(dest_id) & ~F000S_BUSY);
	
    return (KE_MSG_CONSUMED);
}

/// Default State handlers definition
KE_MSG_HANDLER_TAB(f000s)
{
    {F000S_F004_VALUE_UPD_REQ,      (ke_msg_func_t) f000s_f004_val_upd_req_handler},
	{F000S_F002_VALUE_UPD_REQ,      (ke_msg_func_t) f000s_f002_val_upd_req_handler},
    {GATTC_ATT_INFO_REQ_IND,        (ke_msg_func_t) gattc_att_info_req_ind_handler},
    {GATTC_WRITE_REQ_IND,           (ke_msg_func_t) gattc_write_req_ind_handler},
    {GATTC_READ_REQ_IND,            (ke_msg_func_t) gattc_read_req_ind_handler},
    {GATTC_CMP_EVT,                 (ke_msg_func_t) gattc_cmp_evt_handler},
};

void f000s_task_init(struct ke_task_desc *task_desc)
{
    // Get the address of the environment
    struct f000s_env_tag *srv_env = PRF_ENV_GET(F000S, f000s);

    task_desc->msg_handler_tab = f000s_msg_handler_tab;
    task_desc->msg_cnt         = ARRAY_LEN(f000s_msg_handler_tab);
    task_desc->state           = srv_env->state;
    task_desc->idx_max         = F000S_IDX_MAX;
}

#endif /* #if (BLE_F000_SERVER) */


