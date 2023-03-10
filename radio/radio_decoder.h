/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-11-22     Rick       the first version
 */
#ifndef RADIO_RADIO_DECODER_H_
#define RADIO_RADIO_DECODER_H_

#include "rtthread.h"

typedef struct
{
    long Target_ID;
    long From_ID;
    int Counter;
    int Command;
    int Data;
    int Rssi;
}Message;

#endif /* RADIO_RADIO_ENCODER_H_ */
