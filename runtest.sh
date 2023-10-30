#!/bin/sh

echo "start testing ..."
# encode
echo "testing encoder ..."
for testfile in ./test/encode/good*.bminor
do
	if ./bminor --encode $testfile > $testfile.out
	then
		echo "$testfile success (as expected)"
	else
		echo "$testfile failure (INCORRECT)"
	fi
done

for testfile in ./test/encode/bad*.bminor
do
	if ./bminor --encode $testfile > $testfile.out
	then
		echo "$testfile success (INCORRECT)"
	else
		echo "$testfile failure (as expected)"
	fi
done

# scan
echo "testing scanner ..."
for testfile in ./test/scanner/good*.bminor
do
	if ./bminor --scan $testfile > $testfile.out
	then
		echo "$testfile success (as expected)"
	else
		echo "$testfile failure (INCORRECT)"
	fi
done

for testfile in ./test/scanner/bad*.bminor
do
	if ./bminor --scan $testfile > $testfile.out
	then
		echo "$testfile success (INCORRECT)"
	else
		echo "$testfile failure (as expected)"
	fi
done


# parser
echo "testing parser ..."
for testfile in ./test/parser/good*.bminor
do
	if ./bminor --parse $testfile > $testfile.out
	then
		echo "$testfile success (as expected)"
	else
		echo "$testfile failure (INCORRECT)"
	fi
done

for testfile in ./test/parser/bad*.bminor
do
	if ./bminor --parse $testfile > $testfile.out
	then
		echo "$testfile success (INCORRECT)"
	else
		echo "$testfile failure (as expected)"
	fi
done

# printer
echo "testing printer ..."
for testfile in ./test/printer/good*.bminor
do
	if ./bminor --print $testfile > $testfile.a.out
	then
		if ./bminor --print $testfile.a.out > $testfile.b.out
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

echo "testing completed"