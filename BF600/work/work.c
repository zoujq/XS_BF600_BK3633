#include "user_config.h"

#include "rwip_config.h"             // SW configuration

#if (BLE_APP_PRESENT)
#include "rwapp_config.h"
#include <string.h>
#include <time.h>

#include "rwip.h"
#include "app_task.h"                // Application task Definition
#include "app.h"                     // Application Definition
#include "gap.h"                     // GAP Definition
#include "gapm_task.h"               // GAP Manager Task API
#include "gapc_task.h"               // GAP Controller Task API
#include "prf_utils.h"
#include "gpio.h"
#include "uart2.h"
#include "icu.h"
#include "bass.h"

#include "co_bt.h"                   // Common BT Definition
#include "co_math.h"                 // Common Maths Definition

#if (BLE_APP_SEC)
#include "app_sec.h"                 // Application security Definition
#endif // (BLE_APP_SEC)

#if (BLE_APP_FEE0S)
#include "app_fee0.h"                  // USER define FEE0S Application Definitions
#endif //(BLE_APP_FEE0S)

#if (BLE_APP_F000S)
#include "app_f000.h"                  // USER define FEE0S Application Definitions
#include "f000s.h"                  // USER define FEE0S Application Definitions
#include "f000s_task.h"                  // USER define FEE0S Application Definitions
#endif //(BLE_APP_F000S)

#if (BLE_APP_F010S)
#include "app_f010.h"                  // USER define FEE0S Application Definitions
#include "f010s.h"                  // USER define FEE0S Application Definitions
#include "f010s_task.h"                  // USER define FEE0S Application Definitions
#endif //(BLE_APP_F010S)

#if (BLE_APP_F020S)
#include "app_f020.h"                  // USER define FEE0S Application Definitions
#include "f020s.h"                  // USER define FEE0S Application Definitions
#include "f020s_task.h"                  // USER define FEE0S Application Definitions
#endif //(BLE_APP_F020S)

#if (BLE_APP_F030S)
#include "app_f030.h"                  // USER define FEE0S Application Definitions
#include "f030s.h"                  // USER define FEE0S Application Definitions
#include "f030s_task.h"                  // USER define FEE0S Application Definitions
#endif //(BLE_APP_F030S)

#if (BLE_APP_F040S)
#include "app_f040.h"                  // USER define FEE0S Application Definitions
#include "f040s.h"                  // USER define FEE0S Application Definitions
#include "f040s_task.h"                  // USER define FEE0S Application Definitions
#endif //(BLE_APP_F040S)

#if (BLE_APP_FCC0S)
#include "app_fcc0.h"                  // USER define FCC0S Application Definitions
#endif //(BLE_APP_FCC0S)

#if(BLE_APP_ANCS)
#include "app_ancsc.h"               // Application ancs Definition
#endif
#if(BLE_APP_OADS)
#include "app_oads.h"                 // Application oads Definition
#endif

#if (BLE_APP_HT)
#include "app_ht.h"                  // Health Thermometer Application Definitions
#endif //(BLE_APP_HT)

#if (BLE_APP_DIS)
#include "app_dis.h"                 // Device Information Service Application Definitions
#endif //(BLE_APP_DIS)

#if (BLE_APP_BATT)
#include "app_batt.h"                // Battery Application Definitions
#endif //(BLE_APP_DIS)

#if (BLE_APP_HID)
#include "app_hid.h"                 // HID Application Definitions
#endif //(BLE_APP_HID)

#if (DISPLAY_SUPPORT)
#include "app_display.h"             // Application Display Definition
#endif //(DISPLAY_SUPPORT)

#if (BLE_APP_AM0)
#include "app_am0.h"                 // Audio Mode 0 Application
#endif //(BLE_APP_AM0)

#if (NVDS_SUPPORT)
#include "nvds.h"                    // NVDS Definitions
#endif //(NVDS_SUPPORT)
#include "flash.h"
#endif

#include "bass_task.h"
/*
 * DEFINES
 ****************************************************************************************
 */
void set_f001_0x2a9e_rd(uint8_t* buf);
void set_f002_0x2a9d_ind(uint8_t* buf);
void set_f011_0x2a9b_rd(uint8_t* buf);
void set_f012_0x2a9c_ind(uint8_t* buf);
void set_f021_0x2a85_rd(uint8_t* buf);
void set_f022_0x2a8c_rd(uint8_t* buf);
void set_f023_0x2a8e_rd(uint8_t* buf);
void set_f024_0x2a99_rd(uint8_t* buf);
void set_f024_0x2a99_ntf(uint8_t* buf);
void set_f025_0x2a9a_rd(uint8_t* buf);
void set_f026_0x2a9f_rd(uint8_t* buf);
void set_f026_0x2a9f_ntf(uint8_t* buf, uint8_t len);
void set_f031_0xfff1_rd(uint8_t* buf);
void set_f032_0xfff2_ntf(uint8_t* buf, uint8_t len);
void set_f032_0xfff2_rd(uint8_t* buf);
void set_f033_0xfff3_rd(uint8_t* buf);
void set_f034_0xfff4_ntf(uint8_t* buf, uint8_t len);
void set_f034_0xfff4_rd(uint8_t* buf);
void set_f035_0xfff5_rd(uint8_t* buf);
void set_f041_0x2a2b_rd_ntf(uint8_t* buf);
void set_bass_batt_0x2a19_rd(uint8_t batt);

//--------------------------------------


void xs_uart_send_data(uint8_t *buf, uint8_t len);
void send_user_list();
void get_scale_user();
void check_select_user();
void set_select_user_info_to_ble_chara();
void regist_user_on_scale(uint8_t pinL, uint8_t pinH);
void update_select_user_to_scale(uint8_t user_id);
void regist_user_check(uint8_t pinL, uint8_t pinH);
void delay_1s_send_time(int t);
void delay_1s_modify_user_birthday(int t);
void update_weight_body_measurement(uint8_t *buf);
void ji_huo_scale_and_send();
void delay_1s_send_resgist_ok(int t);
void delay_1s_send_take_measure(int t);
void delay_1s_updata_db(int t);
void delay_1s_get_user_info(int t);
uint8_t ignore_repetition(uint8_t *b);
void delay_1s_update_user_info(int t);
void delay_1s_regist_user_birthday(int t);
void init_select_user_info();
void delay_1s_to_loop_regist(int t);
void set_time(int y, int m, int d, int h, int mi, int s);
void update_time();//1s
void work_test(uint8_t *buf);
void re_set_time();


uint8_t g_user_num = 0;
//[0]=flag [1]=user id [2,3,4]=init [5,6]=year [7]=month [8]=day [9]=height [10]=sex [11]=active
//[12,13]=CONSENT [14,15]=weight/BF  [16,17]=DB
uint8_t g_user_list[18] = {0x00, 0x01, 0xFF, 0xFF, 0xFF, 0xC7, 0x07, 0x0A, 0x01, 0xA5, 0x01, 0x03, 0, 0, 0, 0, 0, 0};
uint8_t g_select_user_info[18] = {0x00, 0x01, 0xFF, 0xFF, 0xFF, 0xC7, 0x07, 0x0A, 0x01, 0xA5, 0x01, 0x03, 0, 0, 0, 0, 0, 0};
uint8_t g_user_select[3] = {0};
uint8_t g_user_select_index = 0xff;
uint8_t g_curr_time[8];
uint8_t g_time[10];
int g_sync_time_counter = 0;
uint8_t g_get_user_list = 0;
uint8_t g_measure_data[47] = {0};
uint8_t g_measure_data_len = 0;
uint8_t g_ble_state = 0;
uint8_t g_last_msg[8] = {0};
uint8_t g_scale_state = 1;
uint8_t g_last_msg_set = 0;
uint8_t g_rigist_user_id = 0;
uint8_t g_user_registing = 0;
uint8_t g_start_update_user_info_flag = 0;

uint8_t g_take_measure_rf_ok = 0;
uint8_t g_user_regist_rf_ok = 0;
uint8_t g_regist_10_buff[8] = {0};
uint8_t g_seting_time = 0;

uint32_t g_time_stamp = 0;
uint8_t  g_ba_counter=0;
//ble state
#define BLE_STATE_PIN 0x04

void xs_user_task()
{
	static int t = 0, inited = 0;

	if (inited == 0)
	{
		inited = 1;
	}

	if (t++ > 8)
	{
		t = 0;
		update_time();
	}
	delay_1s_send_time(0);
	delay_1s_modify_user_birthday(0);
	ji_huo_scale_and_send();
	delay_1s_send_resgist_ok(0);
	delay_1s_send_take_measure(0);
	delay_1s_updata_db(0);
	delay_1s_get_user_info(0);
	delay_1s_update_user_info(0);
	delay_1s_regist_user_birthday(0);
	delay_1s_to_loop_regist(0);

}
void init_work()
{

	gpio_config(BLE_STATE_PIN, OUTPUT, PULL_NONE);
	gpio_set(BLE_STATE_PIN, 1);

	UART_PRINTF("init_ble_state:1\n");

	set_time(21, 2, 2, 2, 2, 2);

}

// s=0 已连接   s=1 未连接
void set_ble_state(uint8_t s)
{
	gpio_set(BLE_STATE_PIN, s);

	g_ble_state = s;
	g_user_registing = 0;
	g_ba_counter=0;
	init_select_user_info();

	UART_PRINTF("icu_get_sleep_mode:%d\n",icu_get_sleep_mode());
	if (s == 0)
	{
		icu_set_sleep_mode(0);
	}
	else
	{
		icu_set_sleep_mode(2);
	}


}

//access----------------------------------------------------------------------------------------

void printf_hex(uint8_t *buf, uint8_t len)
{
	for (uint8_t i = 0; i < len; i++)
	{
		UART_PRINTF("%02X ", buf[i]);
	}
}
void xs_uart2_send_data(uint8_t *buf, uint8_t len)
{
	UART_PRINTF("uart2_tx:");
	printf_hex(buf, len);
	// uart2_send(buf,len);
}

void xs_uart2_received_isr(uint8_t *buf, uint8_t len)
{
	UART_PRINTF("uart2_rx:");
	printf_hex(buf, len);
	UART_PRINTF("\r\n");
}
void xs_uart_send_data(uint8_t *buf, uint8_t len)
{
	UART_PRINTF("uart0_tx:");
	printf_hex(buf, len);
	UART_PRINTF("\r\n");
	uart_send(buf, len);
	if (g_scale_state == 0)
	{
		g_last_msg_set = 1;
		memcpy(g_last_msg, buf, 8);
		UART_PRINTF("g_last_msg_set=1\n");
	}
}
void clear_uart_received_buf()
{
	uint8_t b[8] = {0};
	ignore_repetition(b);
}
uint8_t ignore_repetition(uint8_t *b)
{
	static uint8_t last_buf[8] = {0};
	

	if (last_buf[0] == b[0] &&
	        last_buf[1] == b[1] &&
	        last_buf[2] == b[2] &&
	        last_buf[3] == b[3] &&
	        last_buf[4] == b[4] &&
	        last_buf[5] == b[5] &&
	        last_buf[6] == b[6] &&
	        last_buf[7] == b[7] )
	{
		if(last_buf[6]==0xBA)
		{
			g_ba_counter++;
			if(g_ba_counter>30)
			{
				re_set_time();
				g_ba_counter=0;
			}
		}

		return 1;
	}
	else
	{
		last_buf[0] = b[0];
		last_buf[1] = b[1];
		last_buf[2] = b[2];
		last_buf[3] = b[3];
		last_buf[4] = b[4];
		last_buf[5] = b[5];
		last_buf[6] = b[6];
		last_buf[7] = b[7];
		return 0;
	}
}
uint8_t check_cmd_sum(uint8_t *buf)
{
	uint8_t sum=buf[2]+buf[3]+buf[4]+buf[5]+buf[6];
	if(sum==buf[7])
	{
		return 1;
	}
	else
	{
		return 1;
	}
}
void xs_uart_received_isr(uint8_t *buf, uint8_t len)
{
	UART_PRINTF("uart0_rx:");
	printf_hex(buf, len);
	UART_PRINTF("\r\n");
	// work_test(buf);
	if (ignore_repetition(buf))
	{
		return;
	}
	if (buf[0] == 0xFF && buf[1] == 0xA5 && check_cmd_sum(buf))
	{
		g_scale_state = 1;
		if (buf[6] == 0xBA)
		{
			uint8_t fff1_buf[8] = {0};
			struct bass_env_tag* bass_env = PRF_ENV_GET(BASS, bass);
			if (buf[2] > 0x1F)
			{
				buf[2] = 0x1F;
			}
			if (buf[2] < 0x15)
			{
				buf[2] = 0x15;
			}
			buf[2] = (buf[2] - 0x15) * 10;
			bass_env->batt_lvl[0] = buf[2];

			fff1_buf[4] = buf[3];
			fff1_buf[1] = buf[4] >> 1;

			set_f031_0xfff1_rd(fff1_buf);

		}
		else if (buf[6] == 0xAA)
		{
			if (g_user_select[0] != 0)
			{
				delay_1s_get_user_info(1);
			}
		}
		else if (buf[6] == 0xE2)
		{
			g_time[0] = buf[2];
			g_time[1] = 0x07;
			g_time[2] = buf[3];
			g_time[3] = buf[4];
		}
		else if (buf[6] == 0xE3)
		{
			g_time[4] = buf[2];
			g_time[5] = buf[3];
			g_time[6] = buf[4];
			set_time(g_time[0] | (g_time[1] << 8), g_time[2], g_time[3], g_time[4], g_time[5], g_time[6]);
		}
		else if (buf[6] == 0xBD)
		{
			g_scale_state = 0;
			UART_PRINTF("g_scale_state=0\n");
		}
		else if (buf[6] == 0xE0)
		{
			g_user_list[10] = (buf[2] == 1 ? 0 : 1);
			g_user_list[6] = 0x07;
			g_user_list[5] = buf[3];
			g_user_list[7] = buf[4];
			g_user_list[8] = buf[5];

		}
		else if (buf[6] == 0xE1)
		{
			g_user_list[9] = buf[2];
			g_user_list[11] = buf[5];
		}
		else if (buf[6] == 0xE4)
		{
			g_user_list[12] = buf[2];
			g_user_list[13] = buf[3];
			g_user_list[16] = buf[4];
			g_user_list[17] = buf[5];

		}
		else if (buf[6] == 0xE5)
		{
			uint8_t buff_2a9f[4];
			g_user_list[0] = 0;
			g_user_list[1] = buf[2];
			if (	g_user_list[1] == g_user_select[0])
			{
				set_select_user_info_to_ble_chara();
			}
			if (g_get_user_list == 1)
			{
				set_f032_0xfff2_ntf(g_user_list, F030_F032_DATA_LEN);
			}

			buff_2a9f[0] = 0x20;
			buff_2a9f[1] = 0x02;
			buff_2a9f[2] = 0x01;
			set_f026_0x2a9f_ntf(buff_2a9f, 3);

		}
		else if (buf[6] == 0xF0)
		{
			if (g_get_user_list == 1)
			{
				if (buf[2] > 0)
				{
					uint8_t user_finish_flag[1] = {1};
					set_f032_0xfff2_ntf(user_finish_flag, 1);
				}
				else
				{
					uint8_t no_user_flag[1] = {2};
					set_f032_0xfff2_ntf(no_user_flag, 1);
				}
				g_get_user_list = 0;
			}
			if (buf[2] == 0)
			{
				if (g_user_select[0] == g_user_list[1])
				{
					g_user_list[14] = buf[4];
					g_user_list[15] = buf[5];
					set_select_user_info_to_ble_chara();
				}
			}


		}
		else if (buf[6] == 0xE6)
		{
			if (g_user_select[0] == buf[2] &&
			        g_user_select[1] == buf[3] &&
			        g_user_select[2] == buf[4])
			{
				UART_PRINTF("CONSENT CODE OK!");
				delay_1s_get_user_info(10);
			}
			else
			{
				UART_PRINTF("CONSENT CODE ERROR!");
				init_select_user_info();
				uint8_t buff_2a9f[4];
				buff_2a9f[0] = 0x20;
				buff_2a9f[1] = 0x02;
				buff_2a9f[2] = 0x05;
				set_f026_0x2a9f_ntf(buff_2a9f, 3);
			}
		}
		else if (buf[2] == 0x10)
		{
			if (g_user_regist_rf_ok == 1)
			{
				g_user_regist_rf_ok = 0;
			}
			if (g_user_registing == 1)
			{
				delay_1s_regist_user_birthday(10);
			}
			else
			{
				delay_1s_modify_user_birthday(10);
			}
		}
		else if (buf[2] == 0x11)
		{
			g_user_list[1] = buf[3];
			if (g_user_registing == 1)
			{
				delay_1s_send_resgist_ok(10);
			}
			else
			{
				delay_1s_updata_db(10);
			}
		}
		else if (buf[2] == 0x32 && buf[6] == 0)
		{

		}
		else if (buf[2] == 0x30 && buf[6] == 0)
		{
			delay_1s_send_time(9);
		}
		else if (buf[2] == 0x60)
		{
			g_take_measure_rf_ok = 0;
		}

		g_measure_data_len = 0;

	}
	else if (buf[0] == 0xFA || g_measure_data_len>0)
	{
		memcpy(g_measure_data + g_measure_data_len, buf, len);
		g_measure_data_len += len;

		UART_PRINTF("g_measure_data_len:%d\n\r", g_measure_data_len);
		if (g_measure_data_len >= 47)
		{
			update_weight_body_measurement(g_measure_data);			
			for (uint8_t i = 0; i < 47; i++)
			{
				UART_PRINTF("%02X ", g_measure_data[i]);
			}
			g_measure_data_len = 0;
		}
	}
	else 
	{
	
	}

}


//Weight Scale Feature  0x2a9e
void set_f001_0x2a9e_rd(uint8_t* buf)
{
	struct f000s_env_tag* f000s_env = PRF_ENV_GET(F000S, f000s);
	memcpy(f000s_env->f001_val, buf, F000_F001_CHAR_DATA_LEN);
	UART_PRINTF("set_f001_0x2a9e_rd\r\n");
}
void set_f002_0x2a9d_ind(uint8_t* buf)
{
	extern void app_f002_send_ind(uint8_t conidx, uint16_t len, uint8_t* buf);

	struct f000s_env_tag* f000s_env = PRF_ENV_GET(F000S, f000s);
	memcpy(f000s_env->f002_val, buf, F000_F002_CHAR_DATA_LEN);

	app_f002_send_ind(0, F000_F002_CHAR_DATA_LEN, buf);
	UART_PRINTF("set_f002_0x2a9d_ind\r\n");
}


//Body Composition------------------------------------------------------------------------------

void set_f011_0x2a9b_rd(uint8_t* buf)
{
	struct f010s_env_tag* f010s_env = PRF_ENV_GET(F010S, f010s);
	memcpy(f010s_env->f011_val, buf, F010_F011_DATA_LEN);
}
void set_f012_0x2a9c_ind(uint8_t* buf)
{
	extern void app_f012_send_ind(uint8_t conidx, uint16_t len, uint8_t* buf);

	struct f010s_env_tag* f010s_env = PRF_ENV_GET(F010S, f010s);
	memcpy(f010s_env->f012_val, buf, F010_F012_DATA_LEN);

	app_f012_send_ind(0, F010_F012_DATA_LEN, buf);
	UART_PRINTF("set_f012_0x2a9c_ind\r\n");
}

//User Data --------------------------------------------------------------------------------------
//User Data - Date of Birth
void set_f021_0x2a85_rd(uint8_t* buf)
{
	struct f020s_env_tag* f020s_env = PRF_ENV_GET(F020S, f020s);
	memcpy(f020s_env->f021_val, buf, F020_F021_DATA_LEN);
}
void f021_0x2a85_cb(uint8_t* buf)
{
	UART_PRINTF("f021_0x2a85_cb\r\n");
	//{0x00,0x01,0xFF,0xFF,0xFF,0xC7,0x07,0x0A,0x01,0xA5,0x01,0x03,0,0};
	g_user_list[5] = buf[0];
	g_user_list[6] = buf[1];
	g_user_list[7] = buf[2];
	g_user_list[8] = buf[3];
}

//User Data - Gender
void set_f022_0x2a8c_rd(uint8_t* buf)
{
	struct f020s_env_tag* f020s_env = PRF_ENV_GET(F020S, f020s);
	memcpy(f020s_env->f022_val, buf, F020_F022_DATA_LEN);
}
void f022_0x2a8c_cb(uint8_t* buf)
{
	UART_PRINTF("f022_0x2a8c_cb\r\n");
	g_user_list[10] = buf[0];
}

//User Data - Height
void set_f023_0x2a8e_rd(uint8_t* buf)
{
	struct f020s_env_tag* f020s_env = PRF_ENV_GET(F020S, f020s);
	memcpy(f020s_env->f023_val, buf, F020_F023_DATA_LEN);
}
void f023_0x2a8e_cb(uint8_t* buf)
{
	UART_PRINTF("f023_0x2a8e_cb\r\n");
	g_user_list[9] = buf[0];
}
//User Data - Database Change Increment
void set_f024_0x2a99_rd(uint8_t* buf)
{
	struct f020s_env_tag* f020s_env = PRF_ENV_GET(F020S, f020s);
	memcpy(f020s_env->f024_val, buf, F020_F024_DATA_LEN);
}
void f024_0x2a99_cb(uint8_t* buf)
{
	UART_PRINTF("f024_0x2a99_cb\r\n");
	g_user_list[16] = buf[0];
	g_user_list[17] = buf[1];
	delay_1s_update_user_info(30);
	set_select_user_info_to_ble_chara();
}
void set_f024_0x2a99_ntf(uint8_t* buf)
{
	extern void app_f024_send_ntf(uint8_t conidx, uint16_t len, uint8_t* buf);

	struct f020s_env_tag* f020s_env = PRF_ENV_GET(F020S, f020s);
	memcpy(f020s_env->f024_val, buf, F020_F024_DATA_LEN);
	app_f024_send_ntf(0, F020_F024_DATA_LEN, buf);
	UART_PRINTF("set_f024_0x2a99_ntf\r\n");
}
//User Data - User Index
void set_f025_0x2a9a_rd(uint8_t* buf)
{
	struct f020s_env_tag* f020s_env = PRF_ENV_GET(F020S, f020s);
	memcpy(f020s_env->f025_val, buf, F020_F025_DATA_LEN);
}
void f025_0x2a9a_cb(uint8_t* buf)
{
	UART_PRINTF("f025_0x2a9a_cb\r\n");
}
//User Data - User Control Point
void set_f026_0x2a9f_rd(uint8_t* buf)
{
	struct f020s_env_tag* f020s_env = PRF_ENV_GET(F020S, f020s);
	memcpy(f020s_env->f026_val, buf, F020_F026_DATA_LEN);
}
void f026_0x2a9f_cb(uint8_t* buf)
{
	UART_PRINTF("f026_0x2a9f_cb\r\n");
	printf_hex(buf,4);
	//creat user 0x01 00 00
	uint8_t buff[8] = {0};
	if (buf[0] == 0x01)
	{
		g_user_registing = 1;
		g_user_regist_rf_ok = 1;
		buff[0] = 0xA5;
		buff[1] = 0x10;
		buff[2] = 0;
		buff[3] = buf[1];
		buff[4] = buf[2];
		buff[5] = 0xA5;
		buff[6] = 0x03;
		buff[7] = buff[1] + buff[2] + buff[3] + buff[4] + buff[5] + buff[6];
		xs_uart_send_data(buff, 8);
		memcpy(g_regist_10_buff, buff, 8);
		delay_1s_to_loop_regist(20);
		init_select_user_info();

	}
	else if (buf[0] == 0x02)
	{
		//A5 25 02 00 00 00 00 27
		init_select_user_info();
		g_user_select[0] = buf[1];
		g_user_select[1] = buf[2];
		g_user_select[2] = buf[3];

		clear_uart_received_buf();
		buff[0] = 0xA5;
		buff[1] = 0x25;
		buff[2] = g_user_select[0];
		buff[3] = 0;
		buff[4] = 0;
		buff[5] = 0;
		buff[6] = 0;
		buff[7] = buff[1] + buff[2] + buff[3] + buff[4] + buff[5] + buff[6];
		xs_uart_send_data(buff, 8);
	}
	else if (buf[0] == 0x03)
	{
		//A5 21 02 D2 04 00 00 F9
		buff[0] = 0xA5;
		buff[1] = 0x21;
		buff[2] = g_user_select[0];
		buff[3] = g_user_select[1];
		buff[4] = g_user_select[2];
		buff[5] = 0;
		buff[6] = 0;
		buff[7] = buff[1] + buff[2] + buff[3] + buff[4] + buff[5] + buff[6];
		xs_uart_send_data(buff, 8);
		init_select_user_info();

	}
}
void set_f026_0x2a9f_ntf(uint8_t* buf, uint8_t len)
{
	extern void app_f026_send_ind(uint8_t conidx, uint16_t len, uint8_t* buf);
	struct f020s_env_tag* f020s_env = PRF_ENV_GET(F020S, f020s);
	memcpy(f020s_env->f026_val, buf, len);
	app_f026_send_ind(0, len, buf);
	UART_PRINTF("set_f026_0x2a9f_ntf\r\n");
}

//Custom Service---------------------------------------------------------------------------------
//Custom Service - Scale Setting
void set_f031_0xfff1_rd(uint8_t* buf)
{
	struct f030s_env_tag* f030s_env = PRF_ENV_GET(F030S, f030s);
	memcpy(f030s_env->f031_val, buf, F030_F031_DATA_LEN);
}
void f031_0xfff1_cb(uint8_t* buf)
{
	UART_PRINTF("f031_0xfff1_cb\r\n");
	uint8_t buff[8] = {0};

	buff[0] = 0xA5;
	buff[1] = 0x33;
	buff[2] = 1 << buf[1];
	buff[3] = buf[4];
	buff[4] = 0;
	buff[5] = 0;
	buff[6] = 0;
	buff[7] = buff[1] + buff[2] + buff[3] + buff[4] + buff[5] + buff[6];
	xs_uart_send_data(buff, 8); //send date
}
//Custom Service - User list
void set_f032_0xfff2_rd(uint8_t* buf)
{
	struct f030s_env_tag* f030s_env = PRF_ENV_GET(F030S, f030s);
	memcpy(f030s_env->f032_val, buf, F030_F032_DATA_LEN);
}
void f032_0xfff2_cb(uint8_t* buf)
{
	static uint8_t index = 0;
	UART_PRINTF("f032_0xfff2_cb\r\n");
	if (buf[0] == 0)
	{
		g_get_user_list = 1;
		get_scale_user();
	}
	else if (buf[0] & 0x10)
	{
		uint8_t buff[8] = {0};

		buff[0] = 0xA5;
		buff[1] = 0x25;
		buff[2] = buf[0] & 0x0F;
		buff[3] = 0;
		buff[4] = 0;
		buff[5] = 0;
		buff[6] = 0;
		buff[7] = buff[1] + buff[2] + buff[3] + buff[4] + buff[5] + buff[6];
		xs_uart_send_data(buff, 8); //send date
	}
}
void set_f032_0xfff2_ntf(uint8_t* buf, uint8_t len)
{
	extern void app_f032_send_ntf(uint8_t conidx, uint16_t len, uint8_t* buf);

	app_f032_send_ntf(0, len, buf);
	UART_PRINTF("set_f032_0xfff2_ntf\r\n");
}
//Custom Service - Acitivity Level
void set_f033_0xfff3_rd(uint8_t* buf)
{
	struct f030s_env_tag* f030s_env = PRF_ENV_GET(F030S, f030s);
	memcpy(f030s_env->f033_val, buf, F030_F033_DATA_LEN);
}
void f033_0xfff3_cb(uint8_t* buf)
{
	UART_PRINTF("f033_0xfff3_cb\r\n");
	g_user_list[11] = buf[0];
}
//Custom Service - Take Measurement
void set_f034_0xfff4_rd(uint8_t* buf)
{
	struct f030s_env_tag* f030s_env = PRF_ENV_GET(F030S, f030s);
	memcpy(f030s_env->f034_val, buf, F030_F034_DATA_LEN);
}
void f034_0xfff4_cb(uint8_t* buf)
{
	UART_PRINTF("f034_0xfff4_cb\r\n");
	if (buf[0] == 0)
	{
		g_take_measure_rf_ok = 1;
		delay_1s_send_take_measure(1);
	}
}
void set_f034_0xfff4_ntf(uint8_t* buf, uint8_t len)
{
	extern void app_f034_send_ntf(uint8_t conidx, uint16_t len, uint8_t* buf);

	app_f034_send_ntf(0, len, buf);
	UART_PRINTF("set_f034_0xfff4_ntf\r\n");
}
//Custom Service - Refer Weight/BF
void set_f035_0xfff5_rd(uint8_t* buf)
{
	struct f030s_env_tag* f030s_env = PRF_ENV_GET(F030S, f030s);
	memcpy(f030s_env->f035_val, buf, F030_F035_DATA_LEN);
}
void f035_0xfff5_cb(uint8_t* buf)
{
	UART_PRINTF("f035_0xfff5_cb\r\n");
}

//Current Time ---------------------------------------------------------------------------------
void set_f041_0x2a2b_rd_ntf(uint8_t* buf)
{
	extern void app_f041_send_ntf(uint8_t conidx, uint16_t len, uint8_t* buf);

	//app_f041_send_ntf(0,F040_F041_DATA_LEN,buf);
	struct f040s_env_tag* f040s_env = PRF_ENV_GET(F040S, f040s);
	memcpy(f040s_env->f041_val, buf, F040_F041_DATA_LEN);
}
void f041_0x2a2b_cb(uint8_t* buf)
{
	UART_PRINTF("f041_0x2a2b_cb\r\n");
	set_f041_0x2a2b_rd_ntf(buf);
	uint8_t buff[8] = {0};
	memcpy(g_time, buf, 10);

	set_time(g_time[0] | (g_time[1] << 8), g_time[2], g_time[3], g_time[4], g_time[5], g_time[6]);

	buff[0] = 0xA5;
	buff[1] = 0x30;
	buff[2] = buf[0];
	buff[3] = buf[2];
	buff[4] = buf[3];
	buff[5] = 0;
	buff[6] = 0;
	buff[7] = buff[1] + buff[2] + buff[3] + buff[4] + buff[5] + buff[6];
	xs_uart_send_data(buff, 8); //send date

	g_curr_time[0] = 0xA5;
	g_curr_time[1] = 0x31;
	g_curr_time[2] = buf[4];
	g_curr_time[3] = buf[5];
	g_curr_time[4] = buf[6];
	g_curr_time[5] = 0;
	g_curr_time[6] = 0;
	g_curr_time[7] = g_curr_time[1] + g_curr_time[2] + g_curr_time[3] + g_curr_time[4] + g_curr_time[5] + g_curr_time[6];

}
// battery--------------------------------------------------------------------------------------
void set_bass_batt_0x2a19_rd(uint8_t batt)
{
	struct bass_env_tag* bass_env = PRF_ENV_GET(BASS, bass);
	bass_env->batt_lvl[0] = batt;
}
//function

void get_scale_user()
{
	uint8_t buff[8];
	g_user_num = 0;

	buff[0] = 0xA5;
	buff[1] = 0x23;
	buff[2] = 0;
	buff[3] = 0;
	buff[4] = 0;
	buff[5] = 0;
	buff[6] = 0;
	buff[7] = buff[1] + buff[2] + buff[3] + buff[4] + buff[5] + buff[6];
	xs_uart_send_data(buff, 8); //
}

void init_select_user_info()
{
	g_user_select[0] = 0;
	g_user_select[1] = 0;
	g_user_select[2] = 0;
	for (int i = 0; i < 18; i++)
	{
		g_user_list[i] = 0xFF;
	}
	set_select_user_info_to_ble_chara();
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
	uint8_t buff_2a9e[4] = {0xB7, 0, 0, 0};
	uint8_t buff_2a9b[4] = {0xCF, 0x31, 0, 0};

	buff_2a85[0] = g_user_list[5];
	buff_2a85[1] = g_user_list[6];
	buff_2a85[2] = g_user_list[7];
	buff_2a85[3] = g_user_list[8];

	buff_2a8c[0] = g_user_list[10];

	buff_2a8e[0] = g_user_list[9];

	buff_2a9a[0] = g_user_list[1];

	buff_fff4[0] = g_user_list[11];

	buff_fff5[0] = g_user_list[15];
	buff_fff5[1] = g_user_list[14];
	buff_fff5[2] = 0xFF;
	buff_fff5[3] = 0xFF;

	buff_2a99[0] = g_user_list[16];
	buff_2a99[1] = g_user_list[17];
	buff_2a99[2] = 0;
	buff_2a99[3] = 0;

	set_f021_0x2a85_rd(buff_2a85);
	set_f022_0x2a8c_rd(buff_2a8c);
	set_f023_0x2a8e_rd(buff_2a8e);
	set_f025_0x2a9a_rd(buff_2a9a);
	set_f024_0x2a99_rd(buff_2a99);
	set_f033_0xfff3_rd(buff_fff4);
	set_f035_0xfff5_rd(buff_fff5);

	set_f011_0x2a9b_rd(buff_2a9b);
	set_f001_0x2a9e_rd(buff_2a9e);
}


void delay_1s_send_time(int t)
{
	static int t1 = 0;
	if (t > 0)
	{
		t1 = t;
	}
	if (t1 > 0)
	{
		t1--;
		if (t1 == 0)
		{
			xs_uart_send_data(g_curr_time, 8); //send time
		}
	}
}
void delay_1s_to_loop_regist(int t)
{
	static int t1 = 0, times = 0;
	if (t > 0)
	{
		t1 = t;
		times = 0;
	}
	if (t1 > 0)
	{
		t1--;
		if (t1 == 0)
		{
			if (g_user_regist_rf_ok == 1 && times++ < 5)
			{
				xs_uart_send_data(g_regist_10_buff, 8);
				t1 = 20;
			}
			else {
				times = 0;
			}

		}
	}
}

void delay_1s_regist_user_birthday(int t)
{
	static int t1 = 0;
	if (t > 0)
	{
		t1 = t;
	}
	if (t1 > 0)
	{
		t1--;
		if (t1 == 0)
		{
			//A5 11 C7 0A 01 00 00 E3
			uint8_t buff[8];
			buff[0] = 0xA5;
			buff[1] = 0x11;
			buff[2] = 0xC7;
			buff[3] = 0x0A;
			buff[4] = 0x01;
			buff[5] = 0;
			buff[6] = 0;
			buff[7] = buff[1] + buff[2] + buff[3] + buff[4] + buff[5] + buff[6];
			xs_uart_send_data(buff, 8);
		}
	}
}

void delay_1s_modify_user_birthday(int t)
{
	static int t1 = 0;
	if (t > 0)
	{
		t1 = t;
	}
	if (t1 > 0)
	{
		t1--;
		if (t1 == 0)
		{
			//A5 11 C7 0A 01 00 00 E3
			uint8_t buff[8];
			buff[0] = 0xA5;
			buff[1] = 0x11;
			buff[2] = g_user_list[5];
			buff[3] = g_user_list[7];
			buff[4] = g_user_list[8];
			buff[5] = 0;
			buff[6] = 0;
			buff[7] = buff[1] + buff[2] + buff[3] + buff[4] + buff[5] + buff[6];
			xs_uart_send_data(buff, 8);
		}
	}
}
void delay_1s_updata_db(int t)
{
	static int t1 = 0;
	if (t > 0)
	{
		t1 = t;
	}
	if (t1 > 0)
	{
		t1--;
		if (t1 == 0)
		{
			//A5 32 08 00 00 00 00
			uint8_t buff[8];
			buff[0] = 0xA5;
			buff[1] = 0x32;
			buff[2] = g_user_list[16];
			buff[3] = g_user_list[17];
			buff[4] = 0;
			buff[5] = 0;
			buff[6] = 0;
			buff[7] = buff[1] + buff[2] + buff[3] + buff[4] + buff[5] + buff[6];
			xs_uart_send_data(buff, 8);
			delay_1s_get_user_info(20);
		}
	}
}

void delay_1s_update_user_info(int t)
{
	static int t1 = 0;
	if (t > 0)
	{
		t1 = t;
	}
	if (t1 > 0)
	{
		t1--;
		if (t1 == 0)
		{
			if(g_user_list[13] ==0xff && g_user_list[12]==0xff)
			{
				return;
			}
			else
			{
				uint8_t buff[8];
				buff[0] = 0xA5;
				buff[1] = 0x10;
				buff[2] = ((g_user_list[10] == 1 ? 0 : 1) << 7) | g_user_list[1];
				buff[3] = g_user_list[13];
				buff[4] = g_user_list[12];
				buff[5] = g_user_list[9];
				buff[6] = g_user_list[11];
				buff[7] = buff[1] + buff[2] + buff[3] + buff[4] + buff[5] + buff[6];
				xs_uart_send_data(buff, 8);
			}
			
		}
	}

}

void update_weight_body_measurement(uint8_t *buf)
{
	static uint8_t last_time[3] = {0, 0, 0};
	uint8_t weight_m[15] = {0};
	uint8_t body_m[14] = {0};
	uint8_t rf_m[1] = {1};
	int bmr_tmep = 0;
	int water_tmep = 0;

	//Flags	Weight		Year		Month	Day	Hour	Min	Second	User ID	BMI		Height

	weight_m[0] = 0x0E;
	weight_m[1] = ((buf[15] | (buf[16] << 8)) * 2) & 0xff;
	weight_m[2] = ((buf[15] | (buf[16] << 8)) * 2) >> 8;
	weight_m[3] = buf[41];
	weight_m[4] = 0x07;
	weight_m[5] = buf[40];
	weight_m[6] = buf[39];
	weight_m[7] = buf[42];
	weight_m[8] = buf[43];
	weight_m[9] = buf[44];
	weight_m[10] = buf[1];
	weight_m[11] = buf[19];
	weight_m[12] = buf[20];
	weight_m[13] = (buf[10] * 10) & 0xFF;
	weight_m[14] = (buf[10] * 10) >> 8;
	set_f002_0x2a9d_ind(weight_m);
	//Flags		Body Fat %		BMR		Muscle %		Soft Lean Mass		Body Water mass		Impedance

	body_m[0] = 0x98;
	body_m[1] = 0x03;
	body_m[2] = buf[21];
	body_m[3] = buf[22];

	bmr_tmep = (buf[29] | (buf[30] << 8)) * 4186 / 1000;
	// body_m[4]=((buf[29]|(buf[30]<<8))*4)&0xff;
	// body_m[5]=((buf[29]|(buf[30]<<8))*4)>>8;
	body_m[4] = bmr_tmep & 0xff;
	body_m[5] = bmr_tmep >> 8;

	body_m[6] = buf[25];
	body_m[7] = buf[26];
	body_m[8] = buf[35];
	body_m[9] = buf[36];
	// body_m[10]=((buf[23]|(buf[24]<<8))*14)&0xff;
	// body_m[11]=((buf[23]|(buf[24]<<8))*14)>>8;
	//
	water_tmep = ((buf[23] | (buf[24] << 8)) * (buf[15] | (buf[16] << 8)) * 2) / 1000;
	body_m[10] = water_tmep & 0xff;
	body_m[11] = water_tmep >> 8;

	body_m[12] = buf[33];
	body_m[13] = buf[34];

	if (last_time[0] == weight_m[7] && last_time[1] == weight_m[8] && last_time[2] == weight_m[9])
	{
		//duplicate
	}
	else
	{
		last_time[0] = weight_m[7];
		last_time[1] = weight_m[8];
		last_time[2] = weight_m[9];
		set_f012_0x2a9c_ind(body_m);
		set_f034_0xfff4_ntf(rf_m, 1);
	}

}


void ji_huo_scale_and_send()
{
	if (g_last_msg_set == 1 && g_scale_state == 1 )
	{
		UART_PRINTF("g_last_msg_set==1 && g_scale_state==1\n");
		xs_uart_send_data(g_last_msg, 8); //
		g_last_msg_set = 0;
	}
}

void delay_1s_send_resgist_ok(int t)
{
	static int t1 = 0;
	if (t > 0)
	{
		t1 = t;
	}
	if (t1 > 0)
	{
		t1--;
		if (t1 == 0)
		{
			uint8_t buff_2a9f[4];
			g_user_registing = 0;
			buff_2a9f[0] = 0x20;
			buff_2a9f[1] = 0x01;
			buff_2a9f[2] = 0x01;
			buff_2a9f[3] = g_user_list[1];
			set_f026_0x2a9f_ntf(buff_2a9f, 4);
		}
	}
}

void delay_1s_send_take_measure(int t)
{
	static int t1 = 0, times = 0;
	if (t > 0)
	{
		t1 = t;
		times = 0;
	}
	if (t1 > 0)
	{
		t1--;
		if (t1 == 0)
		{
			//A5 60 03 00 00 00 00 60
			if (g_take_measure_rf_ok == 1 && times++ < 5)
			{
				uint8_t buff[8];
				buff[0] = 0xA5;
				buff[1] = 0x60;
				buff[2] = g_user_select[0];
				buff[3] = 0;
				buff[4] = 0;
				buff[5] = 0;
				buff[6] = 0;
				buff[7] = buff[1] + buff[2] + buff[3] + buff[4] + buff[5] + buff[6];
				xs_uart_send_data(buff, 8); //
				t1 = 20;
			}
			else
			{
				times = 0;
			}

		}
	}
}

void delay_1s_get_user_info(int t)
{
	static int t1 = 0;
	if (t > 0)
	{
		t1 = t;
	}
	if (t1 > 0)
	{
		t1--;
		if (t1 == 0)
		{
			uint8_t buff[8];

			buff[0] = 0xA5;
			buff[1] = 0x20;
			buff[2] = g_user_select[0];
			buff[3] = 0;
			buff[4] = 0;
			buff[5] = 0;
			buff[6] = 0;
			buff[7] = buff[1] + buff[2] + buff[3] + buff[4] + buff[5] + buff[6];
			xs_uart_send_data(buff, 8);
		}
	}
}

void set_time(int y, int m, int d, int h, int mi, int s)
{
	uint32_t time_stamp = 0;
	struct tm tm0 = {0};

	tm0.tm_year = (y>1900?y:2021) - 1900;
	tm0.tm_mon = m;
	tm0.tm_mday = d;
	tm0.tm_hour = h;
	tm0.tm_min = mi;
	tm0.tm_sec = s;

	time_stamp = mktime(&tm0);

	if(time_stamp>1609652405)
	{
		g_time_stamp = time_stamp;
	}
	
}

void re_set_time()
{
	uint8_t buff[8] = {0};

	buff[0] = 0xA5;
	buff[1] = 0x30;
	buff[2] = g_time[0];
	buff[3] = g_time[2];
	buff[4] = g_time[3];
	buff[5] = 0;
	buff[6] = 0;
	buff[7] = buff[1] + buff[2] + buff[3] + buff[4] + buff[5] + buff[6];
	xs_uart_send_data(buff, 8); //send date

	g_curr_time[0] = 0xA5;
	g_curr_time[1] = 0x31;
	g_curr_time[2] = g_time[4];
	g_curr_time[3] = g_time[5];
	g_curr_time[4] = g_time[6];
	g_curr_time[5] = 0;
	g_curr_time[6] = 0;
	g_curr_time[7] = g_curr_time[1] + g_curr_time[2] + g_curr_time[3] + g_curr_time[4] + g_curr_time[5] + g_curr_time[6];	
	
}
void update_time()//1s
{

	struct tm *tm1=0;

	g_time_stamp++;

	tm1=localtime(&g_time_stamp);
	
	g_time[0]=(tm1->tm_year+1900)&0xff;
	g_time[1]=0x07;
	g_time[2]=tm1->tm_mon;
	g_time[3]=tm1->tm_mday;
	g_time[4]=tm1->tm_hour;
	g_time[5]=tm1->tm_min;
	g_time[6]=tm1->tm_sec;
	set_f041_0x2a2b_rd_ntf(g_time);
	// UART_PRINTF("%d-%d-%d  %d:%d:%d  %d",
	// 		tm1->tm_year,tm1->tm_mon,tm1->tm_mday,
	// 		tm1->tm_hour,tm1->tm_min,tm1->tm_sec,g_time_stamp);

}

void work_test(uint8_t *buf)
{
	if(buf[0]=='0')
	{
		uint8_t buff[15]={1,2,3};

		set_f002_0x2a9d_ind(buff);
		set_f012_0x2a9c_ind(buff);
	}
}