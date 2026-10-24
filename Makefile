# ==============================================================================
# DANDELION OS v2.7.0 ("HEIMAT-AGORA") — STANDARD GNU MAKEFILE
# ==============================================================================
CC      ?= gcc
CFLAGS  ?= -std=c99 -O3 -Wall -Wextra
LDFLAGS ?= -lm -lpthread
PYTHON  ?= python3

TARGET  := dandelion_os_grimm_fbw_v2.7.0
SRC     := https://www.google.com/search?q=dandelion\_os\_grimm\_fbw\_v2.7.0.c
USB_SH  := ./https://www.google.com/search?q=mk\_switchblade\_usb\_v2.7.0.sh
VERIFY  := verify_dandelion_os_v2.7.0.py
INITRD  := initramfs_dandelion_v2.7.0.cpio.gz

.PHONY: all build check verify shell json initramfs qemu clean help

all: build

build:
	@chmod +x $(USB_SH)
	@$(USB_SH)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

check: build
	$(PYTHON) $(VERIFY) --seed 42

verify: check

shell: $(TARGET)
	./$(TARGET) --shell

json: $(TARGET)
	./$(TARGET) --json "div compute fbw pitch trim"

initramfs: check

qemu: $(INITRD)
	qemu-system-x86_64 \
		-kernel /boot/vmlinuz-$$(uname -r) \
		-initrd $(INITRD) \
		-nographic -m 512M \
		-append "console=ttyS0,115200n8 rdinit=/init"

clean:
	rm -rf build_whiteroom_v2.7.0 build_whiteroom_v2.4.0 $(TARGET) *.aux *.log *.out __pycache__

help:
	@echo "Dandelion OS v2.7.0 ('Heimat-Agora') Build Targets:"
	@echo "  make          - Compile C99 FBW daemon and stage Switchblade Live USB RootFS"
	@echo "  make check    - Run 15-vector 9-Bite Grimm's Law audit & 100k benchmark (<=60s)"
	@echo "  make shell    - Launch interactive F-16 Fly-By-Wire shell"
	@echo "  make json     - Emit sample zero-TUI JSON telemetry packet"
	@echo "  make qemu     - Test-boot initramfs_dandelion_v2.7.0.cpio.gz in QEMU"
	@echo "  make clean    - Remove temporary build staging directories"
