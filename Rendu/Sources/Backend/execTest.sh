#!/bin/bash
for file in $(find $1 -name "*_backend.c")
do
	echo -e "########### "$file" ###########"
	./$2 $file
	echo -e "\n"
done
