#!/bin/sh

echo "start testing scanner ..."

for testfile in ./test/scanner/good*.bminor
do
	if ./bin/bminor --scan $testfile > $testfile.out
	then
		echo "$testfile success (as expected)"
	else
		echo "$testfile failure (INCORRECT)"
	fi
done

for testfile in ./test/scanner/bad*.bminor
do
	if ./bin/bminor --scan $testfile > $testfile.out
	then
		echo "$testfile success (INCORRECT)"
	else
		echo "$testfile failure (as expected)"
	fi
done

echo "scanner testing completed"
