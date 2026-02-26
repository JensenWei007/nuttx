/**************************************************************************
 * arch/rh850/src/common/rh850_saveusercontext.S
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
 **************************************************************************/

/**************************************************************************
 * Included Files
 **************************************************************************/

/**************************************************************************
 * Public Functions
 **************************************************************************/

/**************************************************************************
 * Name: up_saveusercontext
 **************************************************************************/

.section .text
.global  _up_saveusercontext
.type   _up_saveusercontext, @function
_up_saveusercontext:
  add     -4, sp
  st.w    r29, 0[sp]
  add     -8, sp
  mov     sp, r29
  st.w    r6, 4[r29]
  mov     0, r10
  mov     r29, sp
  ld.w    8[sp], r29
  add     12, sp
  jmp     [lp]
