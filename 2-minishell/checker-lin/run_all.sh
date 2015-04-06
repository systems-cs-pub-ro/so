#!/bin/bash

#
# Tema1 Test Suite
#
# 2012, Operating Systems
#

first_test=1
last_test=18
script=./_test/run_test.sh

# Call init to set up testing environment
bash "$script" init

# check the source
bash "$script" check

for i in $(seq $first_test $last_test); do
   bash "$script" $i
done | tee results.txt

cat results.txt | grep -a '\[.*\]$' | awk -F '[] /[]+' '
BEGIN {
    sum=0
}

{
	sum += $(NF-2);
}

END {
    printf "\n%66s  [%02d/90]\n", "Total:", sum;
}'

# Cleanup testing environment
bash "$script" cleanup
rm -f results.txt
