pwd=$(shell pwd)

PREFIX:=docker run --rm -v $(pwd):/usr/src/myapp -w /usr/src/myapp gcc:9.2
gcc:=$(PREFIX) gcc 
ld:=$(PREFIX) ld
objcopy:=$(PREFIX) objcopy

ipl.bin: ipl.asm
	nasm -f bin $^ -o $@ 
asmhead.bin: asmhead.asm
	nasm -f bin $^ -o $@ 
# 编译内核
funcs.o: funcs.asm
	nasm -f elf $^ -o $@
boot.o: boot.c
	$(gcc) -m32 -Wall -Wextra -std=gnu99 -ffreestanding -c $^ -o $@

# 编译字体
font.asm: font.txt compile_font.sh
	./compile_font.sh $<	
font.o:	font.asm
	nasm -f elf $^ -o $@
# 连接内核
kernel.bin: boot.o funcs.o font.o
	$(ld) -m elf_i386 -Ttext 0x8000 -Tdata 0x8000 -e main $^ -o $@ --oformat binary
# 制作软盘镜像
lightos.img: ipl.bin asmhead.bin kernel.bin
	cat $^ > $@

# 其它指令
img:
	make lightos.img
run: 
	make img
	qemu-system-i386 -fda lightos.img -boot a
clean:
	rm -f ipl.bin asmhead.bin kernel.bin lightos.img
	rm -f funcs.o boot.o
	rm -f font.o font.asm
