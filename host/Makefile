CC = gcc
CFLAGS = -Wall -Wextra -g -O3 -std=gnu11
DEBUGFLAGS =
INCLUDES = -I./mbedtls.git/crypto/include
LFLAGS = -L./mbedtls.git/crypto/library -Wl,-rpath=./mbedtls.git/crypto/library
LIBS = -lmbedcrypto -lm

SRCS = $(wildcard src/*.c)

LRAEAD = lraead_gen_testvector

OBJS = $(SRCS:.c=.o)

.PHONY: clean mbedtls

all: $(LRAEAD)

$(LRAEAD): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) $(DEBUGFLAGS) -o $(LRAEAD) $(OBJS) $(LFLAGS) $(LIBS)

mbedtls:
	git submodule init
	git submodule update --init --recursive -- mbedtls.git
	cd mbedtls.git ;\
	cmake -DUSE_SHARED_MBEDTLS_LIBRARY=On ;\
	make ;\
	make test

%.o : %.c %.h
	$(CC) $(CFLAGS) $(INCLUDES) $(DEBUGFLAGS) -c $<  -o $@

clean:
	rm -rf $(OBJS) $(LRAEAD)

distclean: clean
	git submodule deinit -f -- mbedtls.git
