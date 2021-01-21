#!/bin/bash

#
# Tema1 Test Suite
#
# 2020, Operating Systems
#

# ----------------- General declarations and util functions ------------------ #
INPUT_DIR=_test/inputs
REF_DIR=_test/ref
OUT_DIR=_test/outputs
EXEC_NAME=./so-cpp

max_points=95

TEST_LIB=_test/test_lib.sh

MEMCHECK=""
CPP="cpp -P"
[ $(uname -s) == "Linux" ]
IS_LINUX=$?

if [ $IS_LINUX -eq 0 ]; then
	MEMCHECK="valgrind --leak-check=full \
		--show-reachable=yes \
		--vex-iropt-register-updates=allregs-at-mem-access \
		--show-leak-kinds=all \
		--error-exitcode=1 \
		$MEMCHECK_EXTRA \
		--log-file=_log "
		else
			MEMCHECK="drmemory -batch \
				-exit_code_if_errors 1 \
				-quiet \
				$MEMCHECK_EXTRA \
				-- "
fi


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

	unset malloc_limit
	unset calloc_limit
	unset realloc_limit

	input_f=$INPUT_DIR"/test"$test_index".in"
	param_f=$INPUT_DIR"/test"$test_index".param"
	ref_f=$OUT_DIR"/test"$test_index".ref"
	out_f=$OUT_DIR"/test"$test_index".out"
	params="$(test -f $param_f && cat $param_f || echo "")"
}

cleanup_test()
{
	rm -f $out_f
}

init_world()
{
	print_header "Testing - SO Preprocessor"
	mkdir -p $OUT_DIR
}

cleanup_world()
{
	rm -rf $OUT_DIR &> /dev/null
}

# ---------------------------------------------------------------------------- #

# ----------------- Test Suite ----------------------------------------------- #

test_cpp()
{
	init_test
	$CPP $params $input_f > $ref_f
	$MEMCHECK $EXEC_NAME $params $input_f > $out_f
	mem_res=$?
	basic_test compare $out_f $ref_f
	memory_test $mem_res
	cleanup_test
}

test_cpp_stdin()
{
	init_test
	$CPP $params < $input_f > $ref_f
	$MEMCHECK $EXEC_NAME $params < $input_f > $out_f
	mem_res=$?
	basic_test compare $out_f $ref_f
	memory_test $mem_res
	cleanup_test
}

test_cpp_stdout()
{
	init_test
	$CPP $params $input_f $ref_f
	$MEMCHECK $EXEC_NAME $params $input_f $out_f
	mem_res=$?
	basic_test compare $out_f $ref_f
	memory_test $mem_res
	cleanup_test
}

test_bad_params()
{
	init_test
	$EXEC_NAME $params 2> /dev/null
	basic_test test $? -ne 0
	cleanup_test
}

run_until_success()
{
	REF_CODE=12 # ENOMEM
	NR_RUNS=1000 # How many times to run

	for ((i = 0; i < $NR_RUNS; i++)); do
		init_test
		export "$1_limit"=$i
		if [ $IS_LINUX -eq 0 ]; then
			LD_PRELOAD="./libso.so" $EXEC_NAME $params $input_f > $out_f
		else
			"./run.exe" "$EXEC_NAME $params $input_f" > $out_f
		fi

		retcode=$?

		# If there is no error then the output file must be checked
		if test $retcode -eq 0; then
			return $?
		fi

		if test $retcode -ne $REF_CODE; then
			basic_test false
			return -1
		fi

		if test $1 != "realloc"; then
			cleanup_test
		fi

	done
	basic_test false
	return -1
}

test_so_alloc()
{
	if [ $IS_LINUX -eq 0 ]; then
		lines=$(find . -maxdepth 1 -name "libso.so" | wc -l)
	else
		lines=$(find . -maxdepth 1 -name "run.exe" | wc -l)
	fi

	if [ $lines -eq 0 ]; then
		basic_test false
	else

		init_test
		$CPP $input_f $params > $ref_f

		run_until_success "malloc"
		if test $? -ne 0; then
			return
		fi

		run_until_success "calloc"
		if test $? -ne 0; then
			return
		fi

		run_until_success "realloc"

		basic_test compare $out_f $ref_f

		cleanup_test
	fi
}



test_fun_array=(                                                         \
	test_coding_style       "Sources check"                     5   0    \
	test_bad_params         "Test bad input"                    1   0    \
	test_bad_params         "Test bad parameters"               1   0    \
	test_bad_params         "Test multiple files"               1   0    \
	test_cpp_stdin          "Test stdin file"                   1   1    \
	test_cpp                "Test simple file"                  1   1    \
	test_cpp_stdout         "Test simple out file"              1   1    \
	test_cpp                "Test simple define"                2   1    \
	test_cpp                "Test param define"                 2   1    \
	test_cpp                "Test simple define/undef"          2   1    \
	test_cpp                "Test multiple defines"             2   1    \
	test_cpp                "Test nested defines"               3   1    \
	test_cpp                "Test multi-lines defines"          2   1    \
	test_cpp                "Test simple if true"               1   1    \
	test_cpp                "Test simple if false"              1   1    \
	test_cpp                "Test define if true"               1   1    \
	test_cpp                "Test define if false"              1   1    \
	test_cpp                "Test simple if-else true"          1   1    \
	test_cpp                "Test simple if-else false"         1   1    \
	test_cpp                "Test define if-else true"          1   1    \
	test_cpp                "Test define param if-else false"   1   1    \
	test_cpp                "Test simple elif false"            1   1    \
	test_cpp                "Test simple elif true"             1   1    \
	test_cpp                "Test simple elif-else true"        1   1    \
	test_cpp                "Test simple elif-else false"       1   1    \
	test_cpp                "Test complex if-elif-else"         2   1    \
	test_cpp                "Test simple ifdef false"           1   1    \
	test_cpp                "Test simple ifdef true"            1   1    \
	test_cpp                "Test simple ifndef"                1   1    \
	test_cpp                "Test simple ifndef undef"          1   1    \
	test_bad_params         "Test bad include"                  1   0    \
	test_cpp                "Test double define params"         1   1    \
	test_cpp                "Test simple include"               2   1    \
	test_cpp                "Test include directory"            1   1    \
	test_cpp                "Test include param"                1   1    \
	test_cpp                "Test include order"                1   1    \
	test_cpp                "Test include order no main"        1   1    \
	test_cpp                "Test include guard"                2   1    \
	test_so_alloc           "Test everything"                   10  0    \
)

# ---------------------------------------------------------------------------- #

# ----------------- Run test ------------------------------------------------- #

# first we check if we have everything defined
check_tests

# run tests
run_tests $@

exit 0
