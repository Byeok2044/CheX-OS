ASM = nasm
ASMFLAGS = -f elf32

all: chex-os.iso

boot/boot.o: boot/boot.asm
	$(ASM) $(ASMFLAGS) $< -o $@

kernel/kernel.o: kernel/kernel.asm
	$(ASM) $(ASMFLAGS) $< -o $@

kernel.bin: boot/boot.o kernel/kernel.o
	ld -m elf_i386 -T linker.ld -o $@ $^

chex-os.iso: kernel.bin
	mkdir -p iso/boot/grub
	cp kernel.bin iso/boot/
	grub-mkrescue -o $@ iso/

run: chex-os.iso
	qemu-system-i386 -cdrom $

clean:
	rm -f **/*.o kernel.bin chex-os.iso