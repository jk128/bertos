#
# Wizard autogenerated makefile.
# DO NOT EDIT, use the arduino_1wire_user.mk file instead.
#

# Constants automatically defined by the selected modules
arduino_1wire_DEBUG = 1

# Our target application
TRG += arduino_1wire

arduino_1wire_PREFIX = "avr-"

arduino_1wire_SUFFIX = ""

arduino_1wire_SRC_PATH = boards/arduino/examples/arduino_1wire

arduino_1wire_HW_PATH = boards/arduino

# Files automatically generated by the wizard. DO NOT EDIT, USE arduino_1wire_USER_CSRC INSTEAD!
arduino_1wire_WIZARD_CSRC = \
	bertos/algo/crc8.c \
	bertos/cpu/avr/drv/ser_avr.c \
	bertos/cpu/avr/drv/ser_mega.c \
	bertos/cpu/avr/drv/timer_avr.c \
	bertos/cpu/avr/drv/timer_mega.c \
	bertos/drv/ow_1wire.c \
	bertos/drv/ow_ds18x20.c \
	bertos/drv/ow_ds2413.c \
	bertos/drv/ow_ds2438.c \
	bertos/drv/ser.c \
	bertos/drv/timer.c \
	bertos/io/kfile.c \
	bertos/mware/event.c \
	bertos/mware/formatwr.c \
	bertos/mware/hex.c \
	bertos/struct/heap.c \
	#

# Files automatically generated by the wizard. DO NOT EDIT, USE arduino_1wire_USER_PCSRC INSTEAD!
arduino_1wire_WIZARD_PCSRC = \
	bertos/mware/formatwr.c \
	#

# Files automatically generated by the wizard. DO NOT EDIT, USE arduino_1wire_USER_CPPASRC INSTEAD!
arduino_1wire_WIZARD_CPPASRC = \
	 \
	#

# Files automatically generated by the wizard. DO NOT EDIT, USE arduino_1wire_USER_CXXSRC INSTEAD!
arduino_1wire_WIZARD_CXXSRC = \
	 \
	#

# Files automatically generated by the wizard. DO NOT EDIT, USE arduino_1wire_USER_ASRC INSTEAD!
arduino_1wire_WIZARD_ASRC = \
	 \
	#

arduino_1wire_CPPFLAGS = -D'CPU_FREQ=(16000000UL)' -D'ARCH=(ARCH_DEFAULT)' -D'WIZ_AUTOGEN' -I$(arduino_1wire_HW_PATH) -I$(arduino_1wire_SRC_PATH) $(arduino_1wire_CPU_CPPFLAGS) $(arduino_1wire_USER_CPPFLAGS)

# Automatically generated by the wizard. PLEASE DO NOT EDIT!
arduino_1wire_LDFLAGS = $(arduino_1wire_CPU_LDFLAGS) $(arduino_1wire_WIZARD_LDFLAGS) $(arduino_1wire_USER_LDFLAGS)

# Automatically generated by the wizard. PLEASE DO NOT EDIT!
arduino_1wire_CPPAFLAGS = $(arduino_1wire_CPU_CPPAFLAGS) $(arduino_1wire_WIZARD_CPPAFLAGS) $(arduino_1wire_USER_CPPAFLAGS)

# Automatically generated by the wizard. PLEASE DO NOT EDIT!
arduino_1wire_CSRC = $(arduino_1wire_CPU_CSRC) $(arduino_1wire_WIZARD_CSRC) $(arduino_1wire_USER_CSRC)

# Automatically generated by the wizard. PLEASE DO NOT EDIT!
arduino_1wire_PCSRC = $(arduino_1wire_CPU_PCSRC) $(arduino_1wire_WIZARD_PCSRC) $(arduino_1wire_USER_PCSRC)

# Automatically generated by the wizard. PLEASE DO NOT EDIT!
arduino_1wire_CPPASRC = $(arduino_1wire_CPU_CPPASRC) $(arduino_1wire_WIZARD_CPPASRC) $(arduino_1wire_USER_CPPASRC)

# Automatically generated by the wizard. PLEASE DO NOT EDIT!
arduino_1wire_CXXSRC = $(arduino_1wire_CPU_CXXSRC) $(arduino_1wire_WIZARD_CXXSRC) $(arduino_1wire_USER_CXXSRC)

# Automatically generated by the wizard. PLEASE DO NOT EDIT!
arduino_1wire_ASRC = $(arduino_1wire_CPU_ASRC) $(arduino_1wire_WIZARD_ASRC) $(arduino_1wire_USER_ASRC)

# CPU specific flags and options, defined in the CPU definition files.
# Automatically generated by the wizard. PLEASE DO NOT EDIT!
arduino_1wire_MCU = atmega328p
arduino_1wire_CPU_CPPFLAGS = -Os -Ibertos/cpu/avr/
arduino_1wire_PROGRAMMER_CPU = atmega328p
arduino_1wire_STOPFLASH_SCRIPT = bertos/prg_scripts/avr/stopflash.sh
arduino_1wire_STOPDEBUG_SCRIPT = bertos/prg_scripts/none.sh
arduino_1wire_DEBUG_SCRIPT = bertos/prg_scripts/nodebug.sh
arduino_1wire_FLASH_SCRIPT = bertos/prg_scripts/avr/flash.sh

include $(arduino_1wire_SRC_PATH)/arduino_1wire_user.mk
