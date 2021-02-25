/**
 ****************************************************************************************
 *
 * @file app.c
 *
 * @brief Application entry point
 *
 * Copyright (C) RivieraWaves 2009-2015
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

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"             // SW configuration

#include <string.h>
//#include "rwapp_config.h"
#include "app_task.h"                // Application task Definition
#include "app.h"                     // Application Definition
#include "gap.h"                     // GAP Definition
#include "gapm_task.h"               // GAP Manager Task API
#include "gapc_task.h"               // GAP Controller Task API

#include "co_bt.h"                   // Common BT Definition
#include "co_math.h"                 // Common Maths Definition
#include "ke_timer.h"
#include "app_fff0.h"                 // Application security Definition
#include "app_ffa0.h"                 // Application security Definition
#include "app_ff50.h"              // fff0 Module Definition
#include "app_ff60.h"              // fff0 Module Definition
#include "app_ff70.h"              // fff0 Module Definition
#include "app_ff80.h"              // fff0 Module Definition
#include "app_ff90.h"              // fff0 Module Definition

#include "app_dis.h"                 // Device Information Service Application Definitions
#include "app_batt.h"                // Battery Application Definitions
#include "app_oads.h"                 // Application oads Definition
#if (NVDS_SUPPORT)
#include "nvds.h"                    // NVDS Definitions
#endif
#include "rf.h"
#include "uart.h"
#include "adc.h"
#include "gpio.h"
#include "wdt.h"
#include "rtc.h"
#include "prf_utils.h"
#include "uart2.h"       // uart definition
#include "icu.h"

#include "gap.h"
#include "gattc_task.h"
#include "attm.h"
#include "atts.h"
#include "co_utils.h"
#include "ff50s.h"
#include "ff50s_task.h"
#include "uart.h"
#include "ff60s.h"
#include "ff60s_task.h"

#include "ff80s.h"
#include "ff80s_task.h"

#include "fff0s.h"
#include "fff0s_task.h"

#include "fee0s.h"
#include "fee0s_task.h"

#include "bass.h"
#include "bass_task.h"

/*
 * DEFINES
 ****************************************************************************************
 */
void set_ff50_ff52_0x2a9e_rd(uint8_t* buf);
void set_ff50_ff51_0x2a9d_ind(uint8_t* buf);
void set_ff60_ff62_0x2a9b_rd(uint8_t* buf);
void set_ff60_ff61_0x2a9c_ind(uint8_t* buf);
void set_bass_batt_0x2a19_rd(uint8_t batt);
void set_bass_batt_0x2a19_rd(uint8_t batt);
extern void app_fff1_send_lvl(uint8_t* buf, uint8_t len);
extern void app_fff3_send_lvl(uint8_t* buf, uint8_t len);
void xs_uart_send_data(uint8_t *buf, uint8_t len);
unsigned short day_of_year(unsigned short y,unsigned short m,unsigned short d);
void set_fee0_fee2_0x2a85_rd(uint8_t* buf);
void set_fee0_fee3_0x2a9f_ntf(uint8_t* buf,uint8_t len);
void set_fee0_fee4_0x2a8c_rd(uint8_t* buf);
void set_fee0_fee5_0x2a8e_rd(uint8_t* buf);
void set_fee0_fee6_0x2a9a_rd(uint8_t* buf);
void set_fff0_fff5_rd(uint8_t* buf);
void set_fff0_fff4_rd(uint8_t* buf);
void send_user_list();
void get_scale_user();
void check_select_user();
void set_select_user_info_to_ble_chara();
void set_fee0_fee1_0x2a99_ntf(uint8_t* buf);
void set_fee0_fee1_0x2a99_rd(uint8_t* buf);
void regist_user_on_scale(uint8_t pinL,uint8_t pinH);
void update_select_user_to_scale(uint8_t user_id);
void regist_user_check(uint8_t pinL,uint8_t pinH);
void delay_1s_send_time(int t);
void delay_1s_send_user_birthday(int t);
void set_ff80_ff81_0x2a9b_ntf(uint8_t* buf);
void delay_1s_modify_user_birthday(int t);
void update_weight_body_measurement(uint8_t *buf);
void ji_huo_scale_and_send();
void delay_1s_send_resgist_ok(int t);
void delay_1s_send_take_measure(int t);
void delay_1s_updata_db(int t);
void delay_1s_get_user_info(int t);
void set_fff0_fff2_rd(uint8_t* buf);
uint8_t ignore_repetition(uint8_t *b);
void delay_1s_update_user_info(int t);


uint8_t g_user_num=0;
//[0]=flag [1]=user id [2,3,4]=init [5,6]=year [7]=month [8]=day [9]=height [10]=sex [11]=active
//[12,13]=CONSENT [14,15]=weight/BF  [16,17]=DB
uint8_t g_user_list[18]={0x00,0x01,0xFF,0xFF,0xFF,0xC7,0x07,0x0A,0x01,0xA5,0x01,0x03,0,0,0,0,0,0};
uint8_t g_user_info[18]={0x00,0x01,0xFF,0xFF,0xFF,0xC7,0x07,0x0A,0x01,0xA5,0x01,0x03,0,0,0,0,0,0};
uint8_t g_user_select[3]={0};
uint8_t g_user_select_index=0xff;
uint8_t g_curr_time[8];
uint8_t g_current_time_from_ble[10];
int g_sync_time_counter=0;
uint8_t g_get_user_list=0;
uint8_t g_measure_data[47]={0};
uint8_t g_measure_data_len=0;
uint8_t g_ble_state=0;
uint8_t g_last_msg[8]={0};
uint8_t g_scale_state=1;
uint8_t g_last_msg_set=0;
uint8_t g_rigist_user_id=0;
uint8_t g_user_registing=0;
//ble state
#define BLE_STATE_PIN 0x07

void xs_user_task()
{
	static int t=0,inited=0;

	if(inited==0)
	{
		inited=1;
	}

	if(t++>100)
	{
		t=0;
	}
	delay_1s_send_time(0);
	// delay_1s_send_user_birthday(0);
	delay_1s_modify_user_birthday(0);
	ji_huo_scale_and_send();
	delay_1s_send_resgist_ok(0);
	delay_1s_send_take_measure(0);
	delay_1s_updata_db(0);
	delay_1s_get_user_info(0);
	delay_1s_update_user_info(0);

}
void init_ble_state()
{
	
	gpio_config(BLE_STATE_PIN, OUTPUT, PULL_NONE);
	gpio_set(BLE_STATE_PIN, 1);

	UART_PRINTF("init_ble_state:1\n");

}
// s=0 已连接   s=1 未连接
void set_ble_state(uint8_t s)
{
	gpio_set(BLE_STATE_PIN, s);

	g_ble_state=s;
	g_user_registing=0;

	if(s==0)
	{
		icu_set_sleep_mode(1);
	}
	else
	{
		icu_set_sleep_mode(0);
	}
	

}
//uart----------------------------------------------------------------------------------

 void xs_uart2_send_data(uint8_t *buf, uint8_t len)
 {
 	UART_PRINTF("uart2_tx:");
 	for(uint8_t i=0; i<len; i++)
	{
		UART_PRINTF("%x ", buf[i]);
	}
	UART_PRINTF("\r\n");
 	uart2_write(buf,len,NULL,NULL);
 }

 void xs_uart2_received_isr(uint8_t *buf, uint8_t len)
 {
 	UART_PRINTF("uart2_rx:");
 	for(uint8_t i=0; i<len; i++)
	{
		UART_PRINTF("%02x ", buf[i]);
	}
	UART_PRINTF("\r\n");
	// xs_uart_send_data("55667788",8);
	if(buf[0]=='0')
	{
		
	}
 }
 //uart2_write("123456",6,0,0);
 void xs_uart_send_data(uint8_t *buf, uint8_t len)
 {
 	UART_PRINTF("uart_tx:");
 	for(uint8_t i=0; i<len; i++)
	{
		UART_PRINTF("%02X ", buf[i]);
	}
	UART_PRINTF("\r\n");
 	uart_write(buf,len,NULL,NULL);
 	if(g_scale_state==0)
 	{
 		g_last_msg_set=1;
 		memcpy(g_last_msg,buf,8);
 		UART_PRINTF("g_last_msg_set=1\n");
 	}
 	
 }
 void clear_uart_received_buf()
 {	
 	uint8_t b[8]={0};
 	ignore_repetition(b);
 }
 uint8_t ignore_repetition(uint8_t *b)
 {
 	static uint8_t last_buf[8]={0};

 	if(last_buf[0]==b[0] &&
 		last_buf[1]==b[1] &&
 		last_buf[2]==b[2] &&
 		last_buf[3]==b[3] &&
 		last_buf[4]==b[4] &&
 		last_buf[5]==b[5] &&
 		last_buf[6]==b[6] &&
 		last_buf[7]==b[7] )
 	{
 		return 1;
 	}
 	else
 	{
		last_buf[0]=b[0];
 		last_buf[1]=b[1];
 		last_buf[2]=b[2];
 		last_buf[3]=b[3];
 		last_buf[4]=b[4];
 		last_buf[5]=b[5];
 		last_buf[6]=b[6];
 		last_buf[7]=b[7];
		
 		return 0;
 	}
 }
 void xs_uart_received_isr(uint8_t *buf, uint8_t len)
 {
 	UART_PRINTF("uart1_rx:");
 	for(uint8_t i=0; i<len; i++)
	{
		UART_PRINTF("%02X ", buf[i]);
	}
	UART_PRINTF("\r\n");
	if(ignore_repetition(buf))
	{
		return;
	}
	if(buf[0]==0xFF && buf[1]==0xA5)
	{
		g_scale_state=1;
		if(buf[6]==0xBA)
		{
			uint8_t fff2_buf[8]={0};
			struct bass_env_tag* bass_env = PRF_ENV_GET(BASS, bass);
			if(buf[2]>0x1F)
			{
				buf[2]=0x1F;
			}
			if(buf[2]<0x15)
			{
				buf[2]=0x15;
			}
			buf[2]=(buf[2]-0x15)*10;
 			bass_env->batt_lvl[0]=buf[2];

 			fff2_buf[4]=buf[3];
 			fff2_buf[1]=buf[4]>>1;

 			set_fff0_fff2_rd(fff2_buf);
 			
		}
		else if(buf[6]==0xBD)
		{
			g_scale_state=0;
			UART_PRINTF("g_scale_state=0\n");
		}
		else if(buf[6]==0xE0)
		{
			g_user_list[10]=(buf[2]==1?0:1);
			g_user_list[6]=0x07;
			g_user_list[5]=buf[3];
			g_user_list[7]=buf[4];
			g_user_list[8]=buf[5];

			g_user_info[10]=(buf[2]==1?0:1);

		}
		else if(buf[6]==0xE1)
		{
			g_user_list[9]=buf[2];
			g_user_list[11]=buf[5];
		}
		else if(buf[6]==0xE4)
		{
			g_user_list[12]=buf[2];
			g_user_list[13]=buf[3];
			g_user_list[16]=buf[4];
			g_user_list[17]=buf[5];

		}
		else if(buf[6]==0xE5)
		{
			uint8_t buff_2a9f[4];
			g_user_list[0]=0;
			g_user_list[1]=buf[2];
			set_select_user_info_to_ble_chara();
			if(g_get_user_list==1)
			{
				app_fff1_send_lvl(g_user_list,FFF0_FFF1_DATA_LEN);
			}

			buff_2a9f[0]=0x20;
			buff_2a9f[1]=0x02;
			buff_2a9f[2]=0x01;			
			set_fee0_fee3_0x2a9f_ntf(buff_2a9f,3);			

		}
		else if(buf[6]==0xF0)
		{
			if(g_get_user_list==1)
			{
				if(buf[2]>0)
				{
					uint8_t user_finish_flag[1]={1};
					app_fff1_send_lvl(user_finish_flag,1);
				}
				else
				{
					uint8_t no_user_flag[1]={2};
					app_fff1_send_lvl(no_user_flag,1);
				}	
				g_get_user_list=0;
			}
			if(buf[2]==0)
			{
				if(g_user_select[0]==g_user_list[1])
				{
					g_user_list[14]=buf[4];
					g_user_list[15]=buf[5];
					set_select_user_info_to_ble_chara();
				}
			}
			
					
		}
		else if(buf[2]==0x10 && buf[6]==0 )
		{
			delay_1s_modify_user_birthday(10);
		}
		else if(buf[2]==0x11 && buf[6]==0)
		{
			g_user_list[1]=buf[3];
			if(g_user_registing==1)
			{
				delay_1s_send_resgist_ok(10);
			}
			else
			{
				delay_1s_updata_db(10);	
			}					
		}
		else if(buf[2]==0x32 && buf[6]==0)
		{			
			
		}
		else if(buf[6]==0xE6)
		{
			if(g_user_select[0]==buf[2] &&
				g_user_select[1]==buf[3] &&
				g_user_select[2]==buf[4])
			{
				UART_PRINTF("CONSENT CODE OK!");
				delay_1s_get_user_info(10);				
			}
			else
			{
				UART_PRINTF("CONSENT CODE ERROR!");
				uint8_t buff_2a9f[4];
				buff_2a9f[0]=0x20;
				buff_2a9f[1]=0x02;
				buff_2a9f[2]=0x05;			
				set_fee0_fee3_0x2a9f_ntf(buff_2a9f,3);
			}
		}
		else if(buf[2]==0x30 && buf[6]==0)
		{
			delay_1s_send_time(10);
		}
		else if(buf[6]==0xE2)
		{
			g_current_time_from_ble[0]=buf[2];
			g_current_time_from_ble[1]=0x07;
			g_current_time_from_ble[2]=buf[3];
			g_current_time_from_ble[3]=buf[4];
		}
		else if(buf[6]==0xE3)
		{
			g_current_time_from_ble[4]=buf[2];
			g_current_time_from_ble[5]=buf[3];
			g_current_time_from_ble[6]=buf[4];
			set_ff80_ff81_0x2a9b_ntf(g_current_time_from_ble);
		}
		g_measure_data_len=0;

	}
	else if(buf[0]==0xFA)
	{
		memcpy(g_measure_data,buf,len);
		g_measure_data_len+=len;
	}
	else if(g_measure_data_len>0)
	{
		memcpy(g_measure_data+g_measure_data_len,buf,len);
		g_measure_data_len+=len;

		if(g_measure_data_len>=47)
		{
			update_weight_body_measurement(g_measure_data);			
			UART_PRINTF("g_measure_data_len:%d\n\r",g_measure_data_len);
			for(uint8_t i=0; i<47; i++)
			{
				UART_PRINTF("%02X ", g_measure_data[i]);
			}
			g_measure_data_len=0;
		}
		
		
	}
}

 //Weight Scale----------------------------------------------------------------------------------
 
 //Weight Scale Feature  0x2a9e
void set_ff50_ff52_0x2a9e_rd(uint8_t* buf)
 {
 	struct ff50s_env_tag* ff50s_env = PRF_ENV_GET(FF50S, ff50s);
 	memcpy(ff50s_env->ff52_value,buf,FF50_FF52_DATA_LEN);

 }
void set_ff50_ff51_0x2a9d_ind(uint8_t* buf)
{
	extern void app_ff51_send_lvl(uint8_t* buf, uint8_t len);

	app_ff51_send_lvl(buf,FF50_FF51_DATA_LEN);
	UART_PRINTF("set_ff50_ff51_0x2a9d_ind\r\n");
}

//Body Composition------------------------------------------------------------------------------

 void set_ff60_ff62_0x2a9b_rd(uint8_t* buf)
 {
 	struct ff60s_env_tag* ff60s_env = PRF_ENV_GET(FF60S, ff60s);
 	memcpy(ff60s_env->ff62_value,buf,FF60_FF62_DATA_LEN);
 }
 void set_ff60_ff61_0x2a9c_ind(uint8_t* buf)
{
	extern void app_ff61_send_lvl(uint8_t* buf, uint8_t len);	
	app_ff61_send_lvl(buf,FF60_FF61_DATA_LEN);	
	UART_PRINTF("set_ff60_ff61_0x2a9c_ind\r\n");
}
//User Data
void set_fee0_fee1_0x2a99_ntf(uint8_t* buf)
{
	extern void app_fee1_send_lvl(uint8_t* buf, uint8_t len);
	struct fee0s_env_tag* fee0s_env = PRF_ENV_GET(FEE0S, fee0s);
	
 	app_fee1_send_lvl(buf,FEE0_FEE1_DATA_LEN);
 	memcpy(fee0s_env->fee1_value,buf,FEE0_FEE1_DATA_LEN);
 	UART_PRINTF("set_fee0_fee1_0x2a99_ntf Database Change Increment\r\n");

}
//Database Change Increment
void set_fee0_fee1_0x2a99_rd(uint8_t* buf)
{
	extern void app_fee1_send_lvl(uint8_t* buf, uint8_t len);
	struct fee0s_env_tag* fee0s_env = PRF_ENV_GET(FEE0S, fee0s);

 	memcpy(fee0s_env->fee1_value,buf,FEE0_FEE1_DATA_LEN);
 	UART_PRINTF("set_fee0_fee1_0x2a99_rd\r\n");
	
 	for(uint8_t i=0; i<4; i++)
	{
		UART_PRINTF("%02x ", buf[i]);
	}
}

void fee0_fee1_0x2a99_cb(uint8_t* buf)
{
	UART_PRINTF("fee0_fee1_0x2a99_cb\r\n");

	g_user_list[16]=buf[0];
	g_user_list[17]=buf[1];
	delay_1s_update_user_info(10);
}
void set_fee0_fee2_0x2a85_rd(uint8_t* buf)
{
	struct fee0s_env_tag* fee0s_env = PRF_ENV_GET(FEE0S, fee0s);
 	memcpy(fee0s_env->fee2_value,buf,FEE0_FEE2_DATA_LEN);
}
void fee0_fee2_0x2a85_cb(uint8_t* buf)
{
	UART_PRINTF("fee0_fee2_0x2a85_cb\r\n");
	//{0x00,0x01,0xFF,0xFF,0xFF,0xC7,0x07,0x0A,0x01,0xA5,0x01,0x03,0,0};
	g_user_list[5]=buf[0];
	g_user_list[6]=buf[1];
	g_user_list[7]=buf[2];
	g_user_list[8]=buf[3];
	
}
//User Control Point
void set_fee0_fee3_0x2a9f_ntf(uint8_t* buf,uint8_t len)
{
	extern void app_fee3_send_lvl(uint8_t* buf, uint8_t len);
	struct fee0s_env_tag* fee0s_env = PRF_ENV_GET(FEE0S, fee0s);
	
 	app_fee3_send_lvl(buf,len);
 	memcpy(fee0s_env->fee3_value,buf,len);
 	UART_PRINTF("set_fee0_fee3_0x2a9f_ntf User Control Point\r\n");
}
void fee0_fee3_0x2a9f_cb(uint8_t* buf)
{
	UART_PRINTF("fee0_fee3_0x2a9f_cb\r\n");
	//creat user 0x01 00 00
	uint8_t buff[8]={0};	
	if(buf[0]==0x01)
	{
		g_user_registing=1;
		buff[0]=0xA5;
		buff[1]=0x10;
		buff[2]=0;
		buff[3]=buf[1];
		buff[4]=buf[2];
		buff[5]=0xA5;
		buff[6]=0x03;
		buff[7]=buff[1]+buff[2]+buff[3]+buff[4]+buff[5]+buff[6];
		xs_uart_send_data(buff,8);

	}
	else if(buf[0]==0x02)
	{
		//A5 25 02 00 00 00 00 27
		g_user_select[0]=buf[1];
		g_user_select[1]=buf[2];
		g_user_select[2]=buf[3];

		for(int i=0;i<18;i++)
		{
			g_user_list[i]=0xFF;
		}
		set_select_user_info_to_ble_chara();
		clear_uart_received_buf();
		buff[0]=0xA5;
		buff[1]=0x25;
		buff[2]=g_user_select[0];
		buff[3]=0;
		buff[4]=0;
		buff[5]=0;
		buff[6]=0;
		buff[7]=buff[1]+buff[2]+buff[3]+buff[4]+buff[5]+buff[6];
		xs_uart_send_data(buff,8);
	}
	else if(buf[0]==0x03)
	{
		//A5 21 02 D2 04 00 00 F9 
		buff[0]=0xA5;
		buff[1]=0x21;
		buff[2]=g_user_select[0];
		buff[3]=g_user_select[1];
		buff[4]=g_user_select[2];
		buff[5]=0;
		buff[6]=0;
		buff[7]=buff[1]+buff[2]+buff[3]+buff[4]+buff[5]+buff[6];
		xs_uart_send_data(buff,8);
		 
	}
	
}
//Gender
void set_fee0_fee4_0x2a8c_rd(uint8_t* buf)
{
	struct fee0s_env_tag* fee0s_env = PRF_ENV_GET(FEE0S, fee0s);
 	memcpy(fee0s_env->fee4_value,buf,FEE0_FEE4_DATA_LEN);
}
void fee0_fee4_0x2a8c_cb(uint8_t* buf)
{
	UART_PRINTF("fee0_fee4_0x2a8c_cb\r\n");
	g_user_list[10]=buf[0];

}

void set_fee0_fee5_0x2a8e_rd(uint8_t* buf)
{
	struct fee0s_env_tag* fee0s_env = PRF_ENV_GET(FEE0S, fee0s);
 	memcpy(fee0s_env->fee5_value,buf,FEE0_FEE5_DATA_LEN);
}
void fee0_fee5_0x2a8e_cb(uint8_t* buf)
{
	UART_PRINTF("fee0_fee5_0x2a8e_cb\r\n");
	g_user_list[9]=buf[0];
}

void set_fee0_fee6_0x2a9a_rd(uint8_t* buf)
{
	struct fee0s_env_tag* fee0s_env = PRF_ENV_GET(FEE0S, fee0s);
 	memcpy(fee0s_env->fee6_value,buf,FEE0_FEE6_DATA_LEN);
}

//Current Time 
void set_ff80_ff81_0x2a9b_ntf(uint8_t* buf)
{
	extern void app_ff81_send_lvl(uint8_t* buf, uint8_t len);
	struct ff80s_env_tag* ff80s_env = PRF_ENV_GET(FF80S, ff80s);
	
 	// app_ff81_send_lvl(buf,FF80_FF81_DATA_LEN);
 	memcpy(ff80s_env->ff81_value,buf,FF80_FF81_DATA_LEN);
}
void ff80_ff81_0x2a9b_cb(uint8_t* buf)
{
	UART_PRINTF("ff80_ff81_0x2a9b_cb\r\n");
	// set_ff80_ff81_0x2a9b_ntf(buf);
	// set_bass_batt_0x2a19_rd(80);
	
	uint8_t buff[8]={0};		

	buff[0]=0xA5;
	buff[1]=0x30;
	buff[2]=buf[0];
	buff[3]=buf[2];
	buff[4]=buf[3];
	buff[5]=0;
	buff[6]=0;
	buff[7]=buff[1]+buff[2]+buff[3]+buff[4]+buff[5]+buff[6];
	xs_uart_send_data(buff,8);//send date
	
	g_curr_time[0]=0xA5;
	g_curr_time[1]=0x31;
	g_curr_time[2]=buf[4];
	g_curr_time[3]=buf[5];
	g_curr_time[4]=buf[6];
	g_curr_time[5]=0;
	g_curr_time[6]=0;
	g_curr_time[7]=g_curr_time[1]+g_curr_time[2]+g_curr_time[3]+g_curr_time[4]+g_curr_time[5]+g_curr_time[6];
	
}
// battery
void set_bass_batt_0x2a19_rd(uint8_t batt)
{
	struct bass_env_tag* bass_env = PRF_ENV_GET(BASS, bass);
 	bass_env->batt_lvl[0]=batt;
}
//user list
void fff0_fff1_cb(uint8_t* buf)
{
	static uint8_t index=0;
	if(buf[0]==0)
	{
		g_get_user_list=1;
		get_scale_user();
	}
	else if(buf[0]&0x10)
	{
		uint8_t buff[8]={0};		

		buff[0]=0xA5;
		buff[1]=0x25;
		buff[2]=buf[0]&0x0F;
		buff[3]=0;
		buff[4]=0;
		buff[5]=0;
		buff[6]=0;
		buff[7]=buff[1]+buff[2]+buff[3]+buff[4]+buff[5]+buff[6];
		xs_uart_send_data(buff,8);//send date
	}
	UART_PRINTF("fff0_fff1_cb\r\n");


}
void set_fff0_fff2_rd(uint8_t* buf)
{
	struct fff0s_env_tag* fff0s_env = PRF_ENV_GET(FFF0S, fff0s);
 	memcpy(fff0s_env->fff2_value,buf,FFF0_FFF2_DATA_LEN);
}

void fff0_fff2_cb(uint8_t* buf)
{
	UART_PRINTF("fff0_fff2_cb\r\n");
	uint8_t buff[8]={0};		

	buff[0]=0xA5;
	buff[1]=0x33;
	buff[2]=1<<buf[1];
	buff[3]=buf[4];
	buff[4]=0;
	buff[5]=0;
	buff[6]=0;
	buff[7]=buff[1]+buff[2]+buff[3]+buff[4]+buff[5]+buff[6];
	xs_uart_send_data(buff,8);//send date

}
void fff0_fff3_cb(uint8_t* buf)
{
	UART_PRINTF("fff0_fff3_cb\r\n");

	if(buf[0]==0)
	{
		delay_1s_send_take_measure(10);
	}
}
void set_fff0_fff4_rd(uint8_t* buf)
{
	struct fff0s_env_tag* fff0s_env = PRF_ENV_GET(FFF0S, fff0s);
 	memcpy(fff0s_env->fff4_value,buf,FFF0_FFF4_DATA_LEN);
}
void fff0_fff4_cb(uint8_t* buf)
{
	UART_PRINTF("fff0_fff4_cb\r\n");

	g_user_list[11]=buf[0];

}
//Refer Weight/BF
void set_fff0_fff5_rd(uint8_t* buf)
{
	struct fff0s_env_tag* fff0s_env = PRF_ENV_GET(FFF0S, fff0s);
 	memcpy(fff0s_env->fff5_value,buf,FFF0_FFF5_DATA_LEN);
}
void fff0_fff5_cb(uint8_t* buf)
{
	UART_PRINTF("fff0_fff5_cb\r\n");
}
//function

void get_scale_user()
{
	uint8_t buff[8];
	g_user_num=0;

	buff[0]=0xA5;
	buff[1]=0x23;
	buff[2]=0;
	buff[3]=0;
	buff[4]=0;
	buff[5]=0;
	buff[6]=0;
	buff[7]=buff[1]+buff[2]+buff[3]+buff[4]+buff[5]+buff[6];
	xs_uart_send_data(buff,8);//
}


void set_select_user_info_to_ble_chara()
{
	
	uint8_t buff_2a85[4];
	uint8_t buff_2a8c[1];
	uint8_t buff_2a8e[1];
	uint8_t buff_2a9a[1];
	uint8_t buff_fff4[1];
	uint8_t buff_fff5[4];
	uint8_t buff_2a99[4];
	uint8_t buff_2a9e[4]={0xB7,0,0,0};
	uint8_t buff_2a9b[4]={0xCF,0x31,0,0};
	
	buff_2a85[0]=g_user_list[5];
	buff_2a85[1]=g_user_list[6];
	buff_2a85[2]=g_user_list[7];
	buff_2a85[3]=g_user_list[8];

	buff_2a8c[0]=g_user_list[10];

	buff_2a8e[0]=g_user_list[9];

	buff_2a9a[0]=g_user_list[1];

	buff_fff4[0]=g_user_list[11];

	buff_fff5[0]=g_user_list[15];
	buff_fff5[1]=g_user_list[14];
	buff_fff5[2]=0xFF;
	buff_fff5[3]=0xFF;

	buff_2a99[0]=g_user_list[16];
	buff_2a99[1]=g_user_list[17];
	buff_2a99[2]=0;
	buff_2a99[3]=0;
	
	set_fee0_fee2_0x2a85_rd(buff_2a85);
	set_fee0_fee4_0x2a8c_rd(buff_2a8c);
	set_fee0_fee5_0x2a8e_rd(buff_2a8e);
	set_fee0_fee6_0x2a9a_rd(buff_2a9a);
	set_fee0_fee1_0x2a99_rd(buff_2a99);
	set_fff0_fff4_rd(buff_fff4);
	set_fff0_fff5_rd(buff_fff5);

	set_ff60_ff62_0x2a9b_rd(buff_2a9b);
	set_ff50_ff52_0x2a9e_rd(buff_2a9e);
}


void delay_1s_send_time(int t)
{
	static int t1=0;
	if(t>0)
	{
		t1=t;
	}
	if(t1>0)
	{
		t1--;
		if(t1==0)
		{
			xs_uart_send_data(g_curr_time,8);//send time
		}
	}
}

void delay_1s_modify_user_birthday(int t)
{
	static int t1=0;
	if(t>0)
	{
		t1=t;
	}
	if(t1>0)
	{
		t1--;
		if(t1==0)
		{
			//A5 11 C7 0A 01 00 00 E3
			uint8_t buff[8];
			buff[0]=0xA5;
			buff[1]=0x11;
			buff[2]=g_user_list[5];
			buff[3]=g_user_list[7];
			buff[4]=g_user_list[8];
			buff[5]=0;
			buff[6]=0;
			buff[7]=buff[1]+buff[2]+buff[3]+buff[4]+buff[5]+buff[6];
			xs_uart_send_data(buff,8);
		}
	}
}
void delay_1s_updata_db(int t)
{
	static int t1=0;
	if(t>0)
	{
		t1=t;
	}
	if(t1>0)
	{
		t1--;
		if(t1==0)
		{
			//A5 32 08 00 00 00 00
			uint8_t buff[8];
			buff[0]=0xA5;
			buff[1]=0x32;
			buff[2]=g_user_list[16];
			buff[3]=g_user_list[17];
			buff[4]=0;
			buff[5]=0;
			buff[6]=0;
			buff[7]=buff[1]+buff[2]+buff[3]+buff[4]+buff[5]+buff[6];
			xs_uart_send_data(buff,8);
		}
	}
}
void delay_1s_update_user_info(int t)
{
	static int t1=0;
	if(t>0)
	{
		t1=t;
	}
	if(t1>0)
	{
		t1--;
		if(t1==0)
		{
			uint8_t buff[8];
			buff[0]=0xA5;
			buff[1]=0x10;
			buff[2]=(g_user_list[10]<<7)|g_user_list[1];
			buff[3]=g_user_list[13];
			buff[4]=g_user_list[12];
			buff[5]=g_user_list[9];
			buff[6]=g_user_list[11];
			buff[7]=buff[1]+buff[2]+buff[3]+buff[4]+buff[5]+buff[6];
			xs_uart_send_data(buff,8);
		}
	}
	
}
void update_weight_body_measurement(uint8_t *buf)
{
	uint8_t weight_m[15]={0};
	uint8_t body_m[14]={0};
	uint8_t rf_m[1]={1};

	//Flags	Weight		Year		Month	Day	Hour	Min	Second	User ID	BMI		Height	
    
	weight_m[0]=0x0E;
	weight_m[1]=(buf[15]<<1)&0xFF;
	weight_m[2]=((buf[16]<<1)|(buf[15]>>7))&0xFF;
	weight_m[3]=buf[41];
	weight_m[4]=0x07;
	weight_m[5]=buf[40];
	weight_m[6]=buf[39];
	weight_m[7]=buf[42];
	weight_m[8]=buf[43];
	weight_m[9]=buf[44];
	weight_m[10]=buf[1];
	weight_m[11]=buf[19];
	weight_m[12]=buf[20];
	weight_m[13]=(buf[10]*10)&0xFF;
	weight_m[14]=(buf[10]*10)>>8;
	set_ff50_ff51_0x2a9d_ind(weight_m);
	//Flags		Body Fat %		BMR		Muscle %		Soft Lean Mass		Body Water mass		Impedance	

	body_m[0]=0x98;
	body_m[1]=0x03;
	body_m[2]=buf[21];
	body_m[3]=buf[22];
	body_m[4]=buf[29];
	body_m[5]=buf[30];
	body_m[6]=buf[25];
	body_m[7]=buf[26];
	body_m[8]=buf[35];
	body_m[9]=buf[36];
	body_m[10]=buf[23];
	body_m[11]=buf[24];
	body_m[12]=buf[13];
	body_m[13]=buf[14];


	set_ff60_ff61_0x2a9c_ind(body_m);

	app_fff3_send_lvl(rf_m,1);

}


void ji_huo_scale_and_send()
{
	if(g_last_msg_set==1 && g_scale_state==1 )
	{
		UART_PRINTF("g_last_msg_set==1 && g_scale_state==1\n");
		xs_uart_send_data(g_last_msg,8);//
		g_last_msg_set=0;
	}
}

void delay_1s_send_resgist_ok(int t)
{
	static int t1=0;
	if(t>0)
	{
		t1=t;
	}
	if(t1>0)
	{
		t1--;
		if(t1==0)
		{
			uint8_t buff_2a9f[4];
			g_user_registing=0;
			buff_2a9f[0]=0x20;
			buff_2a9f[1]=0x01;
			buff_2a9f[2]=0x01;
			buff_2a9f[3]=g_user_list[1];		
			set_fee0_fee3_0x2a9f_ntf(buff_2a9f,4);
		}
	}
}

void delay_1s_send_take_measure(int t)
{
	static int t1=0;
	if(t>0)
	{
		t1=t;
	}
	if(t1>0)
	{
		t1--;
		if(t1==0)
		{
			//A5 60 03 00 00 00 00 60
			uint8_t buff[8];
			buff[0]=0xA5;
			buff[1]=0x60;
			buff[2]=g_user_select[0];
			buff[3]=0;
			buff[4]=0;
			buff[5]=0;
			buff[6]=0;
			buff[7]=buff[1]+buff[2]+buff[3]+buff[4]+buff[5]+buff[6];
			xs_uart_send_data(buff,8);//
		}
	}
}

void delay_1s_get_user_info(int t)
{
	static int t1=0;
	if(t>0)
	{
		t1=t;
	}
	if(t1>0)
	{
		t1--;
		if(t1==0)
		{
			uint8_t buff[8];
				
			buff[0]=0xA5;
			buff[1]=0x20;
			buff[2]=g_user_select[0];
			buff[3]=0;
			buff[4]=0;
			buff[5]=0;
			buff[6]=0;
			buff[7]=buff[1]+buff[2]+buff[3]+buff[4]+buff[5]+buff[6];
			xs_uart_send_data(buff,8);
		}
	}
}