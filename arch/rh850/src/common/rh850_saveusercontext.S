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

.macro LOAD_ADDR    sym, reg
    movhi hi(\sym), r0, \reg
    movea lo(\sym), \reg, \reg
.endm

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
 * r31: vcpu register struct base address
 * uses r20 and r21
 */
_restore_pc:
    ld.w 124[r31], r20
    stsr 5, r21, 0
    andi 0x80, r21, r21
    cmp r0, r21
    be _restore_ei
    ldsr r20, 2, 0     /* set FEPC */
    br _restore_pc_exit
_restore_ei:
    ldsr r20, 0, 0     /* set EIPC */
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
    feret
_ei_ret:
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

  mov r10, r31

  /* restore PC */
  //jarl _restore_pc, lp
  ld.w 124[r31], r20
  ldsr r20, 0, 0

  ld.w 0[r31], r1
  ld.w 4[r31], r2
  ld.w 8[r31], r3
  ld.w 12[r31], r4
  ld.w 16[r31], r5
  ld.w 20[r31], r6
  ld.w 24[r31], r7
  ld.w 28[r31], r8
  ld.w 32[r31], r9
  ld.w 36[r31], r10
  ld.w 40[r31], r11
  ld.w 44[r31], r12
  ld.w 48[r31], r13
  ld.w 52[r31], r14
  ld.w 56[r31], r15
  ld.w 60[r31], r16
  ld.w 64[r31], r17
  ld.w 68[r31], r18
  ld.w 72[r31], r19
  ld.w 76[r31], r20
  ld.w 80[r31], r21
  ld.w 84[r31], r22
  ld.w 88[r31], r23
  ld.w 92[r31], r24
  ld.w 96[r31], r25
  ld.w 100[r31], r26
  ld.w 104[r31], r27
  ld.w 108[r31], r28
  ld.w 112[r31], r29
  ld.w 116[r31], r30
  //ld.w 120[r31], r31

/*
    mov 0xffc7c124, r20
    mov 0x63, r21
    st.w r21, 0[r20]
*/

  eiret

  /* select ret instruction */
  br _select_xxret
