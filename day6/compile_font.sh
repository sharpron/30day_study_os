#!/bin/sh

if [[ $# -eq 1 ]]; then
	file=$1

	filename=${file%.*}

	outfile=$filename'.asm'
	# 添加汇编头
	echo '[BITS		32]' > $outfile
	echo 'GLOBAL	font' >> $outfile
	echo '[SECTION	.text]' >> $outfile
	echo 'font:' >> $outfile

	while IFS= read -r line
	do
		if [[ ${#line} -eq 0 ]]; then
			continue
		fi
		if [[ $line == "char "* ]]; then
			continue
		fi
		
		line=${line//./0}
	
		echo '\tDB	 0b'${line//\*/1} >> $outfile
	done < $file	
	
	echo 'finish compile.'
else 
	echo 'error: only support one arg'
fi

