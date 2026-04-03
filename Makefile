CC = cc
CFLAGS = -Wall -Wextra

BUILD = build
SOURCES = src/main.c\
	  src/pul.c\
	  src/binary.c\
	  src/crc32.c

release:
	$(CC) $(CFLAGS) $(SOURCES) -DCRC32_FAST -o $(BUILD)/catapult

debug:
	$(CC) $(CFLAGS) -g -DDEBUG -DCRC32_FAST $(SOURCES) -o $(BUILD)/catapult

test:
	cd tests && ./test1.sh

config:
	mkdir $(BUILD)

clean:
	rm -rf $(BUILD)/*
