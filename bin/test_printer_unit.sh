#!/bin/sh

echo "start testing printer ..."

for testfile in ./test/printer/good*.bminor
do
	if ./bin/bminor --print $testfile > $testfile.a.out
	then
		if ./bin/bminor --print $testfile.a.out > $testfile.b.out
		then
			if diff $testfile.a.out $testfile.b.out > $testfile.out
			then 
				echo "$testfile success (as expected)"
			else
				echo "$testfile failure (difference found) (INCORRECT)"
			fi
		else
			echo "$testfile failure (2 parsing) (INCORRECT)"
		fi
	else
		echo "$testfile failure (1st parsing) (INCORRECT)"
	fi
done

echo "printer testing completed"
