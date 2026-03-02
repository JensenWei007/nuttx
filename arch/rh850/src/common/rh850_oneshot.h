/****************************************************************************
 * arch/rh850/src/common/rh850_oneshot.h
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

#ifndef __ARCH_RH850_SRC_COMMON_RH850_ONESHOT_H
#define __ARCH_RH850_SRC_COMMON_RH850_ONESHOT_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdint.h>

#include <nuttx/config.h>
#include <nuttx/timers/oneshot.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define OSTM_CTL_CLEAR_MD0    (0b11111110)
#define OSTM_CTL_CLEAR_MD2    (0b11111011)
#define OSTM_CTL_INT_ENABLE   (0b10000000)

#define OSTM_MODE_FREE_RUN    (0b00000010)
#define OSTM_MODE_INTERVAL    (0b11111101)

#define TIMER_FREQ CONFIG_OSTM_FREQ

#define TIME_US(us) (((TIMER_FREQ)*(us))/(1000000ull))
#define TIME_MS(ms) (TIME_US((ms)*1000ull))
#define TIME_S(s)   (TIME_MS((s)*1000ull))

#define CYCLES_TO_US(cycles) (((cycles) * 1000000ull) / (TIMER_FREQ))
#define CYCLES_TO_MS(cycles) (((cycles) * 1000ull) / (TIMER_FREQ))
#define CYCLES_TO_S(cycles)  ((cycles) / (TIMER_FREQ))

/****************************************************************************
 * Public Types
 ****************************************************************************/

struct rh850_OSTMn {
    volatile uint32_t   OSTMnCMP;       // 0x00
    volatile uint32_t   OSTMnCNT;       // 0x04
    volatile uint8_t    OSTMnTO;        // 0x08
    uint8_t             PAD0[3];
    volatile uint8_t    OSTMnTOE;       // 0x0C
    uint8_t             PAD1[3];
    volatile uint8_t    OSTMnTE;        // 0x10
    uint8_t             PAD2[3];
    volatile uint8_t    OSTMnTS;        // 0x14
    uint8_t             PAD3[3];
    volatile uint8_t    OSTMnTT;        // 0x18
    uint8_t             PAD4[7];
    volatile uint8_t    OSTMnCTL;       // 0x20

    volatile uint16_t   IC0CKSEL8;
    volatile uint16_t   IC0CKSEL9;
};

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

void rh850_ostm_enable(volatile struct rh850_OSTMn *timer);

uint32_t rh850_ostm_get(volatile struct rh850_OSTMn *timer);

void rh850_ostm_set(volatile struct rh850_OSTMn *timer, uint32_t compare);

void rh850_ostm_cancel(volatile struct rh850_OSTMn *timer);

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

struct oneshot_lowerhalf_s *
rh850_oneshot_initialize(int index, uint32_t freq);

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif /* __ARCH_RH850_SRC_COMMON_RH850_ONESHOT_H */