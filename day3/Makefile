ipl_file = ipl.bin
os_file = lightos.sys
img_file = lightos.img

build:
	nasm -f bin ipl.asm -o $(ipl_file)
	nasm lightos.asm -o $(os_file) 
image: build
	dd if=/dev/zero of=$(img_file) bs=512 count=2880
	dd if=$(ipl_file) of=$(img_file) conv=notrunc
	dd if=$(os_file) of=$(img_file) seek=1 conv=notrunc
run: image
	qemu-system-i386 -fda $(img_file)
clean:
	rm -rf $(ipl_file)
	rm -rf $(os_file)
	rm -rf $(img_file)
