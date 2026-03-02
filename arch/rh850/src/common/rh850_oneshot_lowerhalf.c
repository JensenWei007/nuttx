/****************************************************************************
 * arch/rh850/src/common/rh850_oneshot_lowerhalf.c
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

#include <arch/hardware/timer_map.h>

#include "rh850_oneshot.h"

/****************************************************************************
 * Private Types
 ****************************************************************************/

/* This structure provides the private representation of the "lower-half"
 * driver state structure.  This structure must be cast-compatible with the
 * oneshot_lowerhalf_s structure.
 */

struct rh850_oneshot_lowerhalf_s
{
  struct oneshot_lowerhalf_s    lower;
  volatile struct rh850_OSTMn*  ostm;
  int                           index;
  uint32_t                      base;
  int                           irq;
  uint32_t                      freq;
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int rh850_oneshot_max_delay(struct oneshot_lowerhalf_s *lower, 
                                  struct timespec *ts);
static int rh850_oneshot_current(struct oneshot_lowerhalf_s *lower, 
                                struct timespec *ts);
static int rh850_oneshot_start(struct oneshot_lowerhalf_s *lower,
                              const struct timespec *ts);
static int rh850_oneshot_cancel(struct oneshot_lowerhalf_s *lower, 
                                struct timespec *ts);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct oneshot_operations_s g_rh850_oneshot_ops =
{
  .current        = rh850_oneshot_current,
  .start          = rh850_oneshot_start,
  .cancel         = rh850_oneshot_cancel,
  .max_delay      = rh850_oneshot_max_delay
};

static struct rh850_oneshot_lowerhalf_s g_rh850_oneshot_lowerhalf =
{
  .lower.ops = &g_rh850_oneshot_ops
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

void cycles_to_timespec(uint64_t cycles, struct timespec *ts) {
    uint64_t total_ns = (cycles * 1000000000ull) / TIMER_FREQ;
    
    ts->tv_sec = total_ns / 1000000000ull;
    ts->tv_nsec = total_ns % 1000000000ull;
}

static int rh850_oneshot_max_delay(struct oneshot_lowerhalf_s *lower,
                                    struct timespec *ts)
{
  cycles_to_timespec(UINT32_MAX, ts);
  return 0;
}

static int rh850_oneshot_current(struct oneshot_lowerhalf_s *lower,
                                  struct timespec *ts)
{
  struct rh850_oneshot_lowerhalf_s *priv = &g_rh850_oneshot_lowerhalf;
  cycles_to_timespec(rh850_ostm_get(priv->ostm), ts);
  return 0;
}

static int rh850_oneshot_start(struct oneshot_lowerhalf_s *lower,
                              const struct timespec *ts)
{
  struct rh850_oneshot_lowerhalf_s *priv = &g_rh850_oneshot_lowerhalf;
  uint32_t com = TIME_S(ts->tv_sec) + TIME_US(ts->tv_nsec / 1000);
  rh850_ostm_set(priv->ostm, com);
  return 0;
}

static int rh850_oneshot_cancel(struct oneshot_lowerhalf_s *lower,
                                struct timespec *ts)
{
  struct rh850_oneshot_lowerhalf_s *priv = &g_rh850_oneshot_lowerhalf;
  rh850_ostm_cancel(priv->ostm);
  return 0;
}

static int rh850_oneshot_interrupt(int irq, void *context, void *arg)
{
  struct rh850_oneshot_lowerhalf_s *priv = &g_rh850_oneshot_lowerhalf;

  oneshot_process_callback(&priv->lower);

  return 0;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

struct oneshot_lowerhalf_s *
rh850_oneshot_initialize(int index, uint32_t freq)
{
  struct rh850_oneshot_lowerhalf_s *priv = &g_rh850_oneshot_lowerhalf;

  priv->index    = index;
  priv->base     = OSTM_BASE[index];
  priv->irq      = OSTM_IRQ[index];
  priv->freq     = freq;
  priv->ostm     = (void *)priv->base;

  rh850_ostm_enable(priv->ostm);

  irq_attach(priv->irq, rh850_oneshot_interrupt, priv);

  return &priv->lower;
}
