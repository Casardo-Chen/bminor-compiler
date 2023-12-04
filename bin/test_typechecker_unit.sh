#!/bin/sh

echo "start testing typechecker ..."

for testfile in ./test/typecheck/good*.bminor
do
	if ./bin/bminor --typecheck $testfile > $testfile.out
	then
		echo "$testfile success (as expected)"
	else
		echo "$testfile failure (INCORRECT)"
	fi
done

for testfile in ./test/typecheck/bad*.bminor
do
	if ./bin/bminor --typecheck $testfile > $testfile.out
	then
		echo "$testfile success (INCORRECT)"
	else
		echo "$testfile failure (as expected)"
	fi
done

echo "typechecker testing completed"
