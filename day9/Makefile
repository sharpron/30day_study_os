pwd=$(shell pwd)

PREFIX:=docker run --rm -v $(pwd):/usr/src/myapp -w /usr/src/myapp gcc:9.2
gcc:=$(PREFIX) gcc 
ld:=$(PREFIX) ld

output_img:=lightos.img

ipl.bin: ipl.asm
	nasm -f bin $^ -o $@ 
asmhead.bin: asmhead.asm
	nasm -f bin $^ -o $@ 
# 编译内核
funcs.o: funcs.asm
	nasm -f elf $^ -o $@

# 编译字体
font.asm: font.txt compile_font.sh
	./compile_font.sh $<	
font.o:	font.asm
	nasm -f elf $^ -o $@
# 连接内核
kernel.bin: boot.o graphic.o interrupt.o scancode.o fifo8.o funcs.o  font.o
	$(ld) -m elf_i386 -Ttext 0x8000 -e main $^ -o $@ --oformat binary
# 制作软盘镜像
lightos.img: ipl.bin asmhead.bin kernel.bin
	cat $^ > $@

# 编译成.o 文件
%.o: %.c
	$(gcc) -m32 -Wall -Wextra -std=c99 -ffreestanding -c $*.c -o $*.o

# 其它指令
img:
	make lightos.img
run: 
	make img
	qemu-system-i386 -fda $(output_img) -boot a
clean:
	rm -f *.bin
	rm -f *.o
	rm -f $(output_img)
	rm -f font.asm
