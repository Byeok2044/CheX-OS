ASM      = nasm
ASMFLAGS = -f elf32
CC       = gcc
CFLAGS   = -m32 -ffreestanding -fno-stack-protector -nostdlib -Wall -Wextra

OBJS = build/boot.o build/io.o build/vga.o build/keyboard.o build/shell.o build/kernel.o

all: build/. chex-os.iso

build/.:
	mkdir -p build iso/boot/grub

build/boot.o: Boot/Boot.asm
	$(ASM) $(ASMFLAGS) $< -o $@

build/io.o: kernel/io.asm
	$(ASM) $(ASMFLAGS) $< -o $@

build/vga.o: kernel/vga.c
	$(CC) $(CFLAGS) -c $< -o $@

build/keyboard.o: kernel/keyboard.c
	$(CC) $(CFLAGS) -c $< -o $@

build/shell.o: kernel/shell.c
	$(CC) $(CFLAGS) -c $< -o $@

build/kernel.o: kernel/kernel.c
	$(CC) $(CFLAGS) -c $< -o $@

kernel.bin: $(OBJS)
	ld -m elf_i386 -T linker.ld -o $@ $^

chex-os.iso: kernel.bin
	mkdir -p iso/boot/grub
	cp kernel.bin iso/boot/
	cp grub.cfg iso/boot/grub/grub.cfg
	grub-mkrescue -o $@ iso/

run: chex-os.iso
	qemu-system-i386 -cdrom chex-os.iso

clean:
	rm -rf build iso kernel.bin chex-os.iso
