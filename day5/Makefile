
ipl:
	nasm -f bin ipl.asm -o ipl.bin
head:
	nasm -f bin head.asm -o head.bin

# 编译内核
bridge:	
	nasm -f elf bridge.asm -o bridge.o
boot:
	i386-elf-gcc -ffreestanding -c boot.c -o boot.o
kernel: bridge boot
	i386-elf-ld -Ttext 8400 bridge.o boot.o -o kernel.bin --oformat binary

image: ipl head kernel
	cat ipl.bin head.bin kernel.bin > lightos.img
run: image
	qemu-system-i386 -fda lightos.img -boot a
clean:
	rm -f ipl.bin head.bin kernel.bin lightos.img
	rm -f bridge.o boot.o
