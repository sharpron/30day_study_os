pwd=$(shell pwd)

PREFIX:=docker run --rm -v $(pwd):/usr/src/myapp -w /usr/src/myapp gcc:9.2
gcc:=$(PREFIX) gcc 
ld:=$(PREFIX) ld

output_img:=gos.img

ipl.bin: ipl.asm
	nasm -f bin $^ -o $@ 
# 连接内核
kernel.bin: kernel.o funcs.o graphic.o ui.o fifo8.o interrupt.o font.o \
	mouse.o format.o mem.o layer.o
	$(ld) -m elf_i386 -Ttext 0x7e00 $^ -o $@ --oformat binary
# 制作软盘镜像
gos.img: ipl.bin kernel.bin
	cat $^ > $@

# 编译成.o 文件
%.o: %.c
	$(gcc) -m32 -Wall -Wextra -std=c99 -ffreestanding -c $*.c -o $*.o

%.o: %.asm
	nasm -f elf $*.asm -o $*.o

# 其它指令
img:
	make gos.img
run: 
	make img
	qemu-system-i386 -m 32M -fda $(output_img) -boot a
clean:
	rm -f *.bin
	rm -f *.o
	rm -f $(output_img)
