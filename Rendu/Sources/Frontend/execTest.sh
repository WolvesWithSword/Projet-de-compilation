#!/bin/bash
for file in $(find $1 -name "*.c"   | grep -v '\_backend.c$')
do
	echo -e "########### "$file" ###########"
	./$2 $file
	echo -e "\n"
done
