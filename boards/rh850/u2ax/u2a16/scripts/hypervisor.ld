/****************************************************************************
 * boards/rh850/u2a16/scripts/hypervisor.script
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

#include <arch/board/board_memorymap.h>

MEMORY
{
    DATA_FLASH : ORIGIN = DATA_FLASH_BASE, LENGTH = DATA_FLASH_SIZE
    CODE_FLASH : ORIGIN = CODE_FLASH_BASE, LENGTH = CODE_FLASH_SIZE
    RAM : ORIGIN = RAM_BASE, LENGTH = RAM_SIZE
}

ENTRY(_start)

SECTIONS {

    .start : {
        *(.start*)
    } > CODE_FLASH

    .text :  {
        _stext = . ;
        *(.text*)
        _etext = . ;
    } > CODE_FLASH

    .init_section :
    {
        _sinit = ABSOLUTE(.);
        KEEP(*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
        KEEP(*(.init_array .ctors))
        _einit = ABSOLUTE(.);
    } > CODE_FLASH

    .const : {
        *(.const*)
        *(.rodata*)
    } > CODE_FLASH

    .data : ALIGN(8) {
        __data_start = .;
        *(.data .data.*)
        PROVIDE(__global_pointer$ = . + 0x800);
        __gp = . + 0x800;
        *(.sdata .sdata.* .sdata2.*)
        . = ALIGN(8);
        __data_end = .;
    } > RAM AT > DATA_FLASH

    /* LMA (RO_MEM) for startup copy */
    __data_load_start = LOADADDR(.data);
    __data_load_end   = LOADADDR(.data) + SIZEOF(.data);

    .bss (NOLOAD) : {
        __bss_start = .;
        *(.bss* .sbss*)
        *(COMMON)
        . = ALIGN(8);
        __bss_end = .;
    } > RAM

    .stackheap (NOLOAD) : ALIGN(16) {
        PROVIDE(_stack_base = .);
        . = . + STACKHEAP_SIZE;
        PROVIDE(_heap_base = .);
        PROVIDE(__heap_base = .); /* needed for RH850 */
    } > RAM
}
