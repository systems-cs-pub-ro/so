#!/bin/bash

OUT_FILE=out
IN_FILE=input
BIN=./splice
TIME=`which time`

function clear_file
{
	rm -f $OUT_FILE
}

function clear_cache
{
	sync
	echo 1 > /proc/sys/vm/drop_caches
}

function print
{
	echo -e "Physical support:\t$1\tIn cache:\t$2\tOutput exists:\t$3"
}

function run_ram
{
	$TIME -f %E cat /dev/zero | $BIN > /dev/null
}

function run_phys
{
	$TIME -f %E cat $IN_FILE | $BIN > /dev/null
}

clear_cache
clear_file

print 0 0 0
run_ram
echo

clear_cache
print 0 0 1
run_ram
echo

clear_file
print 0 1 0
run_ram
echo

print 0 1 1
run_ram
echo

echo "Generate input file"
cat /dev/zero | $BIN > /dev/null
mv $OUT_FILE $IN_FILE

clear_cache
clear_file

print 1 0 0
run_phys
echo

clear_cache
print 1 0 1
run_phys
echo

clear_file
print 1 1 0
run_phys
echo

print 1 1 1
run_phys
echo

clear_file
rm -f $IN_FILE
