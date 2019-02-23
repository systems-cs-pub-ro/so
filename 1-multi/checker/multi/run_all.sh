#!/bin/bash

#
# Tema1 Test Suite
#
# 2012-2018, Operating Systems
#
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.

first_test=0
last_test=16
script=./_test/run_test.sh

# Call init to set up testing environment
bash "$script" init

# check the source (disabled, part of tests now)
#bash "$script" check

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
    printf "\n%66s  [%02d/95]\n", "Total:", sum;
}'

# Cleanup testing environment
bash "$script" cleanup
rm -f results.txt
