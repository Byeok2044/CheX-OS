ASM      = nasm
ASMFLAGS = -f elf32
CC       = gcc
CFLAGS   = -m32 -ffreestanding -fno-stack-protector -nostdlib -Wall -Wextra

OBJS = boot/boot.o \
       kernel/io.o \
       kernel/vga.o \
       kernel/keyboard.o \
       kernel/shell.o \
       kernel/kernel.o

all: dirs chex-os.iso

dirs:
	mkdir -p boot kernel

boot/boot.o: Boot/Boot.asm
	$(ASM) $(ASMFLAGS) $< -o $@

kernel/io.o: kernel/io.asm
	$(ASM) $(ASMFLAGS) $< -o $@

kernel/kernel.o: kernel/kernel.c
	$(CC) $(CFLAGS) -c $< -o $@

kernel/vga.o: kernel/vga.c
	$(CC) $(CFLAGS) -c $< -o $@

kernel/keyboard.o: kernel/keyboard.c
	$(CC) $(CFLAGS) -c $< -o $@

kernel/shell.o: kernel/shell.c
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
	rm -f $(OBJS) kernel.bin chex-os.iso
	rm -rf boot kernel iso