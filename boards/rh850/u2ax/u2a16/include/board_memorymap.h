/****************************************************************************
 * boards/rh850/u2a16/include/board_memorymap.h
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

#ifndef __BOARDS_RH850_U2A16_INCLUDE_BOARD_MEMORYMAP_H
#define __BOARDS_RH850_U2A16_INCLUDE_BOARD_MEMORYMAP_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* DATA_FLASH */

#define DATA_FLASH_BASE   0x400000
#define DATA_FLASH_SIZE   0x10000

/* CODE_FLASH */

#define CODE_FLASH_BASE   0x410000
#define CODE_FLASH_SIZE   0x3F0000

/* RAM0 */

#define RAM_BASE   0xFE100000
#define RAM_SIZE   0x80000

/****************************************************************************
 * Public Data
 ****************************************************************************/

/* Kernel code memory (RX)  */

// extern uint8_t          __kflash_start[];
// extern uint8_t          __kflash_size[];

#endif /* __BOARDS_RH850_U2A16_INCLUDE_BOARD_MEMORYMAP_H */