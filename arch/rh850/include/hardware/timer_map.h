/****************************************************************************
 * arch/rh850/include/hardware/timer_map.h
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

#ifndef __ARCH_RH850_INCLUDE_HARDWARE_TIMER_MAP_H
#define __ARCH_RH850_INCLUDE_HARDWARE_TIMER_MAP_H

/****************************************************************************
 * Included Files
 ****************************************************************************/


/****************************************************************************
 * Private Data
 ****************************************************************************/

static const uint32_t OSTM_BASE[] = {
    0xFFBF0000, // OSTM0_base
    0xFFBF0100, // OSTM1_base
    0xFFBF0200, // OSTM2_base
    0xFFBF0300, // OSTM3_base
    0xFFBF0400, // OSTM4_base
    0xFFBF0500, // OSTM5_base
    0xFFBF0600, // OSTM6_base
    0xFFBF0700, // OSTM7_base
    0xFFBF0800, // OSTM8_base
    0xFFBF0900  // OSTM9_base
};

static const uint16_t OSTM_IRQ[] = {
    199,    // OSTM0 interrupt number
    200,    // OSTM1 interrupt number
    201,    // OSTM2 interrupt number
    202,    // OSTM3 interrupt number
    203,    // OSTM4 interrupt number
    204,    // OSTM5 interrupt number
    205,    // OSTM6 interrupt number
    206,    // OSTM7 interrupt number
    207,    // OSTM8 interrupt number
    208     // OSTM9 interrupt number
};

#endif /* __ARCH_RH850_INCLUDE_HARDWARE_TIMER_MAP_H */
