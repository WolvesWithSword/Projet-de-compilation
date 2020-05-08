#!/bin/bash
for file in $(ls $1)
do
	echo -e "###################################################### "$file
	./$2 $1/$file
	echo -e "\n"
done
