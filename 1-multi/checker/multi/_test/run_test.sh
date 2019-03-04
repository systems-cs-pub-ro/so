#!/bin/bash

#
# Tema1 Test Suite
#
# 2012-2019, Operating Systems
#

# ----------------- General declarations and util functions ------------------ #
INPUT_DIR=_test/inputs
REF_DIR=_test/ref
OUT_DIR=_test/outputs
EXEC_NAME=./tema1

max_points=95

TEST_LIB=_test/test_lib.sh

MEMCHECK=""
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
    ref_f=$REF_DIR"/test"$test_index".ref"
    out_f=$OUT_DIR"/test"$test_index".out"
}

cleanup_test()
{
    rm -f $out_f
}

init_world()
{
    print_header "Testing - Priority Queue"
    mkdir -p $OUT_DIR
}

cleanup_world()
{
    rm -rf $OUT_DIR &> /dev/null
}

# ---------------------------------------------------------------------------- #

# ----------------- Test Suite ----------------------------------------------- #

test_insert()
{
    init_test
    $MEMCHECK $EXEC_NAME $input_f > $out_f
    mem_res=$?
    basic_test compare $out_f $ref_f
    memory_test $mem_res
    cleanup_test
}

test_insert_stdin()
{
    init_test
    $MEMCHECK $EXEC_NAME < $input_f > $out_f
    mem_res=$?
    basic_test compare $out_f $ref_f
    memory_test $mem_res
    cleanup_test
}

test_multiple_files()
{
    init_test
    $MEMCHECK $EXEC_NAME "$input_f".{1..4} > $out_f
    mem_res=$?
    basic_test compare $out_f $ref_f
    memory_test $mem_res
    cleanup_test
}

test_top()
{
    init_test
    $MEMCHECK $EXEC_NAME < $input_f > $out_f
    mem_res=$?
    basic_test compare $out_f $ref_f
    memory_test $mem_res
    cleanup_test
}

test_pop()
{
    init_test
    $MEMCHECK $EXEC_NAME $input_f > $out_f
    mem_res=$?
    basic_test compare $out_f $ref_f
    memory_test $mem_res
    cleanup_test
}

test_empty_lines()
{
    init_test
    $MEMCHECK $EXEC_NAME $input_f > $out_f
    mem_res=$?
    basic_test compare $out_f $ref_f
    memory_test $mem_res
    cleanup_test
}

test_stress()
{
    init_test
    $MEMCHECK $EXEC_NAME $input_f > $out_f
    mem_res=$?
    basic_test compare $out_f $ref_f
    memory_test $mem_res
    cleanup_test
}

test_invalid()
{
    init_test
    $MEMCHECK $EXEC_NAME $input_f > $out_f
    mem_res=$?
    basic_test compare $out_f $ref_f
    memory_test $mem_res
    cleanup_test
}

test_ldd_compare()
{
    local libname=""
    if [ $IS_LINUX -eq 0 ]; then
        libname="libcompare.so"
    else
        libname="compare.dll"
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

run_until_success()
{
    REF_CODE=12 # ENOMEM
    NR_RUNS=1000 # How many times to run

    for ((i = 0; i < $NR_RUNS; i++)); do
        DEBUG $i
        init_test
        export "$1_limit"=$i
        if [ $IS_LINUX -eq 0 ]; then
            LD_PRELOAD="libso.so" $EXEC_NAME < $input_f > $out_f
        else
            "./run.exe" $EXEC_NAME < $input_f > $out_f
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
    fi
}



test_fun_array=(                                                         \
    test_coding_style       "Sources check"                     5   0    \
    test_insert                "Test insert file"               2   1    \
    test_insert_stdin        "Test insert stdin"                3   1    \
    test_top                "Test top"                          2   1    \
    test_pop                "Test pop"                          2   1    \
    test_empty_lines        "Test empty lines"                  2   1    \
    test_multiple_files     "Test multiple files"               4   1    \
    test_multiple_files     "Test bad file"                     2   1    \
    test_invalid            "Test invalid command"              2   1    \
    test_invalid            "Test params for insert"            2   1    \
    test_invalid            "Test params for top"               2   1    \
    test_invalid            "Test params for pop"               2   1    \
    test_stress             "Test stress 1"                     10  1    \
    test_stress             "Test stress 2"                     10  1    \
    test_stress             "Test stress 3"                     10  1    \
    test_ldd_compare        "Test dynamic link compare (ldd)"   10  0    \
    test_so_alloc           "Test malloc/calloc/realloc"        11  0    \
       )

# ---------------------------------------------------------------------------- #

# ----------------- Run test ------------------------------------------------- #

# first we check if we have everything defined
check_tests

# run tests
run_tests $@

exit 0
