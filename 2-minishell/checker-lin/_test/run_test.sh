#!/bin/bash

#
# Tema2 Test Suite
#
# 2012, Operating Systems
#

# ----------------- General declarations and util functions ------------------ #

exec_name="mini-shell"
ref_name="bash"

MAIN_TEST_DIR="_test/outputs"
INPUT_DIR="_test/inputs"
REFS_DIR="_test/refs"
LOG_FILE="/dev/null"
max_points=90
TEST_TIMEOUT=30

TEST_LIB=_test/test_lib.sh
CHECK_SKIP='parser.\(\(tab\|yy\).\(c\|h\)\|h\)$'

# load the lib functions
if ! [ -e "$TEST_LIB" ]; then
	echo "Test library not found. Check \$TEST_LIB ($TEST_LIB)"
	exit 1
fi
source "$TEST_LIB"

SHELL_PROMPT="> "

# change this of you want to keep the logs after execution
DO_CLEANUP=yes



# ---------------------------------------------------------------------------- #

# ----------------- Init and cleanup tests ----------------------------------- #

# Initializes a test
init_test()
{

	if ! [ -e "$exec_name" ]; then
		echo "$exec_name not found! Cannot run the test"
		exit 1
	fi

	# generates random file as test file
	TEST_NAME=$(printf "test_%02d" $test_index)
	OUT_DIR="${TEST_NAME}_out"
	REF_DIR="${TEST_NAME}_ref"
	IN_FILE="${TEST_NAME}.in"
	REF_FILE="${TEST_NAME}.ref"
	OUT_FILE="${TEST_NAME}.out"

	# copy input files in the testing directory
	cp ${INPUT_DIR}/${TEST_NAME}.txt ${MAIN_TEST_DIR}/${IN_FILE} &> $LOG_FILE
	# copy additional files, if they exist (see test 04)
	cp ${INPUT_DIR}/${TEST_NAME}?*.txt ${MAIN_TEST_DIR} &> $LOG_FILE
	# copy any reference files, if they exist (see test 18)
	cp ${REFS_DIR}/${TEST_NAME}.ref ${MAIN_TEST_DIR} &> $LOG_FILE

	# export mini-shell in path
	PATH="$PATH:$(pwd)"
	cd ${MAIN_TEST_DIR} &> $LOG_FILE

	[ -d "${OUT_DIR}" ] && rm -rf "${OUT_DIR}"
	[ -d "${REF_DIR}" ] && rm -rf "${REF_DIR}"
}

# Cleanups a test
cleanup_test()
{
	if [ "$DO_CLEANUP" = "yes" ]; then
		[ -d "${OUT_DIR}" ] && rm -rf "${OUT_DIR}"
		[ -d "${REF_DIR}" ] && rm -rf "${REF_DIR}"
		[ -e "${IN_BASH}" ] && rm "${IN_BASH}"
		[ -e "${IN_MINI}" ] && rm "${IN_MINI}"
	fi
}

# Initializes the whole testing environment
# Should be the first test called
init_world()
{
	[ -d $MAIN_TEST_DIR ] && rm -rf $MAIN_TEST_DIR
	mkdir -p ${MAIN_TEST_DIR}
	print_header "Testing - mini-shell"
}

# Cleanups the whole testing environment
# Should be the last test called
cleanup_world()
{
	[ "$DO_CLEANUP" = "yes" ] && rm -rf $MAIN_TEST_DIR
}

# ---------------------------------------------------------------------------- #

# ----------------- Test Suite ----------------------------------------------- #

# Specific test example
execute_cmd()
{
	EXEC=$1
	INPUT=$2
	OUTPUT=$3
	[ -z "$OUTPUT" ] && OUTPUT=$LOG_FILE
	mkdir -p ${OUT_DIR} && cd ${OUT_DIR} &> $LOG_FILE
	timeout $TEST_TIMEOUT $EXEC < "${INPUT}" &> $OUTPUT
	# Ocasionally, in a virtualized environment, the diff that compares the
	# target implementation with the reference output starts before the
	# target implementation has finished writing files to disk. Handle this
	# by mandating that all I/O buffers be flushed.
	sync
	cd - &> $LOG_FILE
}


generate_input()
{
	# generate the PROMPT for BASH
	sed 's/\(.*\)/printf \"> \"; \1/' ${IN_FILE} > ${REF_FILE}
}

# checks the output of the commands
test_output()
{
	init_test

	generate_input

	# commands to execute the test
	execute_cmd $ref_name "../${REF_FILE}" "${OUT_FILE}"
	# move OUT_DIR in order to preserve the pwd output
	mv ${OUT_DIR} ${REF_DIR}
	execute_cmd $exec_name "../${IN_FILE}" "${OUT_FILE}"

	# test output
	basic_test diff -r -ui ${REF_DIR} ${OUT_DIR}

	cleanup_test
}

# tests common commands
test_common()
{
	init_test

	# commands to execute the test
	execute_cmd $ref_name "../${IN_FILE}"
	# move OUT_DIR in order to preserve the pwd output
	mv ${OUT_DIR} ${REF_DIR}
	execute_cmd $exec_name "../${IN_FILE}"

	# test output
	basic_test diff -r -ui ${REF_DIR} ${OUT_DIR}

	cleanup_test
}

# test 18 - tests environment variables
test_exec_failed()
{
	init_test

	# commands to execute the test
	execute_cmd $exec_name "../${IN_FILE}" "${REF_FILE}"

	# test output
	basic_test diff -r -uib ${REF_FILE} ${OUT_DIR}/${REF_FILE}

	cleanup_test
}


test_fun_array=(								\
	test_output		"Testing commands without arguments"	3	\
	test_output		"Testing commands with arguments"	2	\
	test_common		"Testing simple redirect operators"	5	\
	test_common		"Testing append redirect operators"	5	\
	test_common		"Testing current directory"		5	\
	test_common		"Testing conditional operators"		5	\
	test_common		"Testing sequential commands"		3	\
	test_common		"Testing environment variables"		5	\
	test_common		"Testing single pipe"			5	\
	test_common		"Testing multiple pipes"		10	\
	test_common		"Testing variables and redirect"	5	\
	test_common		"Testing overwritten variables"		2	\
	test_common		"Testing all operators"			2	\
	test_common		"Testing parallel operator"		10	\
	test_common		"Testing big file"			5	\
	test_common		"Testing sleep command"			7	\
	test_common		"Testing fscanf function"		7	\
	test_exec_failed	"Testing unknown command"		4	\
)


# ---------------------------------------------------------------------------- #

# ----------------- Run test ------------------------------------------------- #

# first we check if we have everything defined
check_tests

# run tests
run_tests $@

exit 0
