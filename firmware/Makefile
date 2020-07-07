PREFIX ?= /usr

CC = $(PREFIX)/bin/arm-none-eabi-gcc
LD = $(PREFIX)/bin/arm-none-eabi-gcc
OBJCOPY = $(PREFIX)/bin/arm-none-eabi-objcopy

TARGET ?= efm32

BUILDDIR = build

SOURCES = \
	lrprf.c \
	lraead.c \
	main.c

INCLUDES = \
	-I.

CFLAGS = -Wall -Wextra -std=c99 -O0 -g3

include targets/$(TARGET)/config.mk
