# ##############################################################################
# arch/risc-v/src/common/CMakeLists.txt
#
# SPDX-License-Identifier: Apache-2.0
#
# Licensed to the Apache Software Foundation (ASF) under one or more contributor
# license agreements.  See the NOTICE file distributed with this work for
# additional information regarding copyright ownership.  The ASF licenses this
# file to you under the Apache License, Version 2.0 (the "License"); you may not
# use this file except in compliance with the License.  You may obtain a copy of
# the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
# License for the specific language governing permissions and limitations under
# the License.
#
# ##############################################################################

set(SRCS)

list(APPEND SRCS rh850_macros.S)
list(APPEND SRCS rh850_saveusercontext.S)

list(APPEND SRCS rh850_doirq.c)
list(APPEND SRCS rh850_exit.c)
list(APPEND SRCS rh850_switchcontext.c)
list(APPEND SRCS rh850_oneshot.c)
list(APPEND SRCS rh850_oneshot_lowerhalf.c)
list(APPEND SRCS rh850_timerisr.c)
list(APPEND SRCS rh850_initialstate.c)
list(APPEND SRCS rh850_allocateheap.c)
list(APPEND SRCS rh850_initialize.c)
list(APPEND SRCS rh850_idle.c)
list(APPEND SRCS rh850_schedulesigaction.c)
list(APPEND SRCS rh850_stackframe.c)
list(APPEND SRCS rh850_releasestack.c)
list(APPEND SRCS rh850_usestack.c)
list(APPEND SRCS rh850_createstack.c)
list(APPEND SRCS rh850_nputs.c)
list(APPEND SRCS rh850_copystate.c)

#if(CONFIG_ENABLE_ALL_SIGNALS)
#  list(APPEND SRCS riscv_schedulesigaction.c riscv_sigdeliver.c)
#endif()

target_sources(arch PRIVATE ${SRCS})
