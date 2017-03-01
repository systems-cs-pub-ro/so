#!/bin/bash

#
# Tema1 Test Suite
#
# 2012-2017, Operating Systems
#

# ----------------- General declarations and util functions ------------------ #

INPUT_DIR=_test/inputs
REF_DIR=_test/ref
OUT_DIR=_test/outputs
EXEC_NAME=./tema1
max_points=95

TEST_LIB=_test/test_lib.sh

# load the lib functions
if ! [ -e "$TEST_LIB" ]; then
	echo "Test library not found. Check \$TEST_LIB ($TEST_LIB)"
	exit 1
fi
source "$TEST_LIB"


# ---------------------------------------------------------------------------- #

# ----------------- Init and cleanup tests ----------------------------------- #

init_test()
{
	if ! [ -e "$EXEC_NAME" ]; then
		echo "$EXEC_NAME not found! Cannot run the test"
		exit 1
	fi

	input_f=$INPUT_DIR"/test"$test_index".in"
	ref_f=$REF_DIR"/test"$test_index".ref"
	out_f=$OUT_DIR"/test"$test_index".out"
}

cleanup_test()
{
	rm -f $out_f
}

init_world()
{
	print_header "Testing - HashTable"
	mkdir -p $OUT_DIR
}

cleanup_world()
{
	rm -rf $OUT_DIR &> /dev/null
}

# ---------------------------------------------------------------------------- #

# ----------------- Test Suite ----------------------------------------------- #

test_size_1()
{
	init_test
	$EXEC_NAME 1 $input_f > $out_f
	basic_test compare $out_f $ref_f
	cleanup_test
}

test_size_1_stdin()
{
	init_test
	$EXEC_NAME 1 < $input_f > $out_f
	basic_test compare $out_f $ref_f
	cleanup_test
}

test_error()
{
	init_test
	OUT="$($EXEC_NAME $(cat $input_f) 2> $out_f)"
	# we should not have anything on output, since it is an error
	basic_test test "x${OUT:+false}" = "x" -a -s $out_f
	cleanup_test
}

test_size_1_multiple()
{
	init_test
	$EXEC_NAME 1 "$input_f".[1-3] > $out_f
	basic_test compare $out_f $ref_f
	cleanup_test
}


test_size_256()
{
	init_test
	$EXEC_NAME 256 $input_f > $out_f
	basic_test compare $out_f $ref_f
	cleanup_test
}

test_size_256_stdin()
{
	init_test
	$EXEC_NAME 256 < $input_f > $out_f
	basic_test compare $out_f $ref_f
	cleanup_test
}

test_files_1()
{
	init_test
	$EXEC_NAME 256 < $input_f > $out_f
	args="compare "$out_f".1 "$ref_f".1 && compare $out_f $ref_f"
	basic_test eval $args
	cleanup_test
}

test_files_2()
{
	init_test
	$EXEC_NAME 256 < $input_f > $out_f
	args="compare "$out_f".1 "$ref_f".1 && compare "$out_f".2 "$ref_f".2 && \
		compare $out_f $ref_f"
	basic_test eval $args
	cleanup_test
}

test_halve_redirects()
{
	init_test
	FILE1="hash.out"
	REF_FILE1=$REF_DIR"/hash.ref"
	FILE2="buckets.out"
	REF_FILE2=$REF_DIR"/buckets.ref"

	$EXEC_NAME 1024 $input_f > $out_f
	args="compare $FILE1 $REF_FILE1 && compare $FILE2 $REF_FILE2 &&
		compare $out_f $ref_f"
	basic_test eval $args

	rm $FILE1 $FILE2
	cleanup_test
}

test_stress1()
{
	init_test
	$EXEC_NAME 512 $input_f > $out_f
	basic_test compare $out_f $ref_f
	cleanup_test
}

test_stress2()
{
	init_test
	$EXEC_NAME 16384 $input_f > $out_f
	basic_test compare $out_f $ref_f
	cleanup_test
}

test_stress3()
{
	init_test
	$EXEC_NAME 16384 $input_f > $out_f
	basic_test compare $out_f $ref_f
	cleanup_test
}

test_invalid1()
{
	init_test
	$EXEC_NAME 1 $input_f > $out_f
	basic_test compare $out_f $ref_f
	cleanup_test
}

test_invalid2()
{
	init_test
	$EXEC_NAME 1 $input_f > $out_f
	basic_test compare $out_f $ref_f
	cleanup_test
}

test_invalid3()
{
	init_test
	$EXEC_NAME 1 $input_f > $out_f
	basic_test compare $out_f $ref_f
	cleanup_test
}

test_ldd()
{
    local libname=""
    if [ $(uname -s) = "Linux" ]; then
    	libname="libhash.so"
    else
    	libname="hash.dll"
    fi

    init_test
    echo $libname > ldd.ref
    ldd $EXEC_NAME | grep $libname | \
        awk -F '[\t ]+' ' { print $2 } ' | \
        tr "\t" " " | tr -d " " > ldd.out
    basic_test compare ldd.out ldd.ref
    rm -f ldd.out ldd.ref
    cleanup_test
}

test_fun_array=(												\
	test_coding_style	"Sources check"				5	\
	test_size_1				"Test add file"					2	\
	test_size_1_stdin		"Test add stdin"				2	\
	test_files_1			"Test print file"				2	\
	test_error				"Test params"					2	\
	test_error				"Test bad size"					2	\
	test_size_1				"Test add simple"				3	\
	test_size_1_multiple	"Test multiple files"			3	\
	test_size_1_multiple	"Test bad file"					2	\
	test_size_256			"Test add same hash"			3	\
	test_size_256_stdin		"Test add different hash"		3	\
	test_size_256			"Test add double"				3	\
	test_size_1				"Test remove one"				2	\
	test_size_256_stdin		"Test remove same hash"			2	\
	test_size_256			"Test remove different hash"	3	\
	test_size_256_stdin		"Test remove non existing"		2	\
	test_size_256			"Test clear"					2	\
	test_size_256_stdin		"Test clear and add"			2	\
	test_size_256			"Test remove and clear"			3	\
	test_size_256_stdin		"Test find"						2	\
	test_files_1			"Test find files"				2	\
	test_size_256			"Test print bucket"				2	\
	test_files_2			"Test print bucket files"		2	\
	test_size_256_stdin		"Test halve"					4	\
	test_size_256			"Test double"					4	\
	test_size_256_stdin		"Test halve double"				4	\
	test_stress1			"Test stress 1"					4	\
	test_stress2 			"Test stress 2"					4	\
	test_stress3 			"Test stress 3"					5	\
	test_invalid1 			"Test invalid command"			2 	\
	test_invalid2 			"Test invalid argument 1" 		1 	\
	test_invalid3 			"Test invalid argument 2" 		1 	\
    test_ldd                "Test dynamic link"             10   \
	)

# ---------------------------------------------------------------------------- #

# ----------------- Run test ------------------------------------------------- #

# first we check if we have everything defined
check_tests

# run tests
run_tests $@

exit 0
