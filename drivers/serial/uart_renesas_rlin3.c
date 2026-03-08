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

#include <nuttx/spinlock.h>
#include <nuttx/irq.h>
#include <nuttx/arch.h>
#include <nuttx/clk/clk.h>
#include <nuttx/dma/dma.h>
#include <nuttx/fs/ioctl.h>
#include <nuttx/serial/uart_renesas_rlin3.h>

#ifdef CONFIG_RENESAS_RLIN3_UART

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define RLIN35_BASE 0xFFC7C100

#define RLIN35_RXBUFSIZE 256
#define RLIN35_TXBUFSIZE 256

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int  renesas_rlin3_setup(FAR struct uart_dev_s *dev);
static void renesas_rlin3_shutdown(FAR struct uart_dev_s *dev);
static int  renesas_rlin3_attach(FAR struct uart_dev_s *dev);
static void renesas_rlin3_detach(FAR struct uart_dev_s *dev);
static int  renesas_rlin3_ioctl(FAR struct file *filep, int cmd, unsigned long arg);
static int  renesas_rlin3_receive(FAR struct uart_dev_s *dev, unsigned int *status);
static void renesas_rlin3_rxint(FAR struct uart_dev_s *dev, bool enable);
static bool renesas_rlin3_rxavailable(FAR struct uart_dev_s *dev);
static void renesas_rlin3_send(FAR struct uart_dev_s *dev, int ch);
static void renesas_rlin3_txint(FAR struct uart_dev_s *dev, bool enable);
static bool renesas_rlin3_txready(FAR struct uart_dev_s *dev);
static bool renesas_rlin3_txempty(FAR struct uart_dev_s *dev);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct renesas_rlin3_ops_s g_renesas_rlin3_ops =
{
  .isr        = renesas_rlin3_interrupt,
  .rxisr      = renesas_rlin3_rxinterrupt,
  .txisr      = renesas_rlin3_txinterrupt,
};

static const struct uart_ops_s g_uart_ops =
{
  .setup          = renesas_rlin3_setup,
  .shutdown       = renesas_rlin3_shutdown,
  .attach         = renesas_rlin3_attach,
  .detach         = renesas_rlin3_detach,
  .ioctl          = renesas_rlin3_ioctl,
  .receive        = renesas_rlin3_receive,
  .rxint          = renesas_rlin3_rxint,
  .rxavailable    = renesas_rlin3_rxavailable,
  .send           = renesas_rlin3_send,
  .txint          = renesas_rlin3_txint,
  .txready        = renesas_rlin3_txready,
  .txempty        = renesas_rlin3_txempty,
};

/* I/O buffers */

static char g_uartrxbuffer[RLIN35_RXBUFSIZE];
static char g_uarttxbuffer[RLIN35_TXBUFSIZE];

static struct renesas_rlin3_s g_uart_priv =
{
  .ops            = &g_renesas_rlin3_ops,
  .uart           = (void*)RLIN35_BASE,
  .uartbase       = RLIN35_BASE,
  .irq            = 436,
  .rxirq          = 437,
  .txirq          = 438,
};

static uart_dev_t g_uart_port =
{
  .recv     =
  {
    .size   = RLIN35_RXBUFSIZE,
    .buffer = g_uartrxbuffer,
  },
  .xmit     =
  {
    .size   = RLIN35_TXBUFSIZE,
    .buffer = g_uarttxbuffer,
  },
  .ops      = &g_uart_ops,
  .priv     = &g_uart_priv,
};

#define CONSOLE_DEV     g_uart_port
#define TTYS0_DEV     g_uart_port

/****************************************************************************
 * Private Functions
 ****************************************************************************/
#include <nuttx/syslog/syslog.h>

static int renesas_rlin3_setup(FAR struct uart_dev_s *dev)
{
  FAR struct renesas_rlin3_s *priv = (FAR struct renesas_rlin3_s *)dev->priv;
  //renesas_rlin3_earlyserialinit(priv->uart);
  return OK;
}

static void renesas_rlin3_shutdown(FAR struct uart_dev_s *dev)
{
  early_syslog("shutdown");

  FAR struct renesas_rlin3_s *priv = (FAR struct renesas_rlin3_s *)dev->priv;

}

static int renesas_rlin3_attach(FAR struct uart_dev_s *dev)
{
  early_syslog("attach");
  FAR struct renesas_rlin3_s *priv = (FAR struct renesas_rlin3_s *)dev->priv;
  int ret;

  /* Attach and enable the IRQ */

  ret = irq_attach(priv->irq, priv->ops->isr, dev);
#ifndef CONFIG_ARCH_NOINTC
  if (ret == OK)
    {
      early_syslog("attach444");
      /* Enable the interrupt (RX and TX interrupts are still disabled
       * in the UART
       * But in UART mode, the interrupt source for this is none.
       * That means this interrupt will not happen.
       */

      up_enable_irq(priv->irq);
      early_syslog("attach555");
    }
#endif
  early_syslog("attach down");

  return ret;
}

static void renesas_rlin3_detach(FAR struct uart_dev_s *dev)
{
  FAR struct renesas_rlin3_s *priv = (FAR struct renesas_rlin3_s *)dev->priv;

  up_disable_irq(priv->irq);
  irqchain_detach(priv->irq, priv->ops->isr, dev);
}

static int renesas_rlin3_ioctl(FAR struct file *filep, int cmd, unsigned long arg)
{
  early_syslog("ioctl");
  return OK;
}

static int renesas_rlin3_receive(FAR struct uart_dev_s *dev, unsigned int *status)
{
  FAR struct renesas_rlin3_s *priv = (FAR struct renesas_rlin3_s *)dev->priv;
  FAR struct renesas_rlin3 *uart = priv->uart;
  uint32_t rbr;

  // Need to check for defs in rh850 & nuttx for status bits
  *status = uart->RLN3nLEST;
  rbr     = uart->RLN3nLURDR;

  return rbr;
}

static void renesas_rlin3_rxint(FAR struct uart_dev_s *dev, bool enable)
{
  FAR struct renesas_rlin3_s *priv = (FAR struct renesas_rlin3_s *)dev->priv;
  FAR struct renesas_rlin3 *uart = priv->uart;

  // In UART mode, the interrupt is enabled.
  // So what we need to do is enable/disable this irq.

  /* Attach and enable the IRQ */

  if(enable) {
    irq_attach(priv->rxirq, priv->ops->rxisr, dev);
    up_enable_irq(priv->rxirq);
  } else {
    up_disable_irq(priv->rxirq);
    irqchain_detach(priv->rxirq, priv->ops->rxisr, dev);
  }
}

static bool renesas_rlin3_rxavailable(FAR struct uart_dev_s *dev)
{
  FAR struct renesas_rlin3_s *priv = (FAR struct renesas_rlin3_s *)dev->priv;
  FAR struct renesas_rlin3 *uart = priv->uart;
  return !(uart->RLN3nLST & RLN3_LST_URS_MSK) & uart->RLN3nLURDR;
}

static void renesas_rlin3_send(FAR struct uart_dev_s *dev, int ch)
{
  FAR struct renesas_rlin3_s *priv = (FAR struct renesas_rlin3_s *)dev->priv;
  FAR struct renesas_rlin3 *uart = priv->uart;
  while (uart->RLN3nLST & RLN3_LST_UTS_MSK);
  uart->RLN3nLUTDR = ch;
}

static void renesas_rlin3_txint(FAR struct uart_dev_s *dev, bool enable)
{
  FAR struct renesas_rlin3_s *priv = (FAR struct renesas_rlin3_s *)dev->priv;
  FAR struct renesas_rlin3 *uart = priv->uart;

  // In UART mode, the interrupt is enabled.
  // So what we need to do is enable/disable this irq.

  /* Attach and enable the IRQ */

  if(enable){
    irq_attach(priv->txirq, priv->ops->txisr, dev);
    up_enable_irq(priv->txirq);

    /* Fake a TX interrupt here by just calling uart_xmitchars() with
     * interrupts disabled (note this may recurse).
    */
    uart_xmitchars(dev);
  } else {
    up_disable_irq(priv->txirq);
    irqchain_detach(priv->txirq, priv->ops->txisr, dev);
  }
}

static bool renesas_rlin3_txready(FAR struct uart_dev_s *dev)
{
  FAR struct renesas_rlin3_s *priv = (FAR struct renesas_rlin3_s *)dev->priv;
  FAR struct renesas_rlin3 *uart = priv->uart;
  //return !(uart->RLN3nLST & RLN3_LST_UTS_MSK);
  return true;
}

static bool renesas_rlin3_txempty(FAR struct uart_dev_s *dev)
{
  FAR struct renesas_rlin3_s *priv = (FAR struct renesas_rlin3_s *)dev->priv;
  FAR struct renesas_rlin3 *uart = priv->uart;
  return !((uart->RLN3nLST & RLN3_LST_UTS_MSK) | uart->RLN3nLUTDR);
}

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
    uart_register("/dev/console", &CONSOLE_DEV);
}

/****************************************************************************
 * Name: renesas_rlin3_interrupt
 *
 * Description:
 *   Handle UART renesas_rlin3 interrupt.
 *
 ****************************************************************************/

int renesas_rlin3_interrupt(int irq, FAR void *context, FAR void *arg) {
  // In UART mode, this should not be called.
  PANIC();
  return 1;
}

/****************************************************************************
 * Name: renesas_rlin3_rxinterrupt
 *
 * Description:
 *   Handle UART renesas_rlin3 RX interrupt.
 *
 ****************************************************************************/

int renesas_rlin3_rxinterrupt(int irq, FAR void *context, FAR void *arg) {
  FAR struct uart_dev_s *dev = (struct uart_dev_s *)arg;

  DEBUGASSERT(dev != NULL && dev->priv != NULL);

  uart_recvchars(dev);

  return OK;
}

/****************************************************************************
 * Name: renesas_rlin3_interrupt
 *
 * Description:
 *   Handle UART renesas_rlin3 TX interrupt.
 *
 ****************************************************************************/

int renesas_rlin3_txinterrupt(int irq, FAR void *context, FAR void *arg) {
  FAR struct uart_dev_s *dev = (struct uart_dev_s *)arg;

  DEBUGASSERT(dev != NULL && dev->priv != NULL);

  uart_xmitchars(dev);

  return OK;
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
