STM32_BUILDDIR = $(BUILDDIR)/stm32

STM32_INCLUDES = \
	$(INCLUDES) \
	-I./targets/stm32 \
	-I./targets/stm32/STM32CubeF2/Drivers/STM32F2xx_HAL_Driver/Inc \
	-I./targets/stm32/STM32CubeF2/Drivers/CMSIS/Include \
	-I./targets/stm32/STM32CubeF2/Drivers/CMSIS/Device/ST/STM32F2xx/Include \
	-I./targets/stm32/tinycrypt/lib/include

STM32_SOURCES = \
	$(SOURCES) \
	targets/stm32/startup_stm32f215xx.s \
	targets/stm32/stm32f2xx_it.c \
	targets/stm32/system_stm32f2xx.c \
	targets/stm32/stm32f2xx_hal_msp.c \
	targets/stm32/STM32CubeF2/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal.c \
	targets/stm32/STM32CubeF2/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_cortex.c \
	targets/stm32/STM32CubeF2/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_gpio.c \
	targets/stm32/STM32CubeF2/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_cryp.c \
	targets/stm32/device.c \
	targets/stm32/aes.c \
	targets/stm32/sha256.c \
	targets/stm32/tinycrypt/lib/source/utils.c \
	targets/stm32/tinycrypt/lib/source/sha256.c

STM32_OBJECTS = $(addprefix $(STM32_BUILDDIR)/, $(addsuffix .o, $(basename $(STM32_SOURCES))))

STM32_DEVICE = STM32F215xx
STM32_MCU_FLAGS = -D$(STM32_DEVICE) -DUSE_HAL_DRIVER \
	-mcpu=cortex-m3 -mthumb -ffunction-sections -fdata-sections \
	-fomit-frame-pointer

STM32_CFLAGS = $(CFLAGS) $(STM32_MCU_FLAGS) $(STM32_INCLUDES) \
	-fmessage-length=0

STM32_LDSCRIPT = targets/stm32/STM32F215RGTx_FLASH.ld

STM32_LINK_LIBS = -Wl,--start-group -lgcc -lc -lnosys   -Wl,--end-group
STM32_LDFLAGS = $(STM32_MCU_FLAGS) -T$(STM32_LDSCRIPT) \
	-Wl,-Map=$(STM32_BUILDDIR)/output.map \
	-Wl,--gc-sections $(STM32_LINK_LIBS)

STM32_ELF = $(STM32_BUILDDIR)/firmware.elf

all: $(STM32_ELF)

targets/stm32/tinycrypt/Makefile:
	git submodule update --init -- targets/stm32/tinycrypt

targets/stm32/STM32CubeF2/README.md:
	git submodule update --init -- targets/stm32/STM32CubeF2

$(STM32_ELF): targets/stm32/tinycrypt/Makefile targets/stm32/STM32CubeF2/README.md $(STM32_OBJECTS)
	$(LD) $(STM32_LDFLAGS) -o $@ $(STM32_OBJECTS)

$(STM32_BUILDDIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) -c $(STM32_CFLAGS) $< -o $@

$(STM32_BUILDDIR)/%.o: %.s
	mkdir -p $(dir $@)
	$(CC) -c $(STM32_CFLAGS) $< -o $@

clean:
	rm -rf $(STM32_BUILDDIR)

distclean: clean
	git submodule deinit -f -- targets/stm32/tinycrypt
	git submodule deinit -f -- targets/stm32/STM32CubeF2
