ARCH := avr
CC := $(ARCH)-gcc
OBJCOPY := $(ARCH)-objcopy

MCU := atmega16

TARGET := target
BUILD_DIR := build
SOURCE_DIR := source

CC_SOURCE_FILES := $(wildcard $(SOURCE_DIR)/*.c)
CC_OBJECT_FILES := $(CC_SOURCE_FILES:$(SOURCE_DIR)/%.c=$(BUILD_DIR)/%.o)




all: $(BUILD_DIR) $(BUILD_DIR)/$(TARGET).hex

build:
	mkdir --parents $(BUILD_DIR)

$(BUILD_DIR)/$(TARGET).hex: $(BUILD_DIR)/$(TARGET).elf
	$(OBJCOPY) -O ihex $^ $@

$(BUILD_DIR)/$(TARGET).elf: $(CC_OBJECT_FILES)
	$(CC) -mmcu=$(MCU) -Os $(CC_OBJECT_FILES) -o $@

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c
	$(CC) -mmcu=$(MCU) -Os -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

flash: all
	avrdude -p $(MCU) -P /dev/ttyACM0 -c avrispv2 -U flash:w:$(BUILD_DIR)/$(TARGET).hex
