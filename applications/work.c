/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-21     Rick       the first version
 */
#include <rtthread.h>
#include <rtdevice.h>
#include <stdio.h>
#include "Radio_Encoder.h"
#include "pin_config.h"

#define DBG_TAG "work"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

rt_thread_t work_t = RT_NULL;
rt_timer_t Heart_timer = RT_NULL;
rt_sem_t Heart_sem = RT_NULL;


uint8_t psi_lost=0;

extern uint32_t Target_ID;

void remote_control(void)
{
    rt_sem_release(Heart_sem);
    if(psi_lost)
    {
        psi_lost = 0;
        SW_led(2);
        return;
    }
}
void Heart_timeout(void *parameter)
{
    LOG_D("Loss of heartbeat");
    psi_lost = 1;
    SW_led(1);
    Relay1_Close();
    Relay2_Close();
}

void work_callback(void *parameter)
{
    uint32_t time;
    while(1)
    {
        if(rt_sem_take(Heart_sem,RT_WAITING_FOREVER) == RT_EOK)
        {
            time = 1*60*1000;
            rt_timer_control(Heart_timer, RT_TIMER_CTRL_SET_TIME, &time);
            rt_timer_start(Heart_timer);
        }
    }
}
void work_init(void)
{
    Heart_sem = rt_sem_create("Heart_refresh", 0, RT_IPC_FLAG_FIFO);
    Heart_timer = rt_timer_create("Heart_timeout", Heart_timeout, RT_NULL, 1*60*1000, RT_TIMER_FLAG_ONE_SHOT|RT_TIMER_FLAG_SOFT_TIMER);
    work_t = rt_thread_create("work", work_callback, RT_NULL, 1024, 10, 10);
    if(work_t  != RT_NULL)
    {
        rt_thread_startup(work_t);
    }
}
