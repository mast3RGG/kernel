CC = gcc
AS = nasm
LD = ld

CFLAGS = -m64 -c -ffreestanding -Wall -Wextra -mno-red-zone -fstack-protector-all
ASFLAGS = -f elf64
LDFLAGS = -m elf_x86_64 --no-warn-rwx-segments -n -T linker.ld

all: kernel.iso

boot.o: boot.asm
	$(AS) $(ASFLAGS) boot.asm -o boot.o

kernel.o: kernel.c
	$(CC) $(CFLAGS) kernel.c -o kernel.o

mmap.o: mmap.c
	$(CC) $(CFLAGS) mmap.c -o mmap.o

pmm.o: pmm.c
	$(CC) $(CFLAGS) pmm.c -o pmm.o
mystdio.o: mystdio.c
	$(CC) $(CFLAGS) mystdio.c -o mystdio.o

kernel.bin: boot.o kernel.o mmap.o pmm.o mystdio.o
	$(LD) $(LDFLAGS) boot.o kernel.o mmap.o pmm.o mystdio.o -o kernel.bin

kernel.iso: kernel.bin grub.cfg
	mkdir -p isodir/boot/grub
	cp kernel.bin isodir/boot/kernel.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o kernel.iso isodir

run: kernel.iso
	qemu-system-x86_64 -cdrom kernel.iso -d int,cpu_reset -no-reboot -no-shutdown

clean:
	rm -rf *.o kernel.bin kernel.iso isodir
