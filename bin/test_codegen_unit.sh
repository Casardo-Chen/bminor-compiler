#!/bin/sh

echo "start testing codegen ..."

for testfile in ./test/codegen/good*.bminor
do
	echo "----------------COMPILE OUTPUT----------------" > $testfile.out
	if ./bin/bminor --codegen $testfile $testfile.s >> $testfile.out
	then
		gcc -g $testfile.s src/library.c -o $testfile.exe
		echo -e "---------------EXECUTION OUTPUT---------------" >> $testfile.out
		./$testfile.exe >> $testfile.out
		echo -e "\n----------------MAIN STD OUTPUT---------------\n$?" >> $testfile.out
		echo "$testfile success (as expected)"
	else
		echo "$testfile failure (INCORRECT)"
	fi
done

echo "codegen testing completed"