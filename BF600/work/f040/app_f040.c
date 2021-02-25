/**
 ****************************************************************************************
 *
 * @file app_f040.c
 *
 * @brief f040 Application Module entry point
 *
 * @auth  gang.cheng
 *
 * @date  2020.03.17
 *
 * Copyright (C) Beken 2019-2022
 *
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup APP
 * @{
 ****************************************************************************************
 */

#include "rwip_config.h"     // SW configuration

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include <string.h>
#include "ke_task.h"                 // Kernel
#include "app_f040.h"              // Battery Application Module Definitions
#include "app.h"                   // Application Definitions
#include "app_task.h"              // application task definitions
#include "f040s_task.h"            // health thermometer functions
#include "co_bt.h"
#include "co_utils.h"
#include "prf_types.h"             // Profile common types definition
#include "arch.h"                  // Platform Definitions
#include "prf.h"
#include "f040s.h"
#include "ke_timer.h"
#include "uart.h"



/*
 * DEFINES
 ****************************************************************************************
 */

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/// fff0 Application Module Environment Structure
struct app_f040_env_tag app_f040_env;

/*
 * GLOBAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */


void app_f040_init(void)
{

  // Reset the environment
  memset(&app_f040_env, 0, sizeof(struct app_f040_env_tag));
}

void app_f040_add_f040s(void)
{

  struct f040s_db_cfg *db_cfg;

  struct gapm_profile_task_add_cmd *req = KE_MSG_ALLOC_DYN(GAPM_PROFILE_TASK_ADD_CMD,
                                          TASK_GAPM, TASK_APP,
                                          gapm_profile_task_add_cmd, sizeof(struct f040s_db_cfg));
  // Fill message
  req->operation = GAPM_PROFILE_TASK_ADD;
  req->sec_lvl =   0;
  req->prf_task_id = TASK_ID_F040S;
  req->app_task = TASK_APP;
  req->start_hdl = 0; //req->start_hdl = 0; dynamically allocated


  // Set parameters
  db_cfg = (struct f040s_db_cfg* ) req->param;

  db_cfg->f047_desc_len = strlen("f047_only_read");

  memcpy(db_cfg->f047_desc, "f047_only_read", strlen("f047_only_read"));

  // Sending of notifications is supported
  // Send the message
  ke_msg_send(req);
}


void app_f044_send_ntf(uint8_t conidx, uint16_t len, uint8_t* buf)
{
  // Allocate the message
  struct f040s_f0445_val_upd_req * req = KE_MSG_ALLOC(F040S_F044_VALUE_UPD_REQ,
                                         prf_get_task_from_id(TASK_ID_F040S),
                                         KE_BUILD_ID(TASK_APP, conidx),
                                         f040s_f0445_val_upd_req);
  // Fill in the parameter structure
  req->length = len;
  memcpy(req->value, buf, len);

  // Send the message
  ke_msg_send(req);
}

void app_f041_send_ntf(uint8_t conidx, uint16_t len, uint8_t* buf)
{
  // Allocate the message
  struct f040s_f0445_val_upd_req * req = KE_MSG_ALLOC(F040S_F041_VALUE_UPD_REQ,
                                         prf_get_task_from_id(TASK_ID_F040S),
                                         KE_BUILD_ID(TASK_APP, conidx),
                                         f040s_f0445_val_upd_req);
  // Fill in the parameter structure
  req->length = len;
  memcpy(req->value, buf, len);

  // Send the message
  ke_msg_send(req);
}



static int f040s_f044_val_ntf_cfg_ind_handler(ke_msg_id_t const msgid,
    struct f040s_f044_val_ntf_cfg_ind const *param,
    ke_task_id_t const dest_id,
    ke_task_id_t const src_id)
{
  uart_printf("f044->param->ntf_cfg = %x\r\n", param->ntf_cfg);
  if (param->ntf_cfg == PRF_CLI_STOP_NTFIND)
  {
    //ke_timer_clear(F040S_F047_LEVEL_PERIOD_NTF,dest_id);
  } else
  {
    //ke_timer_set(F040S_F047_LEVEL_PERIOD_NTF,dest_id , 100);
  }

  return (KE_MSG_CONSUMED);
}

static int f040s_f041_val_ind_cfg_ind_handler(ke_msg_id_t const msgid,
    struct f040s_f041_val_ind_cfg_ind const *param,
    ke_task_id_t const dest_id,
    ke_task_id_t const src_id)
{
  uart_printf("f041->param->ind_cfg = %x\r\n", param->ind_cfg);
  if (param->ind_cfg == PRF_CLI_STOP_NTFIND)
  {
    //ke_timer_clear(F040S_F043_LEVEL_PERIOD_NTF,dest_id);
  } else
  {
    //ke_timer_set(F040S_F043_LEVEL_PERIOD_NTF,dest_id , 100);
  }

  return (KE_MSG_CONSUMED);
}
static int f044_val_upd_rsp_handler(ke_msg_id_t const msgid,
                                    struct f040s_f0445_val_upd_rsp const *param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id)
{
  uart_printf("%s,status:%x\r\n", __func__, param->status);

  if (param->status == GAP_ERR_NO_ERROR)
  {

  }

  return (KE_MSG_CONSUMED);
}

static int f041_val_upd_rsp_handler(ke_msg_id_t const msgid,
                                    struct f040s_f0445_val_upd_rsp const *param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id)
{
  uart_printf("%s,status:%x\r\n", __func__, param->status);

  if (param->status == GAP_ERR_NO_ERROR)
  {

  }

  return (KE_MSG_CONSUMED);
}


/**
 ****************************************************************************************
 * @brief
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int app_f040_msg_dflt_handler(ke_msg_id_t const msgid,
                                     void const *param,
                                     ke_task_id_t const dest_id,
                                     ke_task_id_t const src_id)
{
  uart_printf("%s\r\n", __func__);

  // Drop the message
  return (KE_MSG_CONSUMED);
}


static int f042_writer_cmd_handler(ke_msg_id_t const msgid,
                                   struct f040s_f0423_writer_ind *param,
                                   ke_task_id_t const dest_id,
                                   ke_task_id_t const src_id)
{
  uint8_t buf[20];
  // Drop the message
  uart_printf("F042 conidx:%d,length:%d,param->value = 0x ", param->conidx, param->length);

  for (uint16_t i = 0; i < param->length; i++)
  {
    uart_printf("%02x ", param->value[i]);
  }

  uart_printf("\r\n");
  for (uint8_t i = 0; i < 20; i++)
  {
    buf[i] = i;
  }

  if (param->value[0] == 0x55)
  {
    app_f044_send_ntf(param->conidx, 20, buf);
  }
  if (param->value[0] == 0x66)
  {
    app_f041_send_ntf(param->conidx, 20, buf);
  }
#if 0
  if (param->value[0] == 0xAA)
  {
    struct gapc_conn_param conn_param;
    conn_param.intv_min = param->value[1];
    conn_param.intv_max = param->value[1] + 2;
    conn_param.latency  = 0;
    conn_param.time_out = 200;

    appm_update_param(&conn_param);
  }
#endif
  return (KE_MSG_CONSUMED);
}

static int f043_writer_req_handler(ke_msg_id_t const msgid,
                                   struct f040s_f0423_writer_ind *param,
                                   ke_task_id_t const dest_id,
                                   ke_task_id_t const src_id)
{
  // Drop the message
  uart_printf("F043 conidx:%d,length:%d,param->value = 0x ", param->conidx, param->length);

  for (uint16_t i = 0; i < param->length; i++)
  {
    uart_printf("%02x ", param->value[i]);
  }
  uart_printf("\r\n");

  return (KE_MSG_CONSUMED);
}



/*
 * LOCAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/// Default State handlers definition
const struct ke_msg_handler app_f040_msg_handler_list[] =
{
  // Note: first message is latest message checked by kernel so default is put on top.
  {KE_MSG_DEFAULT_HANDLER,        (ke_msg_func_t)app_f040_msg_dflt_handler},
  {F040S_F044_VALUE_NTF_CFG_IND,  (ke_msg_func_t)f040s_f044_val_ntf_cfg_ind_handler},
  {F040S_F041_VALUE_IND_CFG_IND,  (ke_msg_func_t)f040s_f041_val_ind_cfg_ind_handler},
  {F040S_F044_VALUE_UPD_RSP,      (ke_msg_func_t)f044_val_upd_rsp_handler},
  {F040S_F041_VALUE_UPD_RSP,      (ke_msg_func_t)f041_val_upd_rsp_handler},
  {F040S_F042_WRITER_CMD_IND,   (ke_msg_func_t)f042_writer_cmd_handler},
  {F040S_F043_WRITER_REQ_IND,   (ke_msg_func_t)f043_writer_req_handler},

};

const struct app_subtask_handlers app_f040_handler = APP_HANDLERS(app_f040);



