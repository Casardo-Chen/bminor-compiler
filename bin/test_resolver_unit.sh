#!/bin/sh

echo "start testing resolver ..."

for testfile in ./test/resolve/good*.bminor
do
	if ./bin/bminor --resolve $testfile > $testfile.out
	then
		echo "$testfile success (as expected)"
	else
		echo "$testfile failure (INCORRECT)"
	fi
done

for testfile in ./test/resolve/bad*.bminor
do
	if ./bin/bminor --resolve $testfile > $testfile.out
	then
		echo "$testfile success (INCORRECT)"
	else
		echo "$testfile failure (as expected)"
	fi
done

echo "resolver testing completed"
