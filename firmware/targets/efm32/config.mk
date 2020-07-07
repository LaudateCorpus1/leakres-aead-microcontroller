EFM32_BUILDDIR = $(BUILDDIR)/efm32

EFM32_INCLUDES = \
	$(INCLUDES) \
	-I./targets/efm32 \
	-I./targets/efm32/CMSIS/Include \
	-I./targets/efm32/Device/SiliconLabs/EFM32PG12B/Include \
	-I./targets/efm32/emlib/inc

EFM32_SOURCES = \
	$(SOURCES) \
	targets/efm32/Device/SiliconLabs/EFM32PG12B/Source/system_efm32pg12b.c \
	targets/efm32/Device/SiliconLabs/EFM32PG12B/Source/GCC/startup_efm32pg12b.S \
	targets/efm32/emlib/src/em_assert.c \
	targets/efm32/emlib/src/em_system.c \
	targets/efm32/emlib/src/em_cmu.c \
	targets/efm32/emlib/src/em_gpio.c \
	targets/efm32/emlib/src/em_core.c \
	targets/efm32/emlib/src/em_crypto.c \
	targets/efm32/device.c \
	targets/efm32/sha256.c \
	targets/efm32/aes.c

EFM32_OBJECTS = $(addprefix $(EFM32_BUILDDIR)/, $(addsuffix .o, $(basename $(EFM32_SOURCES))))

EFM32_DEVICE = EFM32PG12B500F1024GL125
EFM32_MCU_FLAGS = -D$(EFM32_DEVICE) -mcpu=cortex-m4 -mthumb \
	-mfloat-abi=softfp -mfpu=fpv4-sp-d16 -ffunction-sections \
	-fdata-sections -fomit-frame-pointer

EFM32_CFLAGS = $(CFLAGS) $(EFM32_MCU_FLAGS) $(EFM32_INCLUDES) \
	-fmessage-length=0

EFM32_LDSCRIPT = targets/efm32/Device/SiliconLabs/EFM32PG12B/Source/GCC/efm32pg12b.ld

EFM32_LINK_LIBS = -Wl,--start-group -lgcc -lc -lnosys   -Wl,--end-group
EFM32_LDFLAGS = $(EFM32_MCU_FLAGS) -T$(EFM32_LDSCRIPT) \
	-Wl,-Map=$(EFM32_BUILDDIR)/output.map \
	-Wl,--gc-sections $(EFM32_LINK_LIBS)

EFM32_ELF = $(EFM32_BUILDDIR)/firmware.elf
EFM32_HEX = $(EFM32_BUILDDIR)/firmware.hex
EFM32_BIN = $(EFM32_BUILDDIR)/firmware.bin

$(EFM32_ELF): $(EFM32_OBJECTS)
	$(LD) $(EFM32_LDFLAGS) -o $@ $(EFM32_OBJECTS) $(EFM32_LDLIBS)

$(EFM32_BUILDDIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) -c $(EFM32_CFLAGS) $< -o $@

$(EFM32_BUILDDIR)/%.o: %.s
	mkdir -p $(dir $@)
	$(CC) -c $(EFM32_CFLAGS) $< -o $@

clean:
	rm -rf $(EFM32_BUILDDIR)

distclean: clean
