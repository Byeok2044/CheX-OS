ASM      = nasm
CC       = gcc
LD       = ld

ASMFLAGS = -f elf32
CFLAGS   = -m32 -ffreestanding -fno-stack-protector -nostdlib -nostdinc \
           -fno-builtin -Wall -Wextra -Ikernel

OBJS = boot/boot.o \
       kernel/kernel.o \
       kernel/IO.o \
       kernel/VGA.o \
       kernel/Keyboard.o \
       kernel/Shell.o

all: chex-os.iso

# ── Boot ─────────────────────────────────────────────
boot/boot.o: Boot/Boot.asm
	$(ASM) $(ASMFLAGS) $< -o $@

# ── Kernel ASM ───────────────────────────────────────
kernel/kernel.o: Kernel/Kernel.asm
	@mkdir -p kernel
	$(ASM) $(ASMFLAGS) $< -o $@

kernel/IO.o: Kernel/IO.asm
	$(ASM) $(ASMFLAGS) $< -o $@

# ── Kernel C sources ─────────────────────────────────
kernel/VGA.o: Kernel/VGA.c Kernel/VGA.h
	$(CC) $(CFLAGS) -c $< -o $@

kernel/Keyboard.o: Kernel/Keyboard.c Kernel/Keyboard.h
	$(CC) $(CFLAGS) -c $< -o $@

kernel/Shell.o: Kernel/Shell.c Kernel/Shell.h
	$(CC) $(CFLAGS) -c $< -o $@

# ── Link ─────────────────────────────────────────────
kernel.bin: $(OBJS)
	$(LD) -m elf_i386 -T linker.ld -o $@ $^

# ── ISO ──────────────────────────────────────────────
chex-os.iso: kernel.bin
	mkdir -p iso/boot/grub
	cp kernel.bin iso/boot/
	cp ISO/Boot/Grub/Grub.cfg iso/boot/grub/grub.cfg
	grub-mkrescue -o $@ iso/

# ── Run ──────────────────────────────────────────────
run: chex-os.iso
	qemu-system-i386 -cdrom chex-os.iso

# ── Clean ────────────────────────────────────────────
clean:
	rm -rf kernel/ boot/*.o kernel.bin chex-os.iso iso/