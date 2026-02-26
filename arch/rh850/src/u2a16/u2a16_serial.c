/****************************************************************************
 * arch/rh850/src/u2a16/u2a16_serial.c
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

#include <nuttx/config.h>

#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <debug.h>

#include <nuttx/irq.h>
#include <nuttx/arch.h>
#include <nuttx/serial/serial.h>
#include <nuttx/serial/uart_renesas_rlin3.h>

#include <arch/board/board.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define PLAT_UART_ADDR (0xFFC7C100) // RLIN35 Base

//#define OR1K_SYS_CLK (20000000)
//#define OR1K_BAUD (115200)
//#define OR1K_DIVISOR (OR1K_SYS_CLK / (16*OR1K_BAUD))

/****************************************************************************
 * Private Data
 ****************************************************************************/

//#ifdef HAVE_SERIAL_CONSOLE
//static spinlock_t g_serial_lock = SP_UNLOCKED;
//#endif
static volatile struct renesas_rlin3 *uart = (void*)PLAT_UART_ADDR;

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: u2a16_earlyserialinit
 *
 * Description:
 *
 ****************************************************************************/

void u2a16_earlyserialinit(void)
{
  renesas_rlin3_earlyserialinit(uart);
}

/****************************************************************************
 * Name: u2a16_serialinit
 *
 * Description:
 *   Register serial console and serial ports.
 *
 ****************************************************************************/

#ifdef USE_SERIALDRIVER
void u2a16_serialinit(void)
{
  /* Register the console */

  /* Register all USARTs */
}
#endif

/****************************************************************************
 * Name: up_putc
 *
 * Description:
 *   Provide priority, low-level access to support OS debug  writes
 *
 ****************************************************************************/

void up_putc(int ch)
{
  renesas_rlin3_putc(uart, ch);
}
