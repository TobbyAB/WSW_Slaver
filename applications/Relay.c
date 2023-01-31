/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-11-03     Tobby       the first version
 */
#include "rtthread.h"
#include "rtdevice.h"
#include "pin_config.h"
#include "led.h"
#include "radio_encoder.h"
#include <stdio.h>

#define DBG_TAG "relay"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>


uint8_t Relay1_Status = 0, Relay2_Status = 0;

void Relay_Init(void)
{
    rt_pin_mode(RELAY1, PIN_MODE_OUTPUT);
    rt_pin_mode(RELAY2, PIN_MODE_OUTPUT);
    rt_pin_write(RELAY1, 0);
    rt_pin_write(RELAY2, 0);
    LOG_I("Relay_Init Success\r\n");
}

void Relay1_Open(void)
{
    if (Relay1_Status == 0)
    {
        Relay1_Status = 1;
        rt_pin_write(RELAY1, 1);
        LOG_I("Relay1 to Open \r\n");
    }
}
MSH_CMD_EXPORT(Relay1_Open, Relay1_Open);

void Relay1_Close(void)
{
    if (Relay1_Status == 1)
    {
        Relay1_Status = 0;
        rt_pin_write(RELAY1, 0);
        LOG_I("Relay1 to Close \r\n");
    }
}
MSH_CMD_EXPORT(Relay1_Close, Relay1_Close);
void Relay2_Open(void)
{
    if (Relay2_Status == 0)
    {
        Relay2_Status = 1;
        rt_pin_write(RELAY2, 1);
        LOG_I("Relay2 to Open \r\n");
    }
}

void Relay2_Close(void)
{
    if (Relay2_Status == 0)
    {
        Relay2_Status = 1;
        rt_pin_write(RELAY2, 0);
        LOG_I("Relay2 to Close \r\n");
    }
}
