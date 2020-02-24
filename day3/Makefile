ipl_file = ipl.bin
os_file = lightos.sys
img_file = lightos.img

build:
	nasm -f bin ipl.asm -o $(ipl_file)
	nasm -f bin head.asm -o head.o
	nasm -f elf bridge.asm -o bridge.o
	i386-elf-gcc -m32 -ffreestanding -c boot.c -o boot.o
image: build
	i386-elf-ld -o main.o bridge.o boot.o
	cat head.o main.o > $(os_file)
	dd if=/dev/zero of=$(img_file) bs=512 count=2880
	dd if=$(ipl_file) of=$(img_file) conv=notrunc
	dd if=$(os_file) of=$(img_file)	seek=1 conv=notrunc

run: image
	qemu-system-i386 -fda $(img_file)
clean:
	rm -rf $(ipl_file)
	rm -rf $(os_file)
	rm -rf head.o main.o bridge.o boot.o
	rm -rf $(img_file)
