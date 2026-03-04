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

/**
 * Restore PC to EIPC or FEPC depending on the exception type
 * r10: vcpu register struct base address
 * uses r20 and r21
 */
_restore_pc:
    ld.w 124[r10], r20
    stsr 5, r21, 0
    andi 0x80, r21, r21
    cmp r0, r21
    be _restore_ei
    ldsr r20, 2, 0     /* set FEPC */
    br _restore_pc_exit
_restore_ei:
    ldsr r20, 2, 0     /* set EIPC */
_restore_pc_exit:
    jmp [lp]

/**
 * Execute eiret or feret depending on the exception type
 * and restores r31 from HVSB
 */
_select_xxret:
    stsr 5, r31, 0
    andi 0x80, r31, r31
    cmp r0, r31
    be _ei_ret
    stsr 20, r31, 1
    feret
_ei_ret:
    stsr 20, r31, 1
    eiret

/**************************************************************************
 * Name: rh850_fullcontextrestore
 **************************************************************************/

.section .text
.global  _rh850_fullcontextrestore
.type   _rh850_fullcontextrestore, @function
_rh850_fullcontextrestore:
  // get arg1 in r10
  add     -4, sp
  st.w    r29, 0[sp]
  add     -4, sp
  mov     sp, r29
  st.w    r6, 0[r29]
  ld.w	  0[r29], r10
  
  /* restore PC */
  jarl _restore_pc, lp

  ld.dw 0[r31], r1
  ld.dw 8[r31], r3
  ld.dw 16[r31], r5
  ld.dw 24[r31], r7
  ld.dw 32[r31], r9
  ld.dw 40[r31], r11
  ld.dw 48[r31], r13
  ld.dw 56[r31], r15
  ld.dw 64[r31], r17
  ld.dw 72[r31], r19
  ld.dw 80[r31], r21
  ld.dw 88[r31], r23
  ld.dw 96[r31], r25
  ld.dw 104[r31], r27
  ld.dw 112[r31], r29
  ld.w 120[r31], r31

  /* select ret instruction */
  br _select_xxret

  /* func ret
  mov	r29, sp
  ld.w	4[sp], r29
  add	8, sp
  jmp	[lp]
  */
