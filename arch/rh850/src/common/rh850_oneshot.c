/****************************************************************************
 * arch/rh850/src/common/rh850_oneshot.c
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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/arch.h>
#include <nuttx/irq.h>
#include <nuttx/config.h>

#include "rh850_oneshot.h"

/****************************************************************************
 * Private Functions
 ****************************************************************************/

void rh850_ostm_enable(volatile struct rh850_OSTMn *timer)
{
    // Set to Free-run compare mode
    timer->OSTMnCTL |= OSTM_MODE_FREE_RUN;

    // Disable interrupts when counting starts, we don't need it
    timer->OSTMnCTL &= OSTM_CTL_CLEAR_MD0;

    // Set OSTMnMD2 to 0, it will reset CNT to 0x0 when start.
    timer->OSTMnCTL &= OSTM_CTL_CLEAR_MD2;

    // Enable interrupt
    timer->OSTMnCTL |= OSTM_CTL_INT_ENABLE;
}

uint32_t rh850_ostm_get(volatile struct rh850_OSTMn *timer)
{
    return timer->OSTMnCNT;
}

void rh850_ostm_set(volatile struct rh850_OSTMn *timer, uint32_t compare)
{
    // Stop
    timer->OSTMnTT = 0x01;
    // Reset CMP, CNT will reset to 0
    timer->OSTMnCMP = compare;
    // Start
    timer->OSTMnTS |= 0x01;
}

void rh850_ostm_cancel(volatile struct rh850_OSTMn *timer)
{
    // Stop
    timer->OSTMnTT = 0x01;
}
