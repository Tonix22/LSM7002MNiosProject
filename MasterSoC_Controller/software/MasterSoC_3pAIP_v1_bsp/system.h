/*
 * system.h - SOPC Builder system and BSP software package information
 *
 * Machine generated for CPU 'nios2_gen2_0' in SOPC Builder design 'master_nios_multiple_slave'
 * SOPC Builder design path: ../../master_nios_multiple_slave.sopcinfo
 *
 * Generated: Tue Sep 23 07:17:01 CDT 2025
 */

/*
 * DO NOT MODIFY THIS FILE
 *
 * Changing this file will have subtle consequences
 * which will almost certainly lead to a nonfunctioning
 * system. If you do modify this file, be aware that your
 * changes will be overwritten and lost when this file
 * is generated again.
 *
 * DO NOT MODIFY THIS FILE
 */

/*
 * License Agreement
 *
 * Copyright (c) 2008
 * Altera Corporation, San Jose, California, USA.
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * This agreement shall be governed in all respects by the laws of the State
 * of California and by the laws of the United States of America.
 */

#ifndef __SYSTEM_H_
#define __SYSTEM_H_

/* Include definitions from linker script generator */
#include "linker.h"


/*
 * CPU configuration
 *
 */

#define ALT_CPU_ARCHITECTURE "altera_nios2_gen2"
#define ALT_CPU_BIG_ENDIAN 0
#define ALT_CPU_BREAK_ADDR 0x00044820
#define ALT_CPU_CPU_ARCH_NIOS2_R1
#define ALT_CPU_CPU_FREQ 50000000u
#define ALT_CPU_CPU_ID_SIZE 1
#define ALT_CPU_CPU_ID_VALUE 0x00000000
#define ALT_CPU_CPU_IMPLEMENTATION "tiny"
#define ALT_CPU_DATA_ADDR_WIDTH 0x13
#define ALT_CPU_DCACHE_LINE_SIZE 0
#define ALT_CPU_DCACHE_LINE_SIZE_LOG2 0
#define ALT_CPU_DCACHE_SIZE 0
#define ALT_CPU_EXCEPTION_ADDR 0x00000020
#define ALT_CPU_FLASH_ACCELERATOR_LINES 0
#define ALT_CPU_FLASH_ACCELERATOR_LINE_SIZE 0
#define ALT_CPU_FLUSHDA_SUPPORTED
#define ALT_CPU_FREQ 50000000
#define ALT_CPU_HARDWARE_DIVIDE_PRESENT 0
#define ALT_CPU_HARDWARE_MULTIPLY_PRESENT 0
#define ALT_CPU_HARDWARE_MULX_PRESENT 0
#define ALT_CPU_HAS_DEBUG_CORE 1
#define ALT_CPU_HAS_DEBUG_STUB
#define ALT_CPU_HAS_ILLEGAL_INSTRUCTION_EXCEPTION
#define ALT_CPU_HAS_JMPI_INSTRUCTION
#define ALT_CPU_ICACHE_LINE_SIZE 0
#define ALT_CPU_ICACHE_LINE_SIZE_LOG2 0
#define ALT_CPU_ICACHE_SIZE 0
#define ALT_CPU_INST_ADDR_WIDTH 0x13
#define ALT_CPU_NAME "nios2_gen2_0"
#define ALT_CPU_OCI_VERSION 1
#define ALT_CPU_RESET_ADDR 0x00000000


/*
 * CPU configuration (with legacy prefix - don't use these anymore)
 *
 */

#define NIOS2_BIG_ENDIAN 0
#define NIOS2_BREAK_ADDR 0x00044820
#define NIOS2_CPU_ARCH_NIOS2_R1
#define NIOS2_CPU_FREQ 50000000u
#define NIOS2_CPU_ID_SIZE 1
#define NIOS2_CPU_ID_VALUE 0x00000000
#define NIOS2_CPU_IMPLEMENTATION "tiny"
#define NIOS2_DATA_ADDR_WIDTH 0x13
#define NIOS2_DCACHE_LINE_SIZE 0
#define NIOS2_DCACHE_LINE_SIZE_LOG2 0
#define NIOS2_DCACHE_SIZE 0
#define NIOS2_EXCEPTION_ADDR 0x00000020
#define NIOS2_FLASH_ACCELERATOR_LINES 0
#define NIOS2_FLASH_ACCELERATOR_LINE_SIZE 0
#define NIOS2_FLUSHDA_SUPPORTED
#define NIOS2_HARDWARE_DIVIDE_PRESENT 0
#define NIOS2_HARDWARE_MULTIPLY_PRESENT 0
#define NIOS2_HARDWARE_MULX_PRESENT 0
#define NIOS2_HAS_DEBUG_CORE 1
#define NIOS2_HAS_DEBUG_STUB
#define NIOS2_HAS_ILLEGAL_INSTRUCTION_EXCEPTION
#define NIOS2_HAS_JMPI_INSTRUCTION
#define NIOS2_ICACHE_LINE_SIZE 0
#define NIOS2_ICACHE_LINE_SIZE_LOG2 0
#define NIOS2_ICACHE_SIZE 0
#define NIOS2_INST_ADDR_WIDTH 0x13
#define NIOS2_OCI_VERSION 1
#define NIOS2_RESET_ADDR 0x00000000


/*
 * Define for each module class mastered by the CPU
 *
 */

#define __ALTERA_AVALON_JTAG_UART
#define __ALTERA_AVALON_ONCHIP_MEMORY2
#define __ALTERA_AVALON_PIO
#define __ALTERA_AVALON_SPI
#define __ALTERA_AVALON_TIMER
#define __ALTERA_AVALON_UART
#define __ALTERA_NIOS2_GEN2
#define __AVALON_AIP_AVALON_INTERFACE


/*
 * System configuration
 *
 */

#define ALT_DEVICE_FAMILY "Cyclone V"
#define ALT_ENHANCED_INTERRUPT_API_PRESENT
#define ALT_IRQ_BASE NULL
#define ALT_LOG_PORT "/dev/null"
#define ALT_LOG_PORT_BASE 0x0
#define ALT_LOG_PORT_DEV null
#define ALT_LOG_PORT_TYPE ""
#define ALT_NUM_EXTERNAL_INTERRUPT_CONTROLLERS 0
#define ALT_NUM_INTERNAL_INTERRUPT_CONTROLLERS 1
#define ALT_NUM_INTERRUPT_CONTROLLERS 1
#define ALT_STDERR "/dev/jtag_uart_0"
#define ALT_STDERR_BASE 0x460b0
#define ALT_STDERR_DEV jtag_uart_0
#define ALT_STDERR_IS_JTAG_UART
#define ALT_STDERR_PRESENT
#define ALT_STDERR_TYPE "altera_avalon_jtag_uart"
#define ALT_STDIN "/dev/jtag_uart_0"
#define ALT_STDIN_BASE 0x460b0
#define ALT_STDIN_DEV jtag_uart_0
#define ALT_STDIN_IS_JTAG_UART
#define ALT_STDIN_PRESENT
#define ALT_STDIN_TYPE "altera_avalon_jtag_uart"
#define ALT_STDOUT "/dev/jtag_uart_0"
#define ALT_STDOUT_BASE 0x460b0
#define ALT_STDOUT_DEV jtag_uart_0
#define ALT_STDOUT_IS_JTAG_UART
#define ALT_STDOUT_PRESENT
#define ALT_STDOUT_TYPE "altera_avalon_jtag_uart"
#define ALT_SYSTEM_NAME "master_nios_multiple_slave"


/*
 * aip_0 configuration
 *
 */

#define AIP_0_BASE 0x45000
#define AIP_0_IRQ -1
#define AIP_0_IRQ_INTERRUPT_CONTROLLER_ID -1
#define AIP_0_NAME "/dev/aip_0"
#define AIP_0_SPAN 1024
#define AIP_0_TYPE "avalon_aip_avalon_interface"
#define ALT_MODULE_CLASS_aip_0 avalon_aip_avalon_interface


/*
 * aip_1 configuration
 *
 */

#define AIP_1_BASE 0x45800
#define AIP_1_IRQ -1
#define AIP_1_IRQ_INTERRUPT_CONTROLLER_ID -1
#define AIP_1_NAME "/dev/aip_1"
#define AIP_1_SPAN 1024
#define AIP_1_TYPE "avalon_aip_avalon_interface"
#define ALT_MODULE_CLASS_aip_1 avalon_aip_avalon_interface


/*
 * aip_2 configuration
 *
 */

#define AIP_2_BASE 0x45400
#define AIP_2_IRQ -1
#define AIP_2_IRQ_INTERRUPT_CONTROLLER_ID -1
#define AIP_2_NAME "/dev/aip_2"
#define AIP_2_SPAN 1024
#define AIP_2_TYPE "avalon_aip_avalon_interface"
#define ALT_MODULE_CLASS_aip_2 avalon_aip_avalon_interface


/*
 * aip_uP_0 configuration
 *
 */

#define AIP_UP_0_BASE 0x45c00
#define AIP_UP_0_IRQ -1
#define AIP_UP_0_IRQ_INTERRUPT_CONTROLLER_ID -1
#define AIP_UP_0_NAME "/dev/aip_uP_0"
#define AIP_UP_0_SPAN 1024
#define AIP_UP_0_TYPE "avalon_aip_avalon_interface"
#define ALT_MODULE_CLASS_aip_uP_0 avalon_aip_avalon_interface


/*
 * hal configuration
 *
 */

#define ALT_INCLUDE_INSTRUCTION_RELATED_EXCEPTION_API
#define ALT_MAX_FD 4
#define ALT_SYS_CLK none
#define ALT_TIMESTAMP_CLK none


/*
 * int_IP_s0 configuration
 *
 */

#define ALT_MODULE_CLASS_int_IP_s0 altera_avalon_pio
#define INT_IP_S0_BASE 0x46090
#define INT_IP_S0_BIT_CLEARING_EDGE_REGISTER 0
#define INT_IP_S0_BIT_MODIFYING_OUTPUT_REGISTER 0
#define INT_IP_S0_CAPTURE 1
#define INT_IP_S0_DATA_WIDTH 8
#define INT_IP_S0_DO_TEST_BENCH_WIRING 0
#define INT_IP_S0_DRIVEN_SIM_VALUE 0
#define INT_IP_S0_EDGE_TYPE "FALLING"
#define INT_IP_S0_FREQ 50000000
#define INT_IP_S0_HAS_IN 1
#define INT_IP_S0_HAS_OUT 0
#define INT_IP_S0_HAS_TRI 0
#define INT_IP_S0_IRQ 3
#define INT_IP_S0_IRQ_INTERRUPT_CONTROLLER_ID 0
#define INT_IP_S0_IRQ_TYPE "EDGE"
#define INT_IP_S0_NAME "/dev/int_IP_s0"
#define INT_IP_S0_RESET_VALUE 0
#define INT_IP_S0_SPAN 16
#define INT_IP_S0_TYPE "altera_avalon_pio"


/*
 * jtag_uart_0 configuration
 *
 */

#define ALT_MODULE_CLASS_jtag_uart_0 altera_avalon_jtag_uart
#define JTAG_UART_0_BASE 0x460b0
#define JTAG_UART_0_IRQ 0
#define JTAG_UART_0_IRQ_INTERRUPT_CONTROLLER_ID 0
#define JTAG_UART_0_NAME "/dev/jtag_uart_0"
#define JTAG_UART_0_READ_DEPTH 64
#define JTAG_UART_0_READ_THRESHOLD 8
#define JTAG_UART_0_SPAN 8
#define JTAG_UART_0_TYPE "altera_avalon_jtag_uart"
#define JTAG_UART_0_WRITE_DEPTH 64
#define JTAG_UART_0_WRITE_THRESHOLD 8


/*
 * leds configuration
 *
 */

#define ALT_MODULE_CLASS_leds altera_avalon_pio
#define LEDS_BASE 0x46080
#define LEDS_BIT_CLEARING_EDGE_REGISTER 0
#define LEDS_BIT_MODIFYING_OUTPUT_REGISTER 0
#define LEDS_CAPTURE 0
#define LEDS_DATA_WIDTH 10
#define LEDS_DO_TEST_BENCH_WIRING 0
#define LEDS_DRIVEN_SIM_VALUE 0
#define LEDS_EDGE_TYPE "NONE"
#define LEDS_FREQ 50000000
#define LEDS_HAS_IN 0
#define LEDS_HAS_OUT 1
#define LEDS_HAS_TRI 0
#define LEDS_IRQ -1
#define LEDS_IRQ_INTERRUPT_CONTROLLER_ID -1
#define LEDS_IRQ_TYPE "NONE"
#define LEDS_NAME "/dev/leds"
#define LEDS_RESET_VALUE 0
#define LEDS_SPAN 16
#define LEDS_TYPE "altera_avalon_pio"


/*
 * mem_program configuration
 *
 */

#define ALT_MODULE_CLASS_mem_program altera_avalon_onchip_memory2
#define MEM_PROGRAM_ALLOW_IN_SYSTEM_MEMORY_CONTENT_EDITOR 0
#define MEM_PROGRAM_ALLOW_MRAM_SIM_CONTENTS_ONLY_FILE 0
#define MEM_PROGRAM_BASE 0x0
#define MEM_PROGRAM_CONTENTS_INFO ""
#define MEM_PROGRAM_DUAL_PORT 0
#define MEM_PROGRAM_GUI_RAM_BLOCK_TYPE "AUTO"
#define MEM_PROGRAM_INIT_CONTENTS_FILE "master_nios_multiple_slave_mem_program"
#define MEM_PROGRAM_INIT_MEM_CONTENT 0
#define MEM_PROGRAM_INSTANCE_ID "NONE"
#define MEM_PROGRAM_IRQ -1
#define MEM_PROGRAM_IRQ_INTERRUPT_CONTROLLER_ID -1
#define MEM_PROGRAM_NAME "/dev/mem_program"
#define MEM_PROGRAM_NON_DEFAULT_INIT_FILE_ENABLED 0
#define MEM_PROGRAM_RAM_BLOCK_TYPE "AUTO"
#define MEM_PROGRAM_READ_DURING_WRITE_MODE "DONT_CARE"
#define MEM_PROGRAM_SINGLE_CLOCK_OP 0
#define MEM_PROGRAM_SIZE_MULTIPLE 1
#define MEM_PROGRAM_SIZE_VALUE 256000
#define MEM_PROGRAM_SPAN 256000
#define MEM_PROGRAM_TYPE "altera_avalon_onchip_memory2"
#define MEM_PROGRAM_WRITABLE 1


/*
 * spi_0 configuration
 *
 */

#define ALT_MODULE_CLASS_spi_0 altera_avalon_spi
#define SPI_0_BASE 0x46040
#define SPI_0_CLOCKMULT 1
#define SPI_0_CLOCKPHASE 1
#define SPI_0_CLOCKPOLARITY 0
#define SPI_0_CLOCKUNITS "Hz"
#define SPI_0_DATABITS 8
#define SPI_0_DATAWIDTH 16
#define SPI_0_DELAYMULT "1.0E-9"
#define SPI_0_DELAYUNITS "ns"
#define SPI_0_EXTRADELAY 0
#define SPI_0_INSERT_SYNC 0
#define SPI_0_IRQ 5
#define SPI_0_IRQ_INTERRUPT_CONTROLLER_ID 0
#define SPI_0_ISMASTER 1
#define SPI_0_LSBFIRST 0
#define SPI_0_NAME "/dev/spi_0"
#define SPI_0_NUMSLAVES 1
#define SPI_0_PREFIX "spi_"
#define SPI_0_SPAN 32
#define SPI_0_SYNC_REG_DEPTH 2
#define SPI_0_TARGETCLOCK 50000000u
#define SPI_0_TARGETSSDELAY "0.0"
#define SPI_0_TYPE "altera_avalon_spi"


/*
 * start_uP configuration
 *
 */

#define ALT_MODULE_CLASS_start_uP altera_avalon_pio
#define START_UP_BASE 0x460a0
#define START_UP_BIT_CLEARING_EDGE_REGISTER 0
#define START_UP_BIT_MODIFYING_OUTPUT_REGISTER 0
#define START_UP_CAPTURE 1
#define START_UP_DATA_WIDTH 1
#define START_UP_DO_TEST_BENCH_WIRING 0
#define START_UP_DRIVEN_SIM_VALUE 0
#define START_UP_EDGE_TYPE "RISING"
#define START_UP_FREQ 50000000
#define START_UP_HAS_IN 1
#define START_UP_HAS_OUT 0
#define START_UP_HAS_TRI 0
#define START_UP_IRQ 1
#define START_UP_IRQ_INTERRUPT_CONTROLLER_ID 0
#define START_UP_IRQ_TYPE "EDGE"
#define START_UP_NAME "/dev/start_uP"
#define START_UP_RESET_VALUE 0
#define START_UP_SPAN 16
#define START_UP_TYPE "altera_avalon_pio"


/*
 * timer_0 configuration
 *
 */

#define ALT_MODULE_CLASS_timer_0 altera_avalon_timer
#define TIMER_0_ALWAYS_RUN 0
#define TIMER_0_BASE 0x46000
#define TIMER_0_COUNTER_SIZE 64
#define TIMER_0_FIXED_PERIOD 0
#define TIMER_0_FREQ 50000000
#define TIMER_0_IRQ 2
#define TIMER_0_IRQ_INTERRUPT_CONTROLLER_ID 0
#define TIMER_0_LOAD_VALUE 1
#define TIMER_0_MULT 2.0E-8
#define TIMER_0_NAME "/dev/timer_0"
#define TIMER_0_PERIOD 2
#define TIMER_0_PERIOD_UNITS "clocks"
#define TIMER_0_RESET_OUTPUT 0
#define TIMER_0_SNAPSHOT 1
#define TIMER_0_SPAN 64
#define TIMER_0_TICKS_PER_SEC 25000000
#define TIMER_0_TIMEOUT_PULSE_OUTPUT 0
#define TIMER_0_TYPE "altera_avalon_timer"


/*
 * uart_0 configuration
 *
 */

#define ALT_MODULE_CLASS_uart_0 altera_avalon_uart
#define UART_0_BASE 0x46060
#define UART_0_BAUD 115200
#define UART_0_DATA_BITS 8
#define UART_0_FIXED_BAUD 1
#define UART_0_FREQ 50000000
#define UART_0_IRQ 4
#define UART_0_IRQ_INTERRUPT_CONTROLLER_ID 0
#define UART_0_NAME "/dev/uart_0"
#define UART_0_PARITY 'N'
#define UART_0_SIM_CHAR_STREAM ""
#define UART_0_SIM_TRUE_BAUD 0
#define UART_0_SPAN 32
#define UART_0_STOP_BITS 1
#define UART_0_SYNC_REG_DEPTH 2
#define UART_0_TYPE "altera_avalon_uart"
#define UART_0_USE_CTS_RTS 0
#define UART_0_USE_EOP_REGISTER 0

#endif /* __SYSTEM_H_ */
