CC ?= gcc
CFLAGS := -std=c99 -O3 -Wall -Wextra -Wconversion -Werror

.PHONY: all verify clean

all: build/dandelion-v5 build/libdandelion_successor_v5_0_0.so

build/dandelion-v5: src/dandelion_successor_v5_0_0.c
	@mkdir -p build
	$(CC) $(CFLAGS) $< -o $@

build/libdandelion_successor_v5_0_0.so: src/dandelion_successor_v5_0_0.c
	@mkdir -p build
	$(CC) $(CFLAGS) -fPIC -shared $< -o $@

verify: all
	./build/dandelion-v5
	python3 madras/dandelion_communicator_madras_v5_0_0.py
	python3 benchmarks/nadene_bergson_channel_benchmark_v1_0_0.py --seed 42 --trials 200
	python3 benchmarks/fidel_dene_shingari_256byte_codec_v1_0_0.py --seed 42

clean:
	rm -rf build
