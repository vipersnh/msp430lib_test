CC=msp430-gcc
CXX = msp430-g++
LD=msp430-ld
OBJ_CCOPY=msp430-objcopy
DEBUG=-g
OPTIMIZATION = -O1
MCU = msp430f5438
# Setup paths to source code
LIB_PATH = .
FREE_RTOS_PATH = ..
SOURCE_PATH =  $(FREE_RTOS_PATH)/Source
PORT_PATH = $(SOURCE_PATH)/portable/GCC/MSP430F449
SDCARD_LIB = $(LIB_PATH)/SDCardFat32Lib
SERIAL_SRCS = $(LIB_PATH)/uart/uart_A3.cpp $(LIB_PATH)/uart/uart.cpp
SERIAL_HEADS = $(LIB_PATH)/uart/

#
CFLAGS= -fomit-frame-pointer -mmcu=$(MCU) $(DEBUG) $(OPTIMIZATION) -I. -I$(SOURCE_PATH)/include -I$(FREE_RTOS_PATH)/Common/include -I$(LIB_PATH)/SDCardFat32Lib/ -I$(LIB_PATH)/SDCardFat32Lib/option/ -DGCC_MSP430  -I$(SERIAL_HEADS) 

CXXFLAGS= -fomit-frame-pointer -mmcu=$(MCU) $(DEBUG) $(OPTIMIZATION) -I. -I$(SOURCE_PATH)/include -I$(FREE_RTOS_PATH)/Common/include -I$(LIB_PATH)/SDCardFat32Lib/ -I$(LIB_PATH)/SDCardFat32Lib/option/ -DGCC_MSP430 -I$(SERIAL_HEADS)  

# Source files that can be built to THUMB mode.
#
SRC_C = \
$(SOURCE_PATH)/tasks.c \
$(SOURCE_PATH)/list.c \
$(SOURCE_PATH)/queue.c \
$(SOURCE_PATH)/portable/MemMang/heap_1.c \
$(PORT_PATH)/port.c \
$(SERIAL_SRC_CS) \

#$(SDCARD_LIB)/diskio.c \
#$(SDCARD_LIB)/ff.c \


SRC_CXX = \
./main.cpp

OBJ_C = $(SRC_C:.c=.o)
OBJ_CXX = $(SRC_CXX:.cpp=.o)
OBJ_UART = ./uart/uart.o ./uart/uart_A3.o
TARGET:  $(OBJ_UART) $(OBJ_C) $(OBJ_CXX) LINK GENERATE_LST GENERATE_HEX

$(OBJ_CXX): $(SRC_CXX) makefile $(OBJ_C)
	@echo
	@echo "Executing C++ compilation"
	$(CXX) -c $(CXXFLAGS)  $< -o $@ -nodefaultlibs -fno-rtti -fno-exceptions -lc -lgcc

$(OBJ_UART): %.o : %.cpp makefile
	@echo
	@echo "Executing Serial UART source compilation"
	$(CXX) -c $(CXXFLAGS) $< -o $@ -nodefaultlibs -fno-rtti -fno-exceptions -lc -lgcc


$(OBJ_C) : %.o : %.c makefile
	@echo
	@echo "Executing C files compilation"
	$(CC) -c  $(CFLAGS) $< -o $@	

LINK : $(OBJ_CXX) $(OBJ_C) $(OBJ_UART)
	@echo
	@echo "Linking files"
	$(CXX) -mmcu=$(MCU) $(OBJ_C) $(OBJ_CXX) $(OBJ_UART) -o main_linked.elf -Wl,-Map=main_linked.map -nodefaultlibs -fno-rtti -fno-exceptions -lc -lgcc -fomit-frame-pointer
	
GENERATE_LST: main_linked.elf
	msp430-objdump -dSt main_linked.elf > main_linked.lst

GENERATE_HEX: main_linked.elf
	msp430-objcopy -O ihex main_linked.elf main_linked.hex

prog: main_linked.hex
	@echo
	@echo
	@echo
	@echo *************************************************************
	@echo Programming MSP430
	mspdebug rf2500 'prog main_linked.hex' 'gdb'
	
	@echo Program done
	@echo 
	@echo
	@echo
	@echo *************************************************************


.PHONY: clean debug DISP
clean :
	rm -rf `find ../../ -name "*.elf"`
	rm -rf `find ../../ -name "*.o"`
	rm -rf `find ../../ -name "*.hex"`
	rm -rf `find ../../ -name "*.lst"`
	rm -rf `find ../../ -name "*.out"`
	rm -rf `find ../../ -name "*.i"`
	rm -rf `find ../../ -name "*.ii"`
	rm -rf `find ../../ -name "*.s"`

disp:
	@echo $(OBJ_CXX)

debug:
	mspdebug rf2500 'gdb'

gdb:
	msp430-gdb ./main_linked.elf




	


