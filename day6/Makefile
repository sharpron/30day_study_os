
ipl.bin: ipl.asm
	nasm -f bin ipl.asm -o $@ 
head.bin: head.asm
	nasm -f bin head.asm -o $@

# 编译内核
bridge.o: bridge.asm
	nasm -f elf bridge.asm -o $@
boot.o: boot.c
	i386-elf-gcc -ffreestanding -c boot.c -o $@

# 编译字体
font.asm: font.txt compile_font.sh
	./compile_font.sh font.txt	
font.o:	font.asm
	nasm -f elf font.asm -o $@

# 连接内核
kernel.bin: bridge.o boot.o font.o
	i386-elf-ld -Ttext 0x8400 bridge.o boot.o font.o -o $@ --oformat binary

# 制作软盘镜像
lightos.img: ipl.bin head.bin kernel.bin
	cat ipl.bin head.bin kernel.bin > $@

# 其它指令
img:
	make lightos.img
run: 
	make img
	qemu-system-i386 -fda lightos.img -boot a
clean:
#	rm -f ipl.bin head.bin kernel.bin lightos.img
#	rm -f bridge.o boot.o
#	rm -f font.o font.asm
