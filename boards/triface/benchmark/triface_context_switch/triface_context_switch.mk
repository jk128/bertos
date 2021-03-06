#
# Wizard autogenerated makefile.
# DO NOT EDIT, use the triface_context_switch_user.mk file instead.
#

# Constants automatically defined by the selected modules


# Our target application
TRG += triface_context_switch

triface_context_switch_PREFIX = "/usr/bin/avr-"

triface_context_switch_SUFFIX = ""

triface_context_switch_SRC_PATH = boards/triface/benchmark/triface_context_switch

triface_context_switch_HW_PATH = boards/triface

# Files automatically generated by the wizard. DO NOT EDIT, USE triface_context_switch_USER_CSRC INSTEAD!
triface_context_switch_WIZARD_CSRC = \
	bertos/benchmark/context_switch.c \
	bertos/cpu/avr/drv/ser_avr.c \
	bertos/cpu/avr/drv/ser_mega.c \
	bertos/cpu/avr/drv/timer_avr.c \
	bertos/cpu/avr/drv/timer_mega.c \
	bertos/drv/ser.c \
	bertos/drv/timer.c \
	bertos/io/kfile.c \
	bertos/kern/proc.c \
	bertos/kern/signal.c \
	bertos/mware/event.c \
	bertos/mware/formatwr.c \
	bertos/mware/hex.c \
	bertos/struct/heap.c \
	#

# Files automatically generated by the wizard. DO NOT EDIT, USE triface_context_switch_USER_PCSRC INSTEAD!
triface_context_switch_WIZARD_PCSRC = \
	bertos/mware/formatwr.c \
	#

# Files automatically generated by the wizard. DO NOT EDIT, USE triface_context_switch_USER_CPPASRC INSTEAD!
triface_context_switch_WIZARD_CPPASRC = \
	bertos/cpu/avr/hw/switch_ctx_avr.S \
	#

# Files automatically generated by the wizard. DO NOT EDIT, USE triface_context_switch_USER_CXXSRC INSTEAD!
triface_context_switch_WIZARD_CXXSRC = \
	 \
	#

# Files automatically generated by the wizard. DO NOT EDIT, USE triface_context_switch_USER_ASRC INSTEAD!
triface_context_switch_WIZARD_ASRC = \
	 \
	#

triface_context_switch_CPPFLAGS = -D'CPU_FREQ=(14745600UL)' -D'ARCH=(ARCH_DEFAULT)' -D'WIZ_AUTOGEN' -I$(triface_context_switch_HW_PATH) -I$(triface_context_switch_SRC_PATH) $(triface_context_switch_CPU_CPPFLAGS) $(triface_context_switch_USER_CPPFLAGS)

# Automatically generated by the wizard. PLEASE DO NOT EDIT!
triface_context_switch_LDFLAGS = $(triface_context_switch_CPU_LDFLAGS) $(triface_context_switch_WIZARD_LDFLAGS) $(triface_context_switch_USER_LDFLAGS)

# Automatically generated by the wizard. PLEASE DO NOT EDIT!
triface_context_switch_CPPAFLAGS = $(triface_context_switch_CPU_CPPAFLAGS) $(triface_context_switch_WIZARD_CPPAFLAGS) $(triface_context_switch_USER_CPPAFLAGS)

# Automatically generated by the wizard. PLEASE DO NOT EDIT!
triface_context_switch_CSRC = $(triface_context_switch_CPU_CSRC) $(triface_context_switch_WIZARD_CSRC) $(triface_context_switch_USER_CSRC)

# Automatically generated by the wizard. PLEASE DO NOT EDIT!
triface_context_switch_PCSRC = $(triface_context_switch_CPU_PCSRC) $(triface_context_switch_WIZARD_PCSRC) $(triface_context_switch_USER_PCSRC)

# Automatically generated by the wizard. PLEASE DO NOT EDIT!
triface_context_switch_CPPASRC = $(triface_context_switch_CPU_CPPASRC) $(triface_context_switch_WIZARD_CPPASRC) $(triface_context_switch_USER_CPPASRC)

# Automatically generated by the wizard. PLEASE DO NOT EDIT!
triface_context_switch_CXXSRC = $(triface_context_switch_CPU_CXXSRC) $(triface_context_switch_WIZARD_CXXSRC) $(triface_context_switch_USER_CXXSRC)

# Automatically generated by the wizard. PLEASE DO NOT EDIT!
triface_context_switch_ASRC = $(triface_context_switch_CPU_ASRC) $(triface_context_switch_WIZARD_ASRC) $(triface_context_switch_USER_ASRC)

# CPU specific flags and options, defined in the CPU definition files.
# Automatically generated by the wizard. PLEASE DO NOT EDIT!
triface_context_switch_MCU = atmega1281
triface_context_switch_CPU_CPPFLAGS = -Os -Ibertos/cpu/avr/
triface_context_switch_PROGRAMMER_CPU = atmega1281
triface_context_switch_STOPFLASH_SCRIPT = bertos/prg_scripts/avr/stopflash.sh
triface_context_switch_STOPDEBUG_SCRIPT = bertos/prg_scripts/none.sh
triface_context_switch_DEBUG_SCRIPT = bertos/prg_scripts/nodebug.sh
triface_context_switch_FLASH_SCRIPT = bertos/prg_scripts/avr/flash.sh

include $(triface_context_switch_SRC_PATH)/triface_context_switch_user.mk
