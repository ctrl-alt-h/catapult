CC = cc
CFLAGS = -Wall -Wextra -DCRC32_FAST

BUILD = build
SOURCES = src/main.c\
	  src/pul.c\
	  src/binary.c\
	  src/crc32.c

release:
	$(CC) $(CFLAGS) $(SOURCES) -o $(BUILD)/catapult

debug:
	$(CC) $(CFLAGS) -g -DDEBUG $(SOURCES) -o $(BUILD)/catapult

test:
	cd tests && ./test1.sh

config:
	mkdir $(BUILD)

clean:
	rm -rf $(BUILD)/*
