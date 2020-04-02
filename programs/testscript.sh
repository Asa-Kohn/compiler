#!/bin/bash

echo "building winnipeg"
cd ..
. build.sh
cd programs

for folder in `ls all-testers/M2`
do
	echo "Entering folder ${folder}"

	cd "all-testers/M2/${folder}"
	for file in `ls`
	do
		# echo "${file}"
		echo "Testing ${file} on winnipeg"
		../../../../src/winnipeg typecheck < "${file}"
	done
	cd ..
	cd ..
	cd ..

	echo " "
done
