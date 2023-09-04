/*
 * Copyright (c) 2023 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include "cmsis.h"
#include "hw_init.h"
#include "startup.h"

/*----------------------------------------------------------------------------
  Reset Handler called on controller reset
 *----------------------------------------------------------------------------*/
void Reset_Handler(void)
{
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
    __disable_irq();
#ifdef NRF_HW_INIT_RESET_ON_BOOT
    /* Reset CONTROL register */
    __set_CONTROL(0);

    /* Allow the MSP and PSP stacks to descend to address 0,
     * effectively disabling stack overflow protection.
     */
    __set_MSPLIM(0);
    __set_PSPLIM(0);

    /* Disable MPU */
    ARM_MPU_Disable();
#endif /* NRF_HW_INIT_RESET_ON_BOOT */

    SCB->VTOR = (uint32_t) &(__VECTOR_TABLE[0]);

#ifdef NRF_HW_INIT_RESET_ON_BOOT
    /* Initialize core architecture registers and system blocks */
    hw_init_reset_on_boot();
#endif /* NRF_HW_INIT_RESET_ON_BOOT */
#endif

    __set_PSP((uint32_t)(&__INITIAL_SP));

    __set_MSPLIM((uint32_t)(&__STACK_LIMIT));
    __set_PSPLIM((uint32_t)(&__STACK_LIMIT));

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
    __TZ_set_STACKSEAL_S((uint32_t *)(&__STACK_SEAL));
#endif

    SystemInit();                             /* CMSIS System Initialization */
    __PROGRAM_START();                        /* Enter PreMain (C library entry point) */
}
