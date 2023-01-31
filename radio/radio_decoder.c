/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-11-22     Rick       the first version
 */
#include <rtthread.h>
#include <stdio.h>
#include <work.h>
#include "radio_app.h"
#include "radio_decoder.h"

#define DBG_TAG "RADIO_DECODER"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

extern uint32_t Self_Id;
extern rf_info info_433;
extern uint32_t Target_ID;

void Solve_433(int rssi, uint8_t *rx_buffer, uint8_t rx_len)
{
    Message Rx_message;
    LOG_D("Solve initial letter is %c\r\n",rx_buffer[1]);
    if (rx_buffer[1] == 'M')
    {
        sscanf((const char *) &rx_buffer[1], "Master{%08ld,%08ld,%02d,%02d,XXX}", &Rx_message.From_ID,
                &Rx_message.Target_ID, &Rx_message.Command, &Rx_message.Data);
        LOG_D("Target_ID  is %08ld \r\n",Rx_message.Target_ID);
        if (Rx_message.Target_ID == Self_Id && Rx_message.From_ID == Target_ID)
        {
            remote_control();
            switch (Rx_message.Command)
            {
            case 0: //心跳
                LOG_I("RF 433 heart is received from Master\r\n");
                SW_led(3);
                rf_433_Enqueue(Target_ID, 0, 1);
                switch (Rx_message.Data)
                {
                case 0:
                    Relay1_Close();
                    Relay2_Close();
                    break;
                case 1:
                    Relay1_Open();
                    Relay2_Close();
                    break;
                case 2:
                    Relay1_Close();
                    Relay2_Open();
                    break;
                case 3:
                    Relay1_Open();
                    Relay2_Open();
                    break;
                default:
                    break;
                }
                break;
            case 1: //按钮
                LOG_I("RF 433 key is received from Master \r\n");
                SW_led(3);
                rf_433_Enqueue(Target_ID, 1, 1);
                break;
            case 2: //SWITCH1
                LOG_I("RF 433 SWITCH1 Control %d is received from Master\r\n", Rx_message.Data);
                SW_led(3);
                if (Rx_message.Data)
                {
                    Relay1_Open();
                    rf_433_Urgent_Enqueue(Target_ID, 2, 1);
                }
                else
                {
                    Relay1_Close();
                    rf_433_Urgent_Enqueue(Target_ID, 2, 0);
                }
                break;
            case 3: //SWITCH2
                LOG_I("RF 433 SWITCH2 Control %d is received from Master\r\n", Rx_message.Data);
                SW_led(3);
                if (Rx_message.Data)
                {
                    Relay2_Open();
                    rf_433_Urgent_Enqueue(Target_ID, 3, 1);
                }
                else
                {
                    Relay2_Close();
                    rf_433_Urgent_Enqueue(Target_ID, 3, 0);
                }
                break;
            case 4: //Learn
                if (Rx_message.Data)
                {
                    Flash_Master_Change(Rx_message.From_ID);
                    Target_ID = Flash_Get_Master();
                    transmitter_on();
                    rt_thread_mdelay(100);
                    SW_led(1);
                    LOG_I("Successful pairing, Master ID of the slaver is %d\r\n", Rx_message.Data);
                }
                break;
            default:
                break;
            }
        }
    }
}

void rf433_rx_callback(int rssi, uint8_t *rx_buffer, uint8_t rx_len)
{
    switch (rx_buffer[1])
    {
    case 'M':
        Solve_433(rssi, rx_buffer, rx_len);
        break;
    }
}

