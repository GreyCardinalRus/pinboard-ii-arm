#/*
#    FreeRTOS V7.1.0 - Copyright (C) 2011 Real Time Engineers Ltd.
#
#    ***************************************************************************
#    *                                                                         *
#    * If you are:                                                             *
#    *                                                                         *
#    *    + New to FreeRTOS,                                                   *
#    *    + Wanting to learn FreeRTOS or multitasking in general quickly       *
#    *    + Looking for basic training,                                        *
#    *    + Wanting to improve your FreeRTOS skills and productivity           *
#    *                                                                         *
#    * then take a look at the FreeRTOS books - available as PDF or paperback  *
#    *                                                                         *
#    *        "Using the FreeRTOS Real Time Kernel - a Practical Guide"        *
#    *                  http://www.FreeRTOS.org/Documentation                  *
#    *                                                                         *
#    * A pdf reference manual is also available.  Both are usually delivered   *
#    * to your inbox within 20 minutes to two hours when purchased between 8am *
#    * and 8pm GMT (although please allow up to 24 hours in case of            *
#    * exceptional circumstances).  Thank you for your support!                *
#    *                                                                         *
#    ***************************************************************************
#
#    This file is part of the FreeRTOS distribution.
#
#    FreeRTOS is free software; you can redistribute it and/or modify it under
#    the terms of the GNU General Public License (version 2) as published by the
#    Free Software Foundation AND MODIFIED BY the FreeRTOS exception.
#    ***NOTE*** The exception to the GPL is included to allow you to distribute
#    a combined work that includes FreeRTOS without being obliged to provide the
#    source code for proprietary components outside of the FreeRTOS kernel.
#    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT
#    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
#    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
#    more details. You should have received a copy of the GNU General Public 
#    License and the FreeRTOS license exception along with FreeRTOS; if not it 
#    can be viewed here: http://www.freertos.org/a00114.html and also obtained 
#    by writing to Richard Barry, contact details for whom are available on the
#    FreeRTOS WEB site.
#
#    1 tab == 4 spaces!
#
#    http://www.FreeRTOS.org - Documentation, latest information, license and
#    contact details.
#
#    http://www.SafeRTOS.com - A version that is certified for use in safety
#    critical systems.
#
#    http://www.OpenRTOS.com - Commercial support, development, porting,
#    licensing and training services.
#*/


#/************************************************************************* 
# * Please ensure to read http://www.freertos.org/portLM3Sxxxx_Eclipse.html
# * which provides information on configuring and running this demo for the
# * various Luminary Micro EKs.
# *************************************************************************/

############# program name
TARGET = 4-Debug
 	
# program version
VER_MAJOR = 0
VER_MINOR = 1

# compile options
OPTIMIZE = -O2
USE_LTO = NO
 	
# Select family
MCU = cortex-m3
# STM32F10X_LD : STM32 Low density devices
# STM32F10X_LD_VL : STM32 Low density Value Line devices
# STM32F10X_MD : STM32 Medium density devices
# STM32F10X_MD_VL : STM32 Medium density Value Line devices
# STM32F10X_HD : STM32 High density devices
# STM32F10X_HD_VL : STM32 XL-density devices
# STM32F10X_CL : STM32 Connectivity line devices
# STM32F10X_XL : STM32 XL-density devices
CHIP = STM32F10X_MD_VL

#MCU = cortex-m4
#STM32F407VG
	
#defines
DEFS = -D$(CHIP)
DEFS += -DVER_MAJOR=$(VER_MAJOR)
DEFS += -DVER_MINOR=$(VER_MINOR)
 	
FLASHER = openocd

CC=arm-none-eabi-gcc
LD=arm-none-eabi-gcc
AR=arm-none-eabi-ar
AS=arm-none-eabi-as
#CP=arm-none-eabi-objcopy
#OD=arm-none-eabi-objdump

# dirs
	BASE = .
 	SRCDIR = $(BASE)/src
	RTOS_ROOT=../FreeRTOS

	RTOS_SOURCE_DIR=$(RTOS_ROOT)/Source
	DEMO_COMMON_DIR=$(RTOS_ROOT)/Demo/Common/Minimal
	DEMO_INCLUDE_DIR=$(RTOS_ROOT)/Demo/Common/include
	UIP_COMMON_DIR=$(RTOS_ROOT)/Demo/Common/ethernet/uIP/uip-1.0/uip 	
	
	CMSIS_DRIVER_DIR=../MyARMLib/CMSIS/include
	#STM32F1_DISCOVERY_DRIVER_DIR=../MyARMLib/STM32/STM32F4_discovery
	STM32F10_DRIVER_DIR=../MyARMLib/STM32/STM32F10x/include
	STM32F10x_StdPeriph_DRIVER_DIR=../MyARMLib/STM32/STM32F10x_StdPeriph_Driver/inc

 	OBJDIR = $(BASE)/obj
 	EXEDIR = $(BASE)/exe
 	LSTDIR = $(BASE)/lst
 	PRJDIR = $(BASE)/prj
 	BAKDIR = $(BASE)/bak
 	
#files
 	HEX = $(EXEDIR)/$(TARGET).hex
 	BIN = $(EXEDIR)/$(TARGET).bin
 	ELF = $(EXEDIR)/$(TARGET).elf
 	MAP = $(LSTDIR)/$(TARGET).map
 	LSS = $(LSTDIR)/$(TARGET).lss
 	OK = $(EXEDIR)/$(TARGET).ok
 	
# linker script (chip dependent)
 	LD_SCRIPT = $(PRJDIR)/$(CHIP).ld
 	
# source directories (all *.c, *.cpp and *.S files included)
 	DIRS := $(SRCDIR)
 	DIRS += $(COMMON)
 	DIRS += $(SRCDIR)/debug
 	DIRS += $(SCMDIR)/Common 
 	DIRS += $(SCMDIR)/CortexM3
 	DIRS += $(SCMDIR)/Extensions/Profiler 
 	
# includes
 	INCS := $(patsubst %, -I "%", $(DIRS))

# individual source files
 	SRCS := 

#calc obj files list
 	OBJS := $(SRCS)
 	OBJS += $(wildcard $(addsuffix /*.cpp, $(DIRS)))
 	OBJS += $(wildcard $(addsuffix /*.c, $(DIRS)))
 	OBJS += $(wildcard $(addsuffix /*.S, $(DIRS)))
 	OBJS := $(notdir $(OBJS))
 	OBJS := $(OBJS:.cpp=.o)
 	OBJS := $(OBJS:.c=.o)
 	OBJS := $(OBJS:.S=.o)
 	OBJS := $(patsubst %, $(OBJDIR)/%, $(OBJS))

#files to archive
	ARCFILES	= \
		$(SRCDIR) \
		$(PRJDIR) \
		$(SCMDIR) \
		$(BASE)/makefile \
		$(BASE)/.cproject \
		$(BASE)/.project

# flags
	FLAGS	= -mcpu=$(MCU) -mthumb
	FLAGS	+= $(INCS)
	FLAGS	+= -MD
	FLAGS	+= $(DEFS) 
	FLAGS	+= -Wa,-adhlns=$(addprefix $(LSTDIR)/, $(notdir $(addsuffix .lst, $(basename $<))))

	AFLAGS	= $(FLAGS)

	CFLAGS	= $(FLAGS)
	CFLAGS	+= $(OPTIMIZE)
	CFLAGS	+= -std=gnu99
	CFLAGS	+= -g
	CFLAGS	+= -ffunction-sections -fdata-sections
	CFLAGS	+= -Wall -Wextra
	CFLAGS	+= -Wimplicit -Wcast-align -Wpointer-arith -Wredundant-decls
	CFLAGS	+= -Wshadow -Wcast-qual -Wcast-align -Wnested-externs -pedantic

	CXXFLAGS	= $(FLAGS)
	CXXFLAGS	+= $(OPTIMIZE)
	CXXFLAGS	+= -g
	CXXFLAGS	+= -fno-exceptions -fno-rtti
	CXXFLAGS	+= -ffunction-sections -fdata-sections
	CXXFLAGS	+= -fno-threadsafe-statics
	CXXFLAGS	+= -funsigned-bitfields -fshort-enums
	CXXFLAGS	+= -Wall -Wextra
	CXXFLAGS	+= -Winline
	CXXFLAGS	+= -Wpointer-arith -Wredundant-decls
	CXXFLAGS	+= -Wshadow -Wcast-qual -Wcast-align -pedantic

	LD_FLAGS	= -mcpu=$(MCU)
	LD_FLAGS	+= -mthumb
	LD_FLAGS	+= -nostartfiles 
	LD_FLAGS	+= -Wl,-Map="$(MAP)",--cref
	LD_FLAGS	+= -Wl,--gc-sections
	LD_FLAGS	+= -T$(LD_SCRIPT)

ifeq ($(USE_LTO),YES)
	CFLAGS		+= -flto
	CXXFLAGS	+= -flto
	LD_FLAGS	+= -flto $(OPTIMIZE)
endif

#openocd command-line

# debug level (d0..d3)
	oocd_params		= -d0
# interface and board/target settings (using the OOCD target-library here)
#	oocd_params		+= -c "fast enable"
	oocd_params		+= -f interface/arm-usb-ocd.cfg 
	oocd_params		+= -f board/stm32f10x_128k_eval.cfg
	oocd_params		+= -c init -c targets
	oocd_params_program	= $(oocd_params)
# commands to prepare flash-write
	oocd_params_program	+= -c "halt"
# flash-write and -verify
	oocd_params_program	+= -c "flash write_image erase $(ELF)"
	oocd_params_program	+= -c "verify_image $(ELF)"
# reset target
	oocd_params_program	+= -c "reset run"
# terminate OOCD after programming
	oocd_params_program	+= -c shutdown

	oocd_params_reset	= $(oocd_params)
	oocd_params_reset	+= -c "reset run"
	oocd_params_reset	+= -c shutdown

.SILENT :

.PHONY: all start dirs build clean program reset archive

############# targets

all : start dirs $(ELF) $(BIN) $(LSS) $(OK)

build: clean all

start:
	@echo --- building $(TARGET)

$(LSS): $(ELF) makefile
	@echo --- making asm-lst...
#	@$(OBJDUMP) -dStC $(ELF) > $(LSS)
	@$(OBJDUMP) -dC $(ELF) > $(LSS)

$(OK): $(ELF)
	@$(SIZE) $(ELF)
	@echo "Errors: none"

$(ELF):	$(OBJS) makefile
	@echo --- linking...
	$(LD) $(OBJS) $(LIBS) $(LD_FLAGS) -o "$(ELF)"

$(HEX): $(ELF)
	@echo --- make hex...
	@$(OBJCOPY) -O ihex $(ELF) $(HEX)
	
$(BIN): $(ELF)
	@echo --- make binary...
	@$(OBJCOPY) -O binary $(ELF) $(BIN)

program: $(ELF)
	@echo "Programming with OPENOCD"
	$(FLASHER) $(oocd_params_program)

reset:
	@echo Resetting device
	$(FLASHER) $(oocd_params_reset)

VPATH := $(DIRS)

$(OBJDIR)/%.o: %.cpp makefile
	@echo --- compiling $<...
	$(CXX) -c $(CXXFLAGS) -o $@ $<

$(OBJDIR)/%.o: %.c makefile
	@echo --- compiling $<...
	$(CC) -c $(CFLAGS) -o $@ $<

$(OBJDIR)/%.o: %.S makefile
	@echo --- assembling $<...
	$(AS) -c $(AFLAGS) -o $@ $<

dirs: $(OBJDIR) $(EXEDIR) $(LSTDIR) $(BAKDIR)

$(OBJDIR):
	-@$(MD) $(OBJDIR)

$(EXEDIR):
	-@$(MD) $(EXEDIR)

$(LSTDIR):
	-@$(MD) $(LSTDIR)

$(BAKDIR):
	-@$(MD) $(BAKDIR)

clean:
	-@$(RM) $(OBJDIR)/*.d 2>/dev/null
	-@$(RM) $(OBJDIR)/*.o 2>/dev/null
	-@$(RM) $(LSTDIR)/*.lst 2>/dev/null
	-@$(RM) $(ELF)
	-@$(RM) $(HEX)
	-@$(RM) $(LSS)
	-@$(RM) $(MAP)
	
archive:
	@echo --- archiving...
	7z a $(BAKDIR)/$(TARGET)_`date +%Y-%m-%d,%H-%M-%S` $(ARCFILES)
	@echo --- done!

# dependencies
ifeq (,$(findstring build,$(MAKECMDGOALS)))
 ifeq (,$(findstring clean,$(MAKECMDGOALS)))
  ifeq (,$(findstring dirs,$(MAKECMDGOALS)))
  -include $(wildcard $(OBJDIR)/*.d) 
  endif
 endif
endif
		


OBJCOPY=arm-none-eabi-objcopy
#LDSCRIPT=$(SRC_DIR)/standalone.ld
#LDSCRIPT=$(SRC_DIR)/STM32F407xG.ld
#LDSCRIPT=$(SRC_DIR)/stm32_flash.ld
# should use --gc-sections but the debugger does not seem to be able to cope with the option.
LINKER_FLAGS=-nostartfiles -Xlinker -oRTOSDemo.axf -Xlinker -M -Xlinker -Map=rtosdemo.map -Xlinker --no-gc-sections


CFLAGS=$(DEBUG) \
		-I $(SRC_DIR) -I $(RTOS_SOURCE_DIR)/include -I $(RTOS_SOURCE_DIR)/portable/GCC/ARM_CM3 \
		-I $(DEMO_INCLUDE_DIR) \
		-D USE_STDPERIPH_DRIVER \
		-D STM32F10X_MD \
		-D inline= -mthumb \
		-mcpu=$(MCU) $(OPTIM) -T$(LDSCRIPT) \
		-D PACK_STRUCT_END=__attribute\(\(packed\)\)  \
		-D ALIGN_STRUCT_END=__attribute\(\(aligned\(4\)\)\) \
		-D sprintf=usprintf -D snprintf=usnprintf -D printf=uipprintf \
		-I $(UIP_COMMON_DIR)  -ffunction-sections -fdata-sections -I $(STM32F10_DRIVER_DIR) \
		-I $(STM32F10x_StdPeriph_DRIVER_DIR)  -I $(CMSIS_DRIVER_DIR) \
		-DTARGET_IS_TEMPEST_RB1

#		../../Utilities/STM32F4-Discovery/stm32f4_discovery_audio_codec.c 
SOURCE=	 $(SRC_DIR)/system_stm32f10x.c\
		$(RTOS_SOURCE_DIR)/list.c \
		$(RTOS_SOURCE_DIR)/timers.c \
		$(RTOS_SOURCE_DIR)/queue.c \
		$(RTOS_SOURCE_DIR)/tasks.c \
		$(RTOS_SOURCE_DIR)/portable/GCC/ARM_CM3/port.c \
		$(RTOS_SOURCE_DIR)/portable/MemMang/heap_2.c\
		../MyARMLib/STM32/STM32F10x_StdPeriph_Driver/src/stm32f10x_gpio.c \
		../MyARMLib/STM32/STM32F10x_StdPeriph_Driver/src/stm32f10x_rcc.c \
		$(SRC_DIR)/main.c
#		../MyARMLib/STM32/STM32F10x_StdPeriph_Driver/src/stm32f10x_syscfg.c \
#		../MyARMLib/STM32/STM32F4xx_StdPeriph_Driver/src/misc.c \
#		../MyARMLib/STM32/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_adc.c \
#		../MyARMLib/STM32/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_dma.c \
#		../MyARMLib/STM32/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_exti.c \
#		../MyARMLib/STM32/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_flash.c \
#		../MyARMLib/STM32/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_gpio.c \
#		../MyARMLib/STM32/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_i2c.c \
#		../MyARMLib/STM32/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rcc.c \
#		../MyARMLib/STM32/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_spi.c \
#		../MyARMLib/STM32/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_tim.c \
#		../MyARMLib/STM32/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_dac.c \

LIBS= 
#$(LUMINARY_DRIVER_DIR)/gcc/libdriver.a $(LUMINARY_DRIVER_DIR)/gcc/libgr.a

OBJS = $(SOURCE:.c=.o)

all: RTOSDemo.bin
	 
RTOSDemo.bin : RTOSDemo.axf
	$(OBJCOPY) RTOSDemo.axf -O binary RTOSDemo.bin

#RTOSDemo.axf : $(OBJS) $(SRC_DIR)/startup_stm32f10x_md.o Makefile
#	$(CC) $(CFLAGS) $(OBJS) $(SRC_DIR)/startup_stm32f10x_md.o $(LIBS) $(LINKER_FLAGS)

RTOSDemo.axf : $(OBJS)  Makefile
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) $(LINKER_FLAGS)

$(OBJS) : %.o : %.c Makefile $(SRC_DIR)/FreeRTOSConfig.h
	$(CC) -c $(CFLAGS) $< -o $@

#%.o : $(SRC_DIR)/%.c
#        $(CC) -c $(CFLAGS) $< -o $@
        
startup_stm32f10x.o : $(SRC_DIR)/startup_stm32f10x_md.s Makefile
	$(CC) -c $(CFLAGS) -O1 $(SRC_DIR)/startup_stm3210x_md.s -o $(SRC_DIR)/startup_stm3210x_md.o
		
clean :
	touch Makefile
	cs-rm -f $(OBJS) $(SRC_DIR)/startup_stm32f10x.o rtosdemo.map RTOSDemo.axf $(SRC_DIR)/RTOSDemo.bin
	



