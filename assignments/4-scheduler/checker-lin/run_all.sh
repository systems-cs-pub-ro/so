#!/bin/bash

script=run_test
max_points=95
timeout=30
REF_FILE="output.ref"
CHECKPATCH_URL="https://raw.githubusercontent.com/torvalds/linux/master/scripts/checkpatch.pl"
CHECKPATCH_IGNORE_FLAGS="
        SPLIT_STRING,SSCANF_TO_KSTRTO,NEW_TYPEDEFS,VOLATILE,INLINE,USE_FUNC,AVOID_EXTERNS,CONST_STRUCT"
CHECKPATCH_ARGS="
        --ignore SPDX_LICENSE_TAG
        --no-tree
        --no-summary
        --terse
        --ignore $CHECKPATCH_IGNORE_FLAGS
        --show-types"

MEMCHECK_ERR_CODE=100
MEMCHECK_EXTRA=""
MEMCHECK="valgrind --leak-check=full \
        --show-reachable=yes \
        --vex-iropt-register-updates=allregs-at-mem-access \
        --show-leak-kinds=all \
        --error-exitcode=$MEMCHECK_ERR_CODE \
        $MEMCHECK_EXTRA \
        --log-file=_log "

check_source()
{
        # check to see if we have checkpatch
        check_patch=$(which checkpatch.pl 2> /dev/null)
        if ! [ -x "$check_patch" ]; then
                echo "'checkpatch.pl' tool not found on your system."\
                        "Skipping source check..."
                echo "Please download 'checkpatch.pl' from '$CHECKPATCH_URL'"\
                        "and install it in your \$PATH."
                echo
                return
        fi

        # try to find sources in the current directory
        src_files="$(find "${SRC_DIR:-.}" -type f -iregex \
                '.*\.\(c\|h\|cpp\|hpp\|cc\|hh\|cxx\|hxx\)')"
        if [ -z "$src_files" ]; then
                read -t 60 -e -p 'Please provide path to your sources: ' SRC_DIR
                if [ "$?" -ne "0" -o -z "$SRC_DIR" ]; then
                        echo -e "No path provided! Skipping source check...\n"
                        return
                fi
                if ! [ -e "$SRC_DIR" ]; then
                        echo -e "File or directory '$SRC_DIR' does not exist. " \
                                "Skipping source check...\n"
                        return
                fi
                src_files="$(find "$SRC_DIR" -type f -iregex \
                        '.*\.\(c\|h\|cpp\|hpp\|cc\|hh\|cxx\|hxx\)')"
                if [ -z "$src_files" ]; then
                        echo -e "No sources found in '$SRC_DIR'. " \
                                "Skipping source check...\n"
                        return
                fi
        fi
        # now we have sources in $SRC_DIR or .
        OUT=$(find "${SRC_DIR:-.}" -type f -iregex \
                '.*\.\(c\|h\|cpp\|hpp\|cc\|hh\|cxx\|hxx\)' | \
                xargs $check_patch $CHECKPATCH_ARGS -f 2>&1 | tail -n +3 | \
                sort -u -t":" -k4,4  | head -n 20)
        echo "$OUT"
        printf "00) Sources check..........................................." | tee check_source_result.txt
        if [ -z "$OUT" ]; then
                printf "passed  [05/$max_points]\n" | tee check_source_result.txt
        else
                printf "failed  [00/$max_points]\n" | tee check_source_result.txt
        fi
}

print_header()
{
     header="${1}"
     header_len=${#header}
     printf "\n"
     if [ $header_len -lt 71 ]; then
         padding=$(((71 - $header_len) / 2))
         for ((i = 0; i < $padding; i++)); do
             printf " "
         done
     fi
     printf "= %s =\n\n" "${header}"
}

test_do_fail()
{
    printf "failed  [ 0/%02d]\n" "$max_points"
}

test_do_pass()
{
    printf "passed  [%02d/%02d]\n" "$1" "$max_points"
}

init_world()
{
    print_header "Testing - SO Scheduler"
}

test_print()
{
    res=$1

    printf "%02d) %s" "$test_index" "$description"

    for ((i = 0; i < 56 - ${#description}; i++)); do
        printf "."
    done

    if test $res -eq 0; then
        test_do_pass "$points"
    else
        test_do_fail "$points"
    fi
}

init_world

# Call init to set up testing environment
timeout $timeout ./_test/"$script" init

# Check the sources using checkpatch
check_source

init_test()
{
        rm -rf $REF_FILE
}

PASS=0
FAIL=1
TESTS_SKIP_MEMCHECK=(15 16 17 21) # skip round robin and stress tests

test_sched()
{
    init_test
    skip_memcheck=false

    for i in "${TESTS_SKIP_MEMCHECK[@]}"; do
        if [ $i -eq $test_index ]; then
            skip_memcheck=true
            break
        fi
    done

    if [ "$skip_memcheck" = "true" ]; then
        timeout $timeout ./_test/"$script" $test_index &> /dev/null
    else
        timeout $timeout $MEMCHECK ./_test/"$script" $test_index &> /dev/null
    fi

    RUN_RC=$?

    if [ $RUN_RC -eq 124 ]; then
        description="$description timeout"
        test_print $FAIL
        return
    fi

    if [[ -f $REF_FILE && $(< $REF_FILE) == "1" ]]; then
        points=$points
        test_print $PASS

        if [ $skip_memcheck == false ]; then
            description="$description memcheck"
            points=$mem_points

            if [ $RUN_RC -eq $MEMCHECK_ERR_CODE ]; then
                test_print $FAIL
            else
                test_print $PASS
            fi
        fi
    else
        test_print $FAIL
    fi
}

test_fun_array=(                                                      \
        test_sched      "Test init params"                      1   1 \
        test_sched      "Test init"                             1   1 \
        test_sched      "Test double init"                      1   1 \
        test_sched      "Test reinit"                           1   1 \
        test_sched      "Test end before init"                  1   1 \
        test_sched      "Test fork param"                       1   1 \
        test_sched      "Test fork handler"                     1   1 \
        test_sched      "Test fork priority"                    1   1 \
        test_sched      "Test fork thread id"                   1   1 \
        test_sched      "Test multiple fork"                    2   1 \
        test_sched      "Test multiple fork thread ids"         2   1 \
        test_sched      "Test exec no preemption"               2   1 \
        test_sched      "Test exec preemption"                  3   1 \
        test_sched      "Test exec multiple"                    5   1 \
        test_sched      "Test exec priorities"                  5   1 \
        test_sched      "Test round robin (exec < quantum)"     2   0 \
        test_sched      "Test round robin (exec > quantum)"     2   0 \
        test_sched      "Test round robin (stress test)"        10  0 \
        test_sched      "Test IO devices"                       1   1 \
        test_sched      "Test IO schedule"                      7   1 \
        test_sched      "Test priorities and IO"                10  1 \
        test_sched      "Test priorities and IO (stress test)"  12  0 \
)

last_test=$((${#test_fun_array[@]} / 4))

for test_index in $(seq 0 $last_test); do

    arr_index=$(($test_index * 4))
    description=${test_fun_array[$(($arr_index + 1))]}

        points=${test_fun_array[$(($arr_index + 2))]}
    mem_points=${test_fun_array[$(($arr_index + 3))]}

    # Run proper function
    ${test_fun_array[$(($arr_index))]}
done | tee results.txt

cat check_source_result.txt results.txt | grep -a '\[.*\]$' | awk -F '[] /[]+' '
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
timeout $timeout ./_test/"$script" cleanup
rm -f results.txt check_source_result.txt $REF_FILE
