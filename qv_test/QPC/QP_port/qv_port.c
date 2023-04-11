/*============================================================================
* QP/C Real-Time Embedded Framework (RTEF)
* Copyright (C) 2005 Quantum Leaps, LLC. All rights reserved.
*
* SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-QL-commercial
*
* This software is dual-licensed under the terms of the open source GNU
* General Public License version 3 (or any later version), or alternatively,
* under the terms of one of the closed source Quantum Leaps commercial
* licenses.
*
* The terms of the open source GNU General Public License version 3
* can be found at: <www.gnu.org/licenses/gpl-3.0>
*
* The terms of the closed source Quantum Leaps commercial licenses
* can be found at: <www.state-machine.com/licensing>
*
* Redistributions in source code must retain this top-level comment block.
* Plagiarizing this software to sidestep the license obligations is illegal.
*
* Contact information:
* <www.state-machine.com>
* <info@state-machine.com>
============================================================================*/
/*!
* @date Last updated on: 2022-12-18
* @version Last updated for: @ref qpc_7_2_0
*
* @file
* @brief QV/C port to ARM Cortex-M, ARM-CLANG toolset
*/
/* This QV port is part of the internal QP implementation */
#define QP_IMPL 1U
#include "qf_port.h"

#define SCnSCB_ICTR  ((uint32_t volatile *)0xE000E004U)
#define SCB_SYSPRI   ((uint32_t volatile *)0xE000ED14U)
#define NVIC_IP      ((uint32_t volatile *)0xE000E400U)
#define FPU_FPCCR   *((uint32_t volatile *)0xE000EF34U)

/*..........................................................................*/
/*
* Initialize the exception priorities and IRQ priorities to safe values.
*
* Description:
* On ARMv7-M or higher, this QK port disables interrupts by means of the
* BASEPRI register. However, this method cannot disable interrupt
* priority zero, which is the default for all interrupts out of reset.
* The following code changes the SysTick priority and all IRQ priorities
* to the safe value QF_BASEPRI, which the QF critical section can disable.
* This avoids breaching of the QF critical sections in case the
* application programmer forgets to explicitly set priorities of all
* "kernel aware" interrupts.
*
* The interrupt priorities established in QV_init() can be later
* changed by the application-level code.
*/
void QV_init(void) {

#if (__ARM_ARCH != 6)   /*--------- if ARMv7-M and higher... */

    /* set exception priorities to QF_BASEPRI...
    * SCB_SYSPRI1: Usage-fault, Bus-fault, Memory-fault
    */
    SCB_SYSPRI[1] = (SCB_SYSPRI[1]
        | (QF_BASEPRI << 16U) | (QF_BASEPRI << 8U) | QF_BASEPRI);

    /* SCB_SYSPRI2: SVCall */
    SCB_SYSPRI[2] = (SCB_SYSPRI[2] | (QF_BASEPRI << 24U));

    /* SCB_SYSPRI3:  SysTick, PendSV, Debug */
    SCB_SYSPRI[3] = (SCB_SYSPRI[3]
        | (QF_BASEPRI << 24U) | (QF_BASEPRI << 16U) | QF_BASEPRI);

    /* set all implemented IRQ priories to QF_BASEPRI... */
    uint8_t nprio = (8U + ((*SCnSCB_ICTR & 0x7U) << 3U)) * 4U;
    for (uint8_t n = 0U; n < nprio; ++n) {
        NVIC_IP[n] = QF_BASEPRI;
    }

#endif                  /*--------- ARMv7-M or higher */

    /* SCB_SYSPRI3: PendSV set to priority 0xFF (lowest) */
    SCB_SYSPRI[3] = (SCB_SYSPRI[3] | (0xFFU << 16U));

#if (__ARM_FP != 0)     /*--------- if VFP available... */
    /* configure the FPU for QK */
    FPU_FPCCR |= (1U << 30U)    /* automatic FPU state preservation (ASPEN) */
                 | (1U << 31U); /* lazy stacking (LSPEN) */
#endif                  /*--------- VFP available */
}

/*==========================================================================*/
#if (__ARM_ARCH == 6) /* if ARMv6-M... */

/* hand-optimized quick LOG2 in assembly (no CLZ instruction in ARMv6-M) */
__attribute__ ((naked))
uint_fast8_t QF_qlog2(uint32_t x) {
__asm volatile (
    "  MOVS    r1,#0            \n"
#if (QF_MAX_ACTIVE > 16U)
    "  LSRS    r2,r0,#16        \n"
    "  BEQ     QF_qlog2_1       \n"
    "  MOVS    r1,#16           \n"
    "  MOVS    r0,r2            \n"
    "QF_qlog2_1:                \n"
#endif
#if (QF_MAX_ACTIVE > 8U)
    "  LSRS    r2,r0,#8         \n"
    "  BEQ     QF_qlog2_2       \n"
    "  ADDS    r1, r1,#8        \n"
    "  MOVS    r0, r2           \n"
    "QF_qlog2_2:                \n"
#endif
    "  LSRS    r2,r0,#4         \n"
    "  BEQ     QF_qlog2_3       \n"
    "  ADDS    r1,r1,#4         \n"
    "  MOV     r0,r2            \n"
    "QF_qlog2_3:                \n"
    "  LDR     r2,=QF_qlog2_LUT \n"
    "  LDRB    r0,[r2,r0]       \n"
    "  ADDS    r0,r1,r0         \n"
    "  BX      lr               \n"
    "  .align                   \n"
    "QF_qlog2_LUT:              \n"
    "  .byte 0, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4"
    );
}

#endif /* ARMv6-M */

