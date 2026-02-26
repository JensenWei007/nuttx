/****************************************************************************
 * drivers/serial/uart_renesas_rlin3.c
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
#include <assert.h>
#include <errno.h>
#include <debug.h>

#include <nuttx/serial/uart_renesas_rlin3.h>

#ifdef CONFIG_RENESAS_RLIN3_UART

/****************************************************************************
 * Private Data
 ****************************************************************************/

volatile unsigned int uart_rxcnt = 0;

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: renesas_rlin3_earlyserialinit
 *
 * Description:
 *   Performs the low level UART initialization early in debug so that the
 *   serial console will be available during boot up.  This must be called
 *   before uart_serialinit.
 *
 *   NOTE: Configuration of the CONSOLE UART was performed by uart_lowsetup()
 *   very early in the boot sequence.
 *
 ****************************************************************************/

void renesas_rlin3_earlyserialinit(FAR struct renesas_rlin3 *uart) {
    *((volatile uint32_t*) MSRKCPROT) = KCPROT_ENABLE;
    *((volatile uint32_t*) MSR_RLIN3) = 0;
    *((volatile uint32_t*) MSRKCPROT) = KCPROT_DISABLE;

    // Set reset mode
    uart->RLN3nLCUC = 0;

    while ((uart->RLN3nLMST & RLIN3_LMST_OMM0_MSK) != 0x0) {
    }

    // Set baud rate to 500000 assuming CLK_RLIN = 80 MHz
    uart->RLN3nLWBR = 0; // prescaler reset;
    uart->RLN3nLWBR = RLIN3_LWBR_LPRS_16 | RLIN3_LWBR_NSPB_10;
    uart->RLN3nLBRP01 = 0;

    // Set data format
    uart->RLN3nLBFC = 
        RLN3_LBFC_UBLS_8B |
        RLN3_LBFC_UBOS_LSB |
        RLN3_LBFC_USBLS_1B |
        RLN3_LBFC_UPS_DIS;

    // Set uart mode
    uart->RLN3nLMD = RLN3_LMD_UART_MODE; 
    uart->RLN3nLCUC = RLN3_LCUC_LIN_CANC;

    while ((uart->RLN3nLMST & RLIN3_LMST_OMM0_MSK) != 0x1) {
    }

    // Enable reception and transmission
    uart->RLN3nLUOER = RLN3_LUOER_UROE | RLN3_LUOER_UTOE;
}

/****************************************************************************
 * Name: renesas_rlin3_serialinit
 *
 * Description:
 *   Register serial console and serial ports.  This assumes that
 *   renesas_rlin3_earlyserialinit was called previously.
 *
 ****************************************************************************/

void renesas_rlin3_serialinit(void) {

}

/****************************************************************************
 * Name: renesas_rlin3_interrupt
 *
 * Description:
 *   Handle UART renesas_rlin3 interrupt.
 *
 ****************************************************************************/

int renesas_rlin3_interrupt(int irq, FAR void *context, FAR void *arg) {
    return 0;
}

/****************************************************************************
 * Name: renesas_rlin3_putc
 *
 * Description:
 *   Write one character to the UART (polled)
 *
 ****************************************************************************/

void renesas_rlin3_putc(FAR struct renesas_rlin3 *uart, int c) {
    while (uart->RLN3nLST & RLN3_LST_UTS_MSK);
    uart->RLN3nLUTDR = c;
}

#endif /* CONFIG_RENESAS_RLIN3_UART */
