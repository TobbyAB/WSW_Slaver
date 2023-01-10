/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-11-25     Rick       the first version
 */
#include <rtthread.h>
#include <stdint.h>
#include "pin_config.h"
#include "agile_button.h"

#define DBG_TAG "key"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

static agile_btn_t *test_btn = RT_NULL;
uint8_t Long_Flag = 0;
extern uint32_t Target_ID;
void test_single_callback(agile_btn_t *btn)
{
    if (Long_Flag)
    {
        Long_Flag = 0;
    }
    else
    {
        //SW_led(3);
        LOG_I("heart single\r\n");
    }
}

void test_hold_callback(agile_btn_t *btn)
{
    SW_led(4); //快闪
    Long_Flag = 1;
    rf_433_Enqueue(99999999, 4, 0);
    LOG_I("Send pairing message \r\n");
}


void button_init(void)
{
    test_btn = agile_btn_create(KEY1, PIN_LOW, PIN_MODE_INPUT_PULLUP);
    agile_btn_set_elimination_time(test_btn, 50);
    agile_btn_set_hold_cycle_time(test_btn, 3000);
    agile_btn_set_event_cb(test_btn, BTN_CLICK_EVENT, test_single_callback);
    agile_btn_set_event_cb(test_btn, BTN_HOLD_EVENT, test_hold_callback);
    agile_btn_start(test_btn);
    LOG_I("button_init Success\r\n");
}
