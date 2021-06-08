/*
* Copyright (C) CEVA(R) Inc. All rights reserved
*
* This product or Software is made available exclusively to licensees that have
* received express written authorization from CEVA to download or receive the
* product or Software and have agreed to the End User License Agreement (EULA).
* If you have not received such express authorization and agreed to the CEVA
* EULA, you may not download, install or use this product or Software.
*
* The information contained in this document is subject to change without
* notice and does not represent a commitment on any part of CEVA(R), Inc.
* CEVA(R), Inc. and its subsidiaries make no warranty of any kind with regard
* to this material, including, but not limited to implied warranties of
* merchantability and fitness for a particular purpose whether arising out of
* law, custom, conduct or otherwise.
*
* While the information contained herein is assumed to be accurate, CEVA(R),
* Inc. assumes no responsibility for any errors or omissions contained herein,
* and assumes no liability for special, direct, indirect or consequential
* damage, losses, costs, charges, claims, demands, fees or expenses, of any
* nature or kind, which are incurred in connection with the furnishing,
* performance or use of this material.
*
* This document contains proprietary information, which is protected by U.S.
* and international copyright laws. All rights reserved. No part of this
* document may be reproduced, photocopied, or translated into another language
* without the prior written consent of CEVA(R), Inc.
**/

// TODO rename-reserved: __stack_size; see clang and linker-script
//__asm__(".globl __stack_size");
/*
 * crt0.c:
 * Composed of three parts:
 *
 * 1. The interrupt handlers table.
 * 3. Data sections for stack, heap, stdin/stdout and virtual registers
 * 4. The _start() routine which calls main() and then exit(),
 */

/*
 * The interrupt handlers table.
 */
__asm__(".section .inttbl, \"ax\"");
__asm__(".globl __cxd_inttbl_start");
__asm__("__cxd_inttbl_start:");
__asm__("SC0.cmp {eq} r1.i , r1.i, pr14.b2 , pr15.b2");
__asm__("PCU.nop");
/* Reset jumps to _start(). */
#if defined (RTL_PR14_WA)
__asm__("PCU.brr #_start, #0x0, #0x0, ?pr14.b");
#else
__asm__("PCU.brr #_start, #0x0, #0x0");    
#endif
__asm__("PCU.nop #0x1");
/* TRAPE/BI/PABP jumps to trape bpi handler  */
__asm__(".org 0x20");

#if defined (RTL_PR14_WA)
__asm__("PCU.brr #0, #0x0, #0x0, ?pr14.b");
#else
__asm__("PCU.brr #0, #0x0, #0x0");
#endif
__asm__("PCU.nop #0x1");

__asm__(";;; brr #__cxd_trape_bpi_handler, #0x0, #0x0 ; temporarily taken for file-io");
/* TRAP jumps to trap handler  */
__asm__(".org 0x40");
#if defined (RTL_PR14_WA)
__asm__("PCU.brr #__cxd_trap_handler, #0x0, #0x0, ?pr14.b");
#else
__asm__("PCU.brr #__cxd_trap_handler, #0x0, #0x0");
#endif
__asm__("PCU.nop #0x1");

/* NMI jumps to __cxd_nmi_handler */
__asm__(".org 0x60");
#if defined (RTL_PR14_WA)
__asm__("PCU.brr #__cxd_nmi_handler, #0x0, #0x0, ?pr14.b");
#else
__asm__("PCU.brr #__cxd_nmi_handler, #0x0, #0x0");
#endif
__asm__("PCU.nop #0x1");

/* INT0 jumps to __cxd_int0_handler */
__asm__(".org 0x080");
#if defined (RTL_PR14_WA)
__asm__("PCU.brr #__cxd_int0_handler, #0x0, #0x0, ?pr14.b");
#else
__asm__("PCU.brr #__cxd_int0_handler, #0x0, #0x0");
#endif
__asm__("PCU.nop #0x1");

/* INT1 jumps to __cxd_int1_handler */
__asm__(".org 0x0c0");
#if defined (RTL_PR14_WA)
__asm__("PCU.brr #__cxd_int1_handler, #0x0, #0x0, ?pr14.b");
#else
__asm__("PCU.brr #__cxd_int1_handler, #0x0, #0x0");
#endif
__asm__("PCU.nop #0x1");

/* INT2 jumps to __cxd_int2_handler */
__asm__(".org 0x100");
#if defined (RTL_PR14_WA)
__asm__("PCU.brr #__cxd_int2_handler, #0x0, #0x0, ?pr14.b");
#else
__asm__("PCU.brr #__cxd_int2_handler, #0x0, #0x0");
#endif
__asm__("PCU.nop #0x1");

/* INT3 jumps to __cxd_int3_handler */
__asm__(".org 0x140");
#if defined (RTL_PR14_WA)
__asm__("PCU.brr #__cxd_int3_handler, #0x0, #0x0, ?pr14.b");
#else
__asm__("PCU.brr #__cxd_int3_handler, #0x0, #0x0");
#endif
__asm__("PCU.nop #0x1");

/* INT4 jumps to __cxd_int4_handler */
__asm__(".org 0x180");
#if defined (RTL_PR14_WA)
__asm__("PCU.brr #__cxd_int4_handler, #0x0, #0x0, ?pr14.b");
#else
__asm__("PCU.brr #__cxd_int4_handler, #0x0, #0x0");
#endif
__asm__("PCU.nop");

/* trap0 jumps to __cxd_trap0_handler */
__asm__(".org 0x200");
#if defined (RTL_PR14_WA)
__asm__("PCU.brr #__cxd_trap0_handler, #0x0, #0x0, ?pr14.b");
#else
__asm__("PCU.brr #__cxd_trap0_handler, #0x0, #0x0");
#endif
__asm__("PCU.nop #0x1");

/* trap1 jumps to __cxd_trap1_handler */
__asm__(".org 0x240");
#if defined (RTL_PR14_WA)
__asm__("PCU.brr #__cxd_trap1_handler, #0x0, #0x0, ?pr14.b");
#else
__asm__("PCU.brr #__cxd_trap1_handler, #0x0, #0x0");
#endif
__asm__("PCU.nop #0x1");

/* trap2 jumps to __cxd_trap2_handler */
__asm__(".org 0x280");
#if defined (RTL_PR14_WA)
__asm__("PCU.brr #__cxd_trap2_handler, #0x0, #0x0, ?pr14.b");
#else
__asm__("PCU.brr #__cxd_trap2_handler, #0x0, #0x0");
#endif
__asm__("PCU.nop #0x1");

/* trap3 jumps to __cxd_trap3_handler */
__asm__(".org 0x2c0");
#if defined (RTL_PR14_WA)
__asm__("PCU.brr #__cxd_trap3_handler, #0x0, #0x0, ?pr14.b");
#else
__asm__("PCU.brr #__cxd_trap3_handler, #0x0, #0x0");
#endif
__asm__("PCU.nop #0x1");
__asm__(".org 0x300");


/*
 * Initialize all the sections needed by the compiler, and set labels at the
 * beginning of the sections that we might need later on.
 */

/* The malloc segment can be mapped to anywhere within memory. */
__asm__(".section __MALLOC_SECT,\"aw\"");
//__asm__(".globl __malloc_sect_start__");
//__asm__("__malloc_sect_start__:");

/* The stack segment can be mapped to anywhere within memory. */
//__asm__(".section __STACK_SECT,\"aw\"");

/* The mailbox segment can be mapped to anywhere within memory. */
__asm__(".section __MAILBOX_SECT,\"aw\"");

/* The mmio segment can be mapped to anywhere within memory. */
__asm__(".section __MMIO_SECT,\"aw\"");

// MAJOR TODO
/* The Args segmnet */
//__asm__(".section __ARG_SECT,\"aw\"");
//__asm__("ARG_SECT:");
//#ifndef NO_ARGV_SECT
//__asm__(".space 400");
//#endif
//__asm__("ARG_SECT_END:");

__asm__(".section .rodata,\"a\"");
//__asm__(".globl __crt0_const_data_start__");
//__asm__("__crt0_const_data_start__:");

__asm__(".section .data,\"aw\"");
//__asm__(".globl __crt0_data_start__");
//__asm__("__crt0_data_start__:");

__asm__(".section .bss,\"aw\"");
//__asm__(".globl __crt0_bss_start__");
//__asm__("__crt0_bss_start__:");

__asm__(".section .no_init,\"aw\"");

//__asm__(".section call_saved_store_restore_sect,\"ax\"");
__asm__(".text");

__asm__(".globl __init_hook_post_start");
__asm__(".globl __init_hook_post_start_ret_label__");

/**
 * The _start() routine:
 * `_start()` configures the environment, calls `main()` and
 * then `exit()`.
 */
__asm__(".globl _start");
__asm__(".type _start,@function");
__asm__("_start:");
__asm__(".ceva_prof _start, 2");
__asm__(".func");

/* set floating point rounding mode bits */
__asm__("SC0.mov #0x80000, r0.i");
__asm__("SC0.mov r0.ui, modG.ui");
__asm__("VPU0.vmov r0.ui, modvfp.ui");

/* call user hooks before initialization */
#if defined (RTL_PR14_WA)
__asm__("PCU.brr #__init_hook_post_start, #0x0, #0x0, ?pr14.b");
#else
__asm__("PCU.brr #__init_hook_post_start, #0x0, #0x0");
#endif
__asm__("__init_hook_post_start_ret_label__:");


/* Enable Interrupts */
//__asm__("PCU.eint");

/* Enable L1 instruction Cache */
__asm__("SC0.mov #0x10018000, r0.di");
__asm__("SC0.mov #0x1, r1.di");
__asm__("SC0.mov #0x10019000, r4.di");
__asm__("SC0.mov #0x18000, r3.di");
__asm__("SC0.mov #0xC, r2.di");
__asm__("SC0.out{cpm} r0.di, (#0x424).di");
__asm__("SC0.out{cpm} r1.di, (#0x428).di");
__asm__("SC0.out{cpm} r4.di, (#0x434).di");
__asm__("SC0.out{cpm} r3.di, (#0x424).di");
__asm__("SC0.out{cpm} r2.di, (#0x404).di");

__asm__("nop");
__asm__("nop");
__asm__("nop");
__asm__("nop");
__asm__("nop");
__asm__("nop");
__asm__("nop");
__asm__("nop");

/*
 * NPS internal memory blocks
TQMB Task Queue Memory Block            0x0000_0000  0x4000
DQMBw DMA Write Queue Memory Block      0x0002_0000  0x2000
DQMBr0 DMA Read-0 Queue Memory Block    0x0004_0000  0x2000
DQMBr1 DMA Read-1 Queue Memory Block    0x0006_0000  0x2000
WDQMB WDM Queue Memory Block            0x0007_0000  0x1000
BMB Bias Memory Block                   0x0008_0000  0x4000
CMB Clipping Memory Block               0x000A_0000  0x4000
PCSMB Post-Clipping Shift Memory Block  0x000A_8000  0x1000
PSSMB Post-Scaling Shift Memory Block   0x000B_0000  0x1000
PAPSMB Post-Activation Post-Shift Memory Block  0x000B_8000  0x1000
PMMB Precision Multiplication Memory Block      0x000C_0000  0x2000
PAMB Parametric ReLU Memory Block               0x000C_8000  0x2000
WMB Weight Memory Blocks                        0x0010_0000  0x40000
DMB Data Memory Blocks                          0x0020_0000  0x100000
*/

//***************************************************************
// Config the VPU base address to be above the highest possible
// NeuPro internal memory base address = 0x003FFFFF
//***************************************************************
__asm__("SC0.mov #0x400000, r31.di");
__asm__("SC0.mov #0x0600, r30.di");
__asm__("SC0.out {cpm} r31.ui, (r30.ui).di");
__asm__("nop");
__asm__("nop");
__asm__("nop");
__asm__("nop");
__asm__("nop");
__asm__("nop");
__asm__("nop");
__asm__("nop");
//*********************************************************************
// Read ADD0_START Register and configure REGION_MID to AXIM0
//*********************************************************************
__asm__("SC0.mov #0x0920, r30.di");
__asm__("SC0.in {cpm} (r30.ui).di, r31.ui");
__asm__("nop");
__asm__("nop");
__asm__("nop");
__asm__("nop");
__asm__("nop");
__asm__("nop");
__asm__("nop");
__asm__("nop");
__asm__("SC0.and r31.s2, #0xf00fffff, r31.s2");
__asm__("SC0.or r31.s2, #0x00200000, r31.s2");
__asm__("SC0.out {cpm} r31.ui, (r30.ui).di");
__asm__("nop");
__asm__("nop");
__asm__("nop");
__asm__("nop");
__asm__("nop");
__asm__("nop");
__asm__("nop");
__asm__("nop");
//TQMB
__asm__("SC0.mov #0x0, r20.di");
__asm__("SC0.mov #0x400, r27.di");//loop count = size / 0x10
__asm__("SC0.nop");
__asm__("SC0.mov #0x0, r8.di");
__asm__("SC0.mov #0x0, r9.di");
__asm__("SC0.mov #0x0, r10.di");
__asm__("SC0.mov #0x0, r11.di");
__asm__("TQMB:");
__asm__("LS1.st r8.ui, r9.ui, r10.ui, r11.ui, (r20.ui).ui4+#16");
__asm__("PCU.brrcmplp {neq} r27.di, #-1, #0, #TQMB");
//DQMBw
__asm__("SC0.mov #0x20000, r20.di");
__asm__("SC0.mov #0x200, r27.di");//loop count = size / 0x10
__asm__("SC0.nop");
__asm__("SC0.mov #0x0, r8.di");
__asm__("SC0.mov #0x0, r9.di");
__asm__("SC0.mov #0x0, r10.di");
__asm__("SC0.mov #0x0, r11.di");
__asm__("DQMBw:");
__asm__("LS1.st r8.ui, r9.ui, r10.ui, r11.ui, (r20.ui).ui4+#16");
__asm__("PCU.brrcmplp {neq} r27.di, #-1, #0, #DQMBw");
//DQMBr0
__asm__("SC0.mov #0x40000, r20.di");
__asm__("SC0.mov #0x200, r27.di");//loop count = size / 0x10
__asm__("SC0.nop");
__asm__("SC0.mov #0x0, r8.di");
__asm__("SC0.mov #0x0, r9.di");
__asm__("SC0.mov #0x0, r10.di");
__asm__("SC0.mov #0x0, r11.di");
__asm__("DQMBr0:");
__asm__("LS1.st r8.ui, r9.ui, r10.ui, r11.ui, (r20.ui).ui4+#16");
__asm__("PCU.brrcmplp {neq} r27.di, #-1, #0, #DQMBr0");
//DQMBr1
__asm__("SC0.mov #0x60000, r20.di");
__asm__("SC0.mov #0x200, r27.di");//loop count = size / 0x10
__asm__("SC0.nop");
__asm__("SC0.mov #0x0, r8.di");
__asm__("SC0.mov #0x0, r9.di");
__asm__("SC0.mov #0x0, r10.di");
__asm__("SC0.mov #0x0, r11.di");
__asm__("DQMBr1:");
__asm__("LS1.st r8.ui, r9.ui, r10.ui, r11.ui, (r20.ui).ui4+#16");
__asm__("PCU.brrcmplp {neq} r27.di, #-1, #0, #DQMBr1");
//WDQMB
__asm__("SC0.mov #0x70000, r20.di");
__asm__("SC0.mov #0x100, r27.di");//loop count = size / 0x10
__asm__("SC0.nop");
__asm__("SC0.mov #0x0, r8.di");
__asm__("SC0.mov #0x0, r9.di");
__asm__("SC0.mov #0x0, r10.di");
__asm__("SC0.mov #0x0, r11.di");
__asm__("WDQMB:");
__asm__("LS1.st r8.ui, r9.ui, r10.ui, r11.ui, (r20.ui).ui4+#16");
__asm__("PCU.brrcmplp {neq} r27.di, #-1, #0, #WDQMB");
//BMB
__asm__("SC0.mov #0x80000, r20.di");
__asm__("SC0.mov #0x400, r27.di");//loop count = size / 0x10
__asm__("SC0.nop");
__asm__("SC0.mov #0x0, r8.di");
__asm__("SC0.mov #0x0, r9.di");
__asm__("SC0.mov #0x0, r10.di");
__asm__("SC0.mov #0x0, r11.di");
__asm__("BMB:");
__asm__("LS1.st r8.ui, r9.ui, r10.ui, r11.ui, (r20.ui).ui4+#16");
__asm__("PCU.brrcmplp {neq} r27.di, #-1, #0, #BMB");
//CMB
__asm__("SC0.mov #0xA0000, r20.di");
__asm__("SC0.mov #0x400, r27.di");//loop count = size / 0x10
__asm__("SC0.nop");
__asm__("SC0.mov #0x0, r8.di");
__asm__("SC0.mov #0x0, r9.di");
__asm__("SC0.mov #0x0, r10.di");
__asm__("SC0.mov #0x0, r11.di");
__asm__("CMB:");
__asm__("LS1.st r8.ui, r9.ui, r10.ui, r11.ui, (r20.ui).ui4+#16");
__asm__("PCU.brrcmplp {neq} r27.di, #-1, #0, #CMB");
//PCSMB
__asm__("SC0.mov #0xA8000, r20.di");
__asm__("SC0.mov #0x100, r27.di");//loop count = size / 0x10
__asm__("SC0.nop");
__asm__("SC0.mov #0x0, r8.di");
__asm__("SC0.mov #0x0, r9.di");
__asm__("SC0.mov #0x0, r10.di");
__asm__("SC0.mov #0x0, r11.di");
__asm__("PCSMB:");
__asm__("LS1.st r8.ui, r9.ui, r10.ui, r11.ui, (r20.ui).ui4+#16");
__asm__("PCU.brrcmplp {neq} r27.di, #-1, #0, #PCSMB");
//PSSMB
__asm__("SC0.mov #0xB0000, r20.di");
__asm__("SC0.mov #0x100, r27.di");//loop count = size / 0x10
__asm__("SC0.nop");
__asm__("SC0.mov #0x0, r8.di");
__asm__("SC0.mov #0x0, r9.di");
__asm__("SC0.mov #0x0, r10.di");
__asm__("SC0.mov #0x0, r11.di");
__asm__("PSSMB:");
__asm__("LS1.st r8.ui, r9.ui, r10.ui, r11.ui, (r20.ui).ui4+#16");
__asm__("PCU.brrcmplp {neq} r27.di, #-1, #0, #PSSMB");
//PAPSMB
__asm__("SC0.mov #0xB8000, r20.di");
__asm__("SC0.mov #0x100, r27.di");//loop count = size / 0x10
__asm__("SC0.nop");
__asm__("SC0.mov #0x0, r8.di");
__asm__("SC0.mov #0x0, r9.di");
__asm__("SC0.mov #0x0, r10.di");
__asm__("SC0.mov #0x0, r11.di");
__asm__("PAPSMB:");
__asm__("LS1.st r8.ui, r9.ui, r10.ui, r11.ui, (r20.ui).ui4+#16");
__asm__("PCU.brrcmplp {neq} r27.di, #-1, #0, #PAPSMB");
//PMMB
__asm__("SC0.mov #0xC0000, r20.di");
__asm__("SC0.mov #0x200, r27.di");//loop count = size / 0x10
__asm__("SC0.nop");
__asm__("SC0.mov #0x0, r8.di");
__asm__("SC0.mov #0x0, r9.di");
__asm__("SC0.mov #0x0, r10.di");
__asm__("SC0.mov #0x0, r11.di");
__asm__("PMMB:");
__asm__("LS1.st r8.ui, r9.ui, r10.ui, r11.ui, (r20.ui).ui4+#16");
__asm__("PCU.brrcmplp {neq} r27.di, #-1, #0, #PMMB");
//PAMB
__asm__("SC0.mov #0xC8000, r20.di");
__asm__("SC0.mov #0x200, r27.di");//loop count = size / 0x10
__asm__("SC0.nop");
__asm__("SC0.mov #0x0, r8.di");
__asm__("SC0.mov #0x0, r9.di");
__asm__("SC0.mov #0x0, r10.di");
__asm__("SC0.mov #0x0, r11.di");
__asm__("PAMB:");
__asm__("LS1.st r8.ui, r9.ui, r10.ui, r11.ui, (r20.ui).ui4+#16");
__asm__("PCU.brrcmplp {neq} r27.di, #-1, #0, #PAMB");
//WMB
__asm__("SC0.mov #0x100000, r20.di");
__asm__("SC0.mov #0x4000, r27.di");//loop count = size / 0x10
__asm__("SC0.nop");
__asm__("SC0.mov #0x0, r8.di");
__asm__("SC0.mov #0x0, r9.di");
__asm__("SC0.mov #0x0, r10.di");
__asm__("SC0.mov #0x0, r11.di");
__asm__("WMB:");
__asm__("LS1.st r8.ui, r9.ui, r10.ui, r11.ui, (r20.ui).ui4+#16");
__asm__("PCU.brrcmplp {neq} r27.di, #-1, #0, #WMB");
//DMB
__asm__("SC0.mov #0x200000, r20.di");
__asm__("SC0.mov #0x10000, r27.di");//loop count = size / 0x10
__asm__("SC0.nop");
__asm__("SC0.mov #0x0, r8.di");
__asm__("SC0.mov #0x0, r9.di");
__asm__("SC0.mov #0x0, r10.di");
__asm__("SC0.mov #0x0, r11.di");
__asm__("DMB:");
__asm__("LS1.st r8.ui, r9.ui, r10.ui, r11.ui, (r20.ui).ui4+#16");
__asm__("PCU.brrcmplp {neq} r27.di, #-1, #0, #DMB");

__asm__("nop");
__asm__("nop");
__asm__("nop");
__asm__("nop");
__asm__("nop");
__asm__("nop");
__asm__("nop");
__asm__("nop");

//***************************************************************
// Move back DTCM to 0x0
//***************************************************************
__asm__("SC0.mov #0x0, r31.di");
__asm__("SC0.mov #0x0600, r30.di");
__asm__("SC0.out {cpm} r31.ui, (r30.ui).di");
__asm__("nop");
__asm__("nop");
__asm__("nop");
__asm__("nop");
__asm__("nop");
__asm__("nop");
__asm__("nop");
__asm__("nop");

//*********************************************************************
// Read ADD0_START Register and configure REGION_MID to EDP
//*********************************************************************
__asm__("SC0.mov #0x0920, r30.di");
__asm__("SC0.in {cpm} (r30.ui).di, r31.ui");
__asm__("nop");
__asm__("nop");
__asm__("nop");
__asm__("nop");
__asm__("nop");
__asm__("nop");
__asm__("nop");
__asm__("nop");
__asm__("SC0.and r31.s2, #0xf00fffff, r31.s2");
__asm__("SC0.or r31.s2, #0x00100000, r31.s2");
__asm__("SC0.out {cpm} r31.ui, (r30.ui).di");
__asm__("nop");
__asm__("nop");
__asm__("nop");
__asm__("nop");
__asm__("nop");
__asm__("nop");
__asm__("nop");
__asm__("nop");

// DTCM Memory Initialization
__asm__("SC0.mov #0x0, r20.di");
__asm__("SC0.mov #0x8000, r27.di");//loop count = size / 0x10
__asm__("SC0.nop");

__asm__("SC0.mov #0x0, r8.di");
__asm__("SC0.mov #0x0, r9.di");
__asm__("SC0.mov #0x0, r10.di");
__asm__("SC0.mov #0x0, r11.di");

__asm__("dtcm_mem_init:");
__asm__("LS1.st r8.ui, r9.ui, r10.ui, r11.ui, (r20.ui).ui4+#16");
__asm__("PCU.brrcmplp {neq} r27.di, #-1, #0, #dtcm_mem_init");

__asm__("nop");
__asm__("nop");
__asm__("nop");

/*
 * set up the stack
 */
// TODO rename-reserved: .__STACK_SECT
__asm__("SC0.mov #__stack_start, r2.i");
__asm__("SC0.mov r2.ui, sp.ui");

__asm__("nop #0x2");
__asm__("SC0.add sp.ui, #__stack_size, sp.ui");
__asm__("nop #0x4");
/* align the stack pointer on a 4-byte boundary */
__asm__("SC0.mov sp.ui, r2.ui");
__asm__("SC0.and r2.s2, #0xfffffffc, r2.s2");
__asm__("_sp_end_init:");
__asm__("SC0.mov r2.ui, sp.ui");
/* init all configuration registers. */
#if defined (RTL_PR14_WA)
__asm__("PCU.callr {ds1} #__init_config_regs, ?pr14.b");
#else
__asm__("PCU.callr {ds1} #__init_config_regs");
#endif
__asm__("PCU.nop #0x1");

/*
 * copy any data sections across internal memory boundaries, before calling
 * data initialization functions
 */

/*
 * BSS Initialization. Note that access < 128bit to DTCM leads to ECC error.
 *
 */
__asm__("SC0.mov #__bss_start, r3.di\n"
        "|| SC1.mov #__bss_end, r4.di");
__asm__("SC0.mov #0x0, r2.di");
__asm__("nop");
__asm__("PCU.brrcmp {ge} r3.ui, r4.ui, #.L__skip_bss_init, #0x0, #0x0, pr15.b2, pr15.b2");
__asm__(".L__bss_init_loop:");
__asm__("LS1.st r2.di, r2.di, r2.di, r2.di, (r3.ui).di4+#16");
__asm__("nop");
__asm__("PCU.brrcmp {lt} r3.ui, r4.ui, #.L__bss_init_loop, #0x0, #0x0, pr15.b2, pr15.b2");
__asm__("PCU.nop #0x1");

__asm__(".L__skip_bss_init:");
__asm__("SC0.mov #__bss1_start, r3.di\n"
        "|| SC1.mov #__bss1_end, r4.di");
__asm__("SC0.mov #0x0, r2.di");
__asm__("nop");
__asm__("PCU.brrcmp {ge} r3.ui, r4.ui, #.L__skip_bss1_init, #0x0, #0x0, pr15.b2, pr15.b2");
__asm__(".L__bss1_init_loop:");
__asm__("LS1.st r2.di, r2.di, r2.di, r2.di, (r3.ui).di4+#16");
__asm__("nop");
__asm__("PCU.brrcmp {lt} r3.ui, r4.ui, #.L__bss1_init_loop, #0x0, #0x0, pr15.b2, pr15.b2");
__asm__("PCU.nop #0x1");

/*
 * Copy Data Section. Note that access < 128bit to DTCM leads to ECC error.
 *
 */
__asm__(".L__skip_bss1_init:");
__asm__("SC0.mov #__elf_data_internal_start_LMA, r3.di\n"
        "|| SC1.mov #__elf_data_internal_start, r4.di");
__asm__("SC0.mov #__elf_data_internal_size, r2.di");
__asm__("nop");
__asm__("PCU.brrcmp {eq} r2.ui, #0, #.L__skip_data_init, #0x0, #0x0, pr15.b2, pr15.b2");
__asm__("SC0.add r4.di, #__elf_data_internal_size, r2.di");
__asm__(".L__data_init_loop:");
__asm__("LS0.ld (r3.ui).di4+#16, r5.di, r6.di, r7.di, r8.di");
__asm__("PCU.nop #0x1");
__asm__("LS1.st r5.di, r6.di, r7.di, r8.di, (r4.ui).di4+#16");
__asm__("nop");
__asm__("PCU.brrcmp {lt} r4.ui, r2.ui, #.L__data_init_loop, #0x0, #0x0, pr15.b2, pr15.b2");
__asm__("PCU.nop #0x1");

__asm__(".L__skip_data_init:");
//#if defined (RTL_PR14_WA)
//__asm__("PCU.callr {ds1} #__init_data_sections, ?pr14.b");
//#else
//__asm__("PCU.callr {ds1} #__init_data_sections");
//#endif
__asm__("PCU.nop #0x1");

/*
 * The `__not_needed` symbol is used by the RCMPL suite for profiling
 */
__asm__(".global __not_needed");
__asm__("__not_needed:");

/*
 * Initializations must come after globals have been copied from program memory
 * to data memory.
 */
#if defined (RTL_PR14_WA)
__asm__("PCU.callr {ds1} #__init_func_vec, ?pr14.b");
#else
__asm__("PCU.callr {ds1} #__init_func_vec");
#endif
__asm__("PCU.nop #0x1");
#if defined (RTL_PR14_WA)
__asm__("PCU.callr #__init_hook_pre_main, ?pr14.b");
#else
__asm__("PCU.callr #__init_hook_pre_main");
#endif

/*
 * preparing the arguments for main:
 * - [##__arg_sect_start.0] holds argc.
 * - ##__arg_sect_start.1 holds argv.
 * If there is no args section, make sure argc/argv are zero
 */
/*
__asm__(
	"SC0.mov #__arg_sect_start, r2.i\n"
	"|| SC1.mov #__arg_sect_end, r3.i\n"
	"SC0.mov #0, r0.i\n"
	"|| SC1.mov #0, r1.i\n");
__asm__("nop");
__asm__("PCU.brrcmp {eq} r2.di, r3.di, #.L__args_loaded, #0x0, #0x0, pr15.b2, pr15.b2");
__asm__("LS0.ld (#__arg_sect_start).ui, r0.i\n"
	"|| SC0.mov #__arg_sect_start+4, r1.i");
__asm__(".L__args_loaded:");
*/
#if defined (RTL_PR14_WA)
__asm__("PCU.callr #main, ?pr14.b");
#else
__asm__("PCU.callr #main");
#endif
__asm__("nop #0x1");
// main will leave the return value in r0, passed straight on to exit
__asm__("PCU.brr {ds1} #exit, #0x0, #0x0, ?pr14.b");
__asm__("nop #0x1");
__asm__(".endfunc");
__asm__(".L_start_func_end0:");
__asm__(".size _start, .L_start_func_end0-_start");
