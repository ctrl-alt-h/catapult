CC = cc
CFLAGS = -Wall -Wextra -DCRC32_FAST
BUILD = ./build

release:
	cd $(BUILD) && \
	cmake .. && \
	cmake --build .

debug:
	cd $(BUILD) && \
	cmake -DCMAKE_BUILD_TYPE=Debug .. && \
	cmake --build .

config:
	mkdir -p $(BUILD)

clean:
	rm -rf $(BUILD)/*
