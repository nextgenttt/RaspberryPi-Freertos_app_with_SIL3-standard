#
#	FreeRTOS portable layer for RaspberryPi
#
OBJECTS += $(BUILD_DIR)FreeRTOS/Source/portable/GCC/RaspberryPi/port.o
OBJECTS += $(BUILD_DIR)FreeRTOS/Source/portable/GCC/RaspberryPi/portisr.o

#
#	FreeRTOS Core
#
OBJECTS += $(BUILD_DIR)FreeRTOS/Source/croutine.o
OBJECTS += $(BUILD_DIR)FreeRTOS/Source/list.o
OBJECTS += $(BUILD_DIR)FreeRTOS/Source/queue.o
OBJECTS += $(BUILD_DIR)FreeRTOS/Source/tasks.o
OBJECTS += $(BUILD_DIR)FreeRTOS/Source/event_groups.o

#
#	Interrupt Manager & GPIO Drivers
#
OBJECTS += $(BUILD_DIR)Drivers/interrupts.o
OBJECTS += $(BUILD_DIR)Drivers/gpio.o
OBJECTS += $(BUILD_DIR)Drivers/mini_uart.o
OBJECTS += $(BUILD_DIR)Drivers/mem.o


$(BUILD_DIR)FreeRTOS/Source/portable/GCC/RaspberryPi/port.o: CFLAGS += -I $(BASE)Demo/

OBJECTS += $(BUILD_DIR)FreeRTOS/Source/portable/MemMang/heap_4.o
OBJECTS += $(BUILD_DIR)Demo/startup.o
OBJECTS += $(BUILD_DIR)Demo/main.o



