#!/bin/bash

#
# Asynchronous Web Server - test suite
#
# 2011, Operating Systems
#

# Use one try and 3 seconds timeout for wget.
alias wget='wget -t 1 -T 3'

# Enable/disable debug (1/0).
DEBUG_=1

DEBUG()
{
    if test "x$DEBUG_" = "x1"; then
        $@ 1>&2
    fi
}

max_points=90

# Enable/disable exiting when program fails.
EXIT_IF_FAIL=0

test_do_fail()
{
    points=$1
    printf "failed  [  0/%02d]\n" "$max_points"
    if test "x$EXIT_IF_FAIL" = "x1"; then
        exit 1
    fi
}

test_do_pass()
{
    points=$1
    printf "passed  [%02d/%02d]\n" "$points" "$max_points"
}

basic_test()
{
    message=$1
    points=$2
    shift 2
    test_command=$@

    printf "%02d) %s" "$test_index" "$message"

    for ((i = 0; i < 56 - ${#message}; i++)); do
        printf "."
    done

    $test_command > /dev/null 2>&1
    if test $? -eq 0; then
        test_do_pass "$points"
    else
        test_do_fail "$points"
    fi
}

exec_name="./aws.exe"
obj_name="./aws.obj"
exec_pid="-1"
aws_listen_port=8888
static_folder="./static/"
dynamic_folder="./dynamic/"

init_test()
{
    $exec_name &
    if test $? -eq 0; then
        exec_pid=$!
    fi
    sleep 1
}

cleanup_test()
{
    /usr/bin/kill -9 "$exec_pid" > /dev/null 2>&1
    wait "$exec_pid" 2>/dev/null
}

test_executable_exists()
{
    init_test

    basic_test "${FUNCNAME[0]}" 1 test -e "$exec_name"

    cleanup_test
}

test_executable_runs()
{
    init_test

    ps | grep 'aws$' > /dev/null 2>&1
    basic_test "${FUNCNAME[0]}" 1 test $? -eq 0

    cleanup_test
}

# Use netstat to see whether server is listening on proper port.
test_is_listening_on_port()
{
    init_test

    netstat /an | grep "0.0.0.0:$aws_listen_port" | grep LISTENING | awk -F '[ \t]+' '{print $3;}' | grep "$aws_listen_port" > /dev/null 2>&1
    basic_test "${FUNCNAME[0]}" 2 test "$?" -eq 0

    cleanup_test
}

# Use netcat for connecting to server.
test_accepts_connections()
{
    init_test

    nc localhost "$aws_listen_port" 0<&- &
    nc_pid=$!
    sleep 2
    netstat /an | grep ":$aws_listen_port" | grep ESTABLISHED > /dev/null 2>&1
    basic_test "${FUNCNAME[0]}" 1 test $? -eq 0

    /usr/bin/kill -9 "$nc_pid" > /dev/null 2>&1
    wait "$nc_pid" 2> /dev/null

    cleanup_test
}

test_accepts_multiple_connections()
{
    init_test

    nc localhost "$aws_listen_port" 0<&- &
    nc_pid1=$!
    nc localhost "$aws_listen_port" 0<&- &
    nc_pid2=$!
    sleep 2
    n_conns=$(netstat /an | grep ":$aws_listen_port" | grep ESTABLISHED | wc -l 2> /dev/null)
    #DEBUG echo "n_conns: $n_conns"

    basic_test "${FUNCNAME[0]}" 1 test $n_conns -eq 4

    /usr/bin/kill -9 "$nc_pid1" "$nc_pid2" > /dev/null 2>&1
    wait "$nc_pid1" "$nc_pid2" 2> /dev/null

    cleanup_test
}

# Use nm to list undefined symbols in object file.
test_uses_iocp()
{
    init_test

    nm -u "$obj_name" | grep CreateIoCompletionPort > /dev/null 2>&1
    basic_test "${FUNCNAME[0]}" 1 test $? -eq 0

    cleanup_test
}

test_disconnect()
{
    init_test

    nc localhost "$aws_listen_port" 0<&- &
    nc_pid=$!

    sleep 1
    /usr/bin/kill -9 -f "$nc_pid" > /dev/null 2>&1

    wait "$nc_pid" 2> /dev/null

    netstat /an | grep ":$aws_listen_port" | grep ESTABLISHED > /dev/null 2>&1
    basic_test "${FUNCNAME[0]}" 1 test $? -ne 0

    cleanup_test
}

test_multiple_disconnect()
{
    init_test

    nc localhost "$aws_listen_port" 0<&- &
    nc_pid1=$!
    nc localhost "$aws_listen_port" 0<&- &
    nc_pid2=$!

    sleep 1

    /usr/bin/kill -9 "$nc_pid1" "$nc_pid2" > /dev/null 2>&1
    wait "$nc_pid1" "$nc_pid2" &> /dev/null

    netstat /an | grep ":$aws_listen_port" | grep ESTABLISHED > /dev/null 2>&1
    basic_test "${FUNCNAME[0]}" 1 test $? -ne 0

    cleanup_test
}

test_connect_disconnect_connect()
{
    init_test

    nc localhost "$aws_listen_port" 0<&- &
    nc_pid=$!
    /usr/bin/kill -9 "$nc_pid" > /dev/null 2>&1
    wait "$nc_pid" 2> /dev/null

    nc localhost "$aws_listen_port" 0<&- &
    nc_pid=$!

    sleep 2
    netstat /an | grep ":$aws_listen_port" | grep ESTABLISHED > /dev/null 2>&1
    basic_test "${FUNCNAME[0]}" 1 test $? -eq 0

    /usr/bin/kill -9 "$nc_pid" > /dev/null 2>&1
    wait "$nc_pid" 2> /dev/null

    cleanup_test
}

test_multiple_connect_disconnect_connect()
{
    init_test

    nc localhost "$aws_listen_port" 0<&- &
    nc_pid1=$!
    /usr/bin/kill -9 "$nc_pid1" > /dev/null 2>&1
    wait "$nc_pid1" &> /dev/null

    nc localhost "$aws_listen_port" 0<&- &
    nc_pid1=$!
    nc localhost "$aws_listen_port" 0<&- &
    nc_pid2=$!
    /usr/bin/kill -9 "$nc_pid2" > /dev/null 2>&1
    wait "$nc_pid2" &> /dev/null

    nc localhost "$aws_listen_port" 0<&- &
    nc_pid2=$!

    sleep 2
    n_conns=$(netstat /an | grep ":$aws_listen_port" | grep ESTABLISHED | wc -l 2> /dev/null)
    basic_test "${FUNCNAME[0]}" 1 test "$n_conns" -eq 4

    /usr/bin/kill -9 "$nc_pid1" "$nc_pid2" > /dev/null 2>&1
    wait "$nc_pid1" "$nc_pid2" &> /dev/null

    cleanup_test
}

test_unordered_connect_disconnect_connect()
{
    init_test

    nc localhost "$aws_listen_port" 0<&- &
    nc_pid1=$!
    nc localhost "$aws_listen_port" 0<&- &
    nc_pid2=$!
    /usr/bin/kill -9 "$nc_pid1" "$nc_pid2" > /dev/null 2>&1
    wait "$nc_pid1" "$nc_pid2" 2> /dev/null

    nc localhost "$aws_listen_port" 0<&- &
    nc_pid1=$!
    nc localhost "$aws_listen_port" 0<&- &
    nc_pid2=$!

	sleep 2
    n_conns=$(netstat /an | grep ":$aws_listen_port" | grep ESTABLISHED | wc -l 2> /dev/null)
    basic_test "${FUNCNAME[0]}" 1 test "$n_conns" -eq 4

    /usr/bin/kill -9 "$nc_pid1" "$nc_pid2" > /dev/null 2>&1
    wait "$nc_pid1" "$nc_pid2" 2> /dev/null

    cleanup_test
}

test_replies_http_request()
{
    init_test

    echo -ne "GET /$(basename $static_folder)/small00.dat HTTP/1.0\r\n\r\n" | nc localhost $aws_listen_port > small00.dat 2> /dev/null
    sleep 1
    head -1 small00.dat | grep '^HTTP/' > /dev/null 2>&1
    basic_test "${FUNCNAME[0]}" 3 test $? -eq 0

    rm small00.dat
    cleanup_test
}

test_replies_http_request_1()
{
    init_test

    echo -ne "GET /$(basename $static_folder)/small00.dat HTTP/1.1\r\n\r\n" | nc localhost $aws_listen_port > small00.dat 2> /dev/null
    sleep 1
    head -1 small00.dat | grep '^HTTP/' > /dev/null 2>&1
    basic_test "${FUNCNAME[0]}" 1 test $? -eq 0

    rm small00.dat
    cleanup_test
}

test_uses_transmitfile()
{
    init_test

    nm -u "$obj_name" | grep TransmitFile > /dev/null 2>&1
    basic_test "${FUNCNAME[0]}" 2 test $? -eq 0

    cleanup_test
}

test_get_small_file_wget()
{
    init_test

    wget "http://localhost:8888/$(basename $static_folder)/small00.dat" -o wget.log -O small00.dat
    code1=$?
    [ -s small00.dat ]
    code2=$?
    basic_test "${FUNCNAME[0]}" 2 test $code1 -eq 0 -a $code2 -eq 0

    rm small00.dat
    cleanup_test
}

test_get_small_file_wget_cmp()
{
    init_test

    wget "http://localhost:8888/$(basename $static_folder)/small00.dat" -o wget.log -O small00.dat
    cmp small00.dat $static_folder/small00.dat > /dev/null 2>&1
    basic_test "${FUNCNAME[0]}" 5 test $? -eq 0

    rm small00.dat
    cleanup_test
}

test_get_large_file_wget()
{
    init_test

    wget "http://localhost:8888/$(basename $static_folder)/large00.dat" -o wget.log -O large00.dat
    code1=$?
    [ -s large00.dat ]
    code2=$?
    basic_test "${FUNCNAME[0]}" 2 test $code1 -eq 0 -a $code2 -eq 0

    rm large00.dat
    cleanup_test
}

test_get_large_file_wget_cmp()
{
    init_test

    wget "http://localhost:8888/$(basename $static_folder)/large00.dat" -o wget.log -O large00.dat
    cmp large00.dat $static_folder/large00.dat > /dev/null 2>&1
    basic_test "${FUNCNAME[0]}" 5 test $? -eq 0

    rm large00.dat
    cleanup_test
}

test_get_bad_file_404()
{
    init_test

    echo -ne "GET /$(basename $static_folder)/abcdef.dat HTTP/1.0\r\n\r\n" | nc localhost $aws_listen_port > abcdef.dat 2> /dev/null
    head -1 abcdef.dat | grep '^HTTP/' | grep '404' > /dev/null 2>&1
    basic_test "${FUNCNAME[0]}" 2 test $? -eq 0

    rm abcdef.dat
    cleanup_test
}

# Use non-static and non-dynamic file path.
test_bad_path_404()
{
    init_test

    echo -ne "GET /xyzt/abcdef.dat HTTP/1.0\r\n\r\n" | nc localhost $aws_listen_port > abcdef.dat 2> /dev/null
    head -1 abcdef.dat | grep '^HTTP/' | grep '404' > /dev/null 2>&1
    basic_test "${FUNCNAME[0]}" 2 test $? -eq 0

    rm abcdef.dat
    cleanup_test
}

test_get_one_file_then_another()
{
    init_test

    wget "http://localhost:8888/$(basename $static_folder)/large00.dat" -o /dev/null -O large00.dat
    wget "http://localhost:8888/$(basename $static_folder)/large01.dat" -o /dev/null -O large01.dat
    cmp large00.dat $static_folder/large00.dat > /dev/null 2>&1
    code1=$?
    cmp large01.dat $static_folder/large01.dat > /dev/null 2>&1
    code2=$?
    basic_test "${FUNCNAME[0]}" 3 test $code1 -eq 0 -a $code2 -eq 0

    rm large00.dat large01.dat
    cleanup_test
}

test_get_two_simultaneous_files()
{
    init_test

    wget "http://localhost:8888/$(basename $static_folder)/large00.dat" -o /dev/null -O large00.dat &
    pid1=$!
    wget "http://localhost:8888/$(basename $static_folder)/large01.dat" -o /dev/null -O large01.dat &
    pid2=$!
    wait $pid1
    wait $pid2
    cmp large00.dat $static_folder/large00.dat > /dev/null 2>&1
    code1=$?
    cmp large01.dat $static_folder/large01.dat > /dev/null 2>&1
    code2=$?
    basic_test "${FUNCNAME[0]}" 4 test $code1 -eq 0 -a $code2 -eq 0

    cleanup_test
}

test_get_multiple_simultaneous_files()
{
    init_test

    ret_val=0
    declare -a pids
    for i in $(seq 0 19); do
        filename=large$(printf "%02g" $i).dat
        wget "http://localhost:8888/$(basename $static_folder)/$filename" -o /dev/null -O $filename &
        pids[$i]=$!
    done
    for i in $(seq 0 19); do
        wait ${pids[$i]}
    done
    for i in $(seq 0 19); do
        filename=large$(printf "%02g" $i).dat
        if ! cmp $static_folder/$filename $filename > /dev/null 2>&1; then
            ret_val=1
            break
        fi
    done

    basic_test "${FUNCNAME[0]}" 5 test $ret_val -eq 0

    for i in $(seq 0 19); do
        filename=large$(printf "%02g" $i).dat
        rm $filename
    done
    cleanup_test
}

test_get_small_dyn_file_wget()
{
    init_test

    wget "http://localhost:8888/$(basename $dynamic_folder)/small00.dat" -o wget.log -O small00.dat
    code1=$?
    [ -s small00.dat ]
    code2=$?
    basic_test "${FUNCNAME[0]}" 3 test $code1 -eq 0 -a $code2 -eq 0

    rm small00.dat
    cleanup_test
}

test_get_small_dyn_file_wget_cmp()
{
    init_test

    wget "http://localhost:8888/$(basename $dynamic_folder)/small00.dat" -o wget.log -O small00.dat
    cmp small00.dat $dynamic_folder/small00.dat > /dev/null 2>&1
    basic_test "${FUNCNAME[0]}" 5 test $? -eq 0

    rm small00.dat
    cleanup_test
}

test_get_large_dyn_file_wget()
{
    init_test

    wget "http://localhost:8888/$(basename $dynamic_folder)/large00.dat" -o wget.log -O large00.dat
    code1=$?
    [ -s large00.dat ]
    code2=$?
    basic_test "${FUNCNAME[0]}" 3 test $code1 -eq 0 -a $code2 -eq 0

    rm large00.dat
    cleanup_test
}

test_get_large_dyn_file_wget_cmp()
{
    init_test

    wget "http://localhost:8888/$(basename $dynamic_folder)/large00.dat" -o wget.log -O large00.dat
    cmp large00.dat $dynamic_folder/large00.dat > /dev/null 2>&1
    basic_test "${FUNCNAME[0]}" 5 test $? -eq 0

    rm large00.dat
    cleanup_test
}

test_get_bad_dyn_file_404()
{
    init_test

    echo -ne "GET /$(basename $dynamic_folder)/abcdef.dat HTTP/1.0\r\n\r\n" | nc localhost $aws_listen_port > abcdef.dat 2> /dev/null
    head -1 abcdef.dat | grep '^HTTP/' | grep '404' > /dev/null 2>&1
    basic_test "${FUNCNAME[0]}" 2 test $? -eq 0

    rm abcdef.dat
    cleanup_test
}

test_get_one_dyn_file_then_another()
{
    init_test

    wget "http://localhost:8888/$(basename $dynamic_folder)/large00.dat" -o /dev/null -O large00.dat
    wget "http://localhost:8888/$(basename $dynamic_folder)/large01.dat" -o /dev/null -O large01.dat
    cmp large00.dat $dynamic_folder/large00.dat > /dev/null 2>&1
    code1=$?
    cmp large01.dat $dynamic_folder/large01.dat > /dev/null 2>&1
    code2=$?
    basic_test "${FUNCNAME[0]}" 4 test $code1 -eq 0 -a $code2 -eq 0

    rm large00.dat large01.dat
    cleanup_test
}

test_get_two_simultaneous_dyn_files()
{
    init_test

    wget "http://localhost:8888/$(basename $dynamic_folder)/large00.dat" -o /dev/null -O large00.dat &
    pid1=$!
    wget "http://localhost:8888/$(basename $dynamic_folder)/large01.dat" -o /dev/null -O large01.dat &
    pid2=$!
    wait $pid1
    wait $pid2
    cmp large00.dat $dynamic_folder/large00.dat > /dev/null 2>&1
    code1=$?
    cmp large01.dat $dynamic_folder/large01.dat > /dev/null 2>&1
    code2=$?
    basic_test "${FUNCNAME[0]}" 5 test $code1 -eq 0 -a $code2 -eq 0

    cleanup_test
}

test_get_multiple_simultaneous_dyn_files()
{
    init_test

    ret_val=0
    declare -a pids
    for i in $(seq 0 19); do
        filename=large$(printf "%02g" $i).dat
        wget "http://localhost:8888/$(basename $dynamic_folder)/$filename" -o /dev/null -O $filename &
        pids[$i]=$!
    done
    for i in $(seq 0 19); do
        wait ${pids[$i]}
    done
    for i in $(seq 0 19); do
        filename=large$(printf "%02g" $i).dat
        if ! cmp $dynamic_folder/$filename $filename > /dev/null 2>&1; then
            ret_val=1
            break
        fi
    done

    basic_test "${FUNCNAME[0]}" 6 test $ret_val -eq 0

    for i in $(seq 0 19); do
        filename=large$(printf "%02g" $i).dat
        rm $filename
    done
    cleanup_test
}

test_get_two_simultaneous_stat_dyn_files()
{
    init_test

    wget "http://localhost:8888/$(basename $static_folder)/large00.dat" -o /dev/null -O large00.dat &
    pid1=$!
    wget "http://localhost:8888/$(basename $dynamic_folder)/large01.dat" -o /dev/null -O large01.dat &
    pid2=$!
    wait $pid1
    wait $pid2
    cmp large00.dat $static_folder/large00.dat > /dev/null 2>&1
    code1=$?
    cmp large01.dat $dynamic_folder/large01.dat > /dev/null 2>&1
    code2=$?
    basic_test "${FUNCNAME[0]}" 4 test $code1 -eq 0 -a $code2 -eq 0

    cleanup_test
}

test_get_multiple_simultaneous_stat_dyn_files()
{
    init_test

    ret_val=0
    declare -a pids
    for i in $(seq 0 9); do
        filename=large$(printf "%02g" $i).dat
        wget "http://localhost:8888/$(basename $static_folder)/$filename" -o /dev/null -O $filename &
        pids[$i]=$!
    done
    for i in $(seq 9 19); do
        filename=large$(printf "%02g" $i).dat
        wget "http://localhost:8888/$(basename $dynamic_folder)/$filename" -o /dev/null -O $filename &
        pids[$i]=$!
    done
    for i in $(seq 0 19); do
        wait ${pids[$i]}
    done
    for i in $(seq 0 9); do
        filename=large$(printf "%02g" $i).dat
        if ! cmp $static_folder/$filename $filename > /dev/null 2>&1; then
            ret_val=1
            break
        fi
    done
    for i in $(seq 9 19); do
        filename=large$(printf "%02g" $i).dat
        if ! cmp $dynamic_folder/$filename $filename > /dev/null 2>&1; then
            ret_val=1
            break
        fi
    done

    basic_test "${FUNCNAME[0]}" 5 test $ret_val -eq 0

    for i in $(seq 0 19); do
        filename=large$(printf "%02g" $i).dat
        rm $filename
    done
    cleanup_test
}

test_fun_array=(                                \
    test_executable_exists                      \
    test_executable_runs                        \
    test_is_listening_on_port                   \
    test_accepts_connections                    \
    test_accepts_multiple_connections           \
    test_uses_iocp                              \
    test_disconnect                             \
    test_multiple_disconnect                    \
    test_connect_disconnect_connect             \
    test_multiple_connect_disconnect_connect    \
    test_unordered_connect_disconnect_connect   \
    test_replies_http_request                   \
    test_replies_http_request_1                 \
    test_uses_transmitfile                      \
    test_get_small_file_wget                    \
    test_get_small_file_wget_cmp                \
    test_get_large_file_wget                    \
    test_get_large_file_wget_cmp                \
    test_get_bad_file_404                       \
    test_bad_path_404                           \
    test_get_one_file_then_another              \
    test_get_two_simultaneous_files             \
    test_get_multiple_simultaneous_files        \
    test_get_small_dyn_file_wget                \
    test_get_small_dyn_file_wget_cmp            \
    test_get_large_dyn_file_wget                \
    test_get_large_dyn_file_wget_cmp            \
    test_get_bad_dyn_file_404                   \
    test_get_one_dyn_file_then_another          \
    test_get_two_simultaneous_dyn_files         \
    test_get_multiple_simultaneous_dyn_files    \
    test_get_two_simultaneous_stat_dyn_files    \
    test_get_multiple_simultaneous_stat_dyn_files   \
)

el_mondo_init()
{
    mkdir $static_folder > /dev/null 2>&1
    mkdir $dynamic_folder > /dev/null 2>&1
    for i in $(seq -f "%02g" 0 19); do
        dd if=/dev/urandom of=$static_folder/small$i.dat bs=2K count=1 > /dev/null 2>&1
        dd if=/dev/urandom of=$static_folder/large$i.dat bs=2M count=1 > /dev/null 2>&1
        cp $static_folder/small$i.dat $dynamic_folder/small$i.dat
        cp $static_folder/large$i.dat $dynamic_folder/large$i.dat
    done
}

el_mondo_cleanup()
{
    rm -fr $static_folder > /dev/null 2>&1
    rm -fr $dynamic_folder > /dev/null 2>&1
}

if test $# -ne 1; then
    echo "Usage: $0 test_number | init | cleanup" 1>&2
    exit 1
fi

test_index=$1

if test $test_index == "init"; then
    el_mondo_init
    exit 0
fi

if test $test_index == "cleanup"; then
    el_mondo_cleanup
    exit 0
fi

arr_index=$(($test_index - 1))
last_test=${#test_fun_array[@]}

if test "$arr_index" -gt "$last_test" -o "$arr_index" -lt 0; then
    echo "Error: Test index is out range (0 < test_index <= $last_test)." 1>&2
    exit 1
fi

# run proper function
${test_fun_array[$(($arr_index))]}

exit 0

