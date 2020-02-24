
ipl:
	nasm -f bin ipl.asm -o ipl.bin
head:
	nasm -f bin head.asm -o head.bin
boot:
	gcc -m32 -ffreestanding -c boot.c -o boot.bin
image: ipl head boot
	cat ipl.bin head.bin boot.bin > lightos.img
run: image
	qemu-system-i386 -fda lightos.img -boot a
clean:
	rm -f ipl.bin head.bin boot.bin lightos.img
