#!/bin/bash

#
# Asynchronous Web Server - test suite
#
# 2011-2017, Operating Systems
#

# ----------------- General declarations and util functions ------------------ #

# Use one try and 3 seconds timeout for wget
alias wget='wget -t 1 -T 3'

exec_name="./aws.exe"
obj_name="./aws.obj"
exec_pid=-1
aws_listen_port=8888
static_folder="./static/"
dynamic_folder="./dynamic/"

LOG_FILE=test.log
WGET_LOG=wget.log

# Enable/disable cleanup (yes/no)
DO_CLEANUP=yes

max_points=90

# Enable/disable exiting when program fails (1/0)
EXIT_IF_FAIL=0

# Enable/disable debug (1/0)
DEBUG_=0

DEBUG()
{
	if test "x$DEBUG_" = "x1"; then
		$@ 1>&2
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
	points=$1
	printf "failed  [ 0/%02d]\n" $max_points
	if test "x$EXIT_IF_FAIL" = "x1"; then
		exit 1
	fi
}

test_do_pass()
{
	points=$1
	printf "passed  [%02d/%02d]\n" $points $max_points
}

basic_test()
{
	printf "%02d) %s" $test_index "$description"

	for ((i = 0; i < 56 - ${#description}; ++i)); do
		printf "."
	done

	$@ > /dev/null 2>&1
	if test $? -eq 0; then
		test_do_pass $points
	else
		test_do_fail $points
	fi
}

# ---------------------------------------------------------------------------- #

# ----------------- Init and cleanup tests ----------------------------------- #

# Initializes a test
init_test()
{
	$exec_name &>> $LOG_FILE &
	if test $? -eq 0; then
		exec_pid=$!
	fi

	sleep 1
}

# Cleanups a test
cleanup_test()
{
	/usr/bin/kill -9 $exec_pid &>> $LOG_FILE

	if [ "$DO_CLEANUP" = "yes" ]; then
		rm -rf $WGET_LOG &>> $LOG_FILE
	fi

	wait $exec_pid > /dev/null 2>&1
}

# Initializes the whole testing environment
# Should be the first test called
init_world()
{
	[ -d $static_folder ] && rm -rf $static_folder &>> $LOG_FILE
	[ -d $dynamic_folder ] && rm -rf $dynamic_folder &>> $LOG_FILE
	mkdir $static_folder &>> $LOG_FILE
	mkdir $dynamic_folder &>> $LOG_FILE

	print_header "Testing - Asynchronous Web Server"

	for i in $(seq -f "%02g" 0 19); do
		dd if=/dev/urandom of=$static_folder/small$i.dat \
			bs=2K count=1 &>> $LOG_FILE
		dd if=/dev/urandom of=$static_folder/large$i.dat \
			bs=2M count=1 &>> $LOG_FILE
		cp $static_folder/small$i.dat $dynamic_folder/small$i.dat
		cp $static_folder/large$i.dat $dynamic_folder/large$i.dat
	done
}

# Cleanups the whole testing environment
# Should be the last test called
cleanup_world()
{
	if [ "$DO_CLEANUP" = "yes" ]; then
		rm -fr $static_folder &>> $LOG_FILE
		rm -fr $dynamic_folder &>> $LOG_FILE
		rm -fr $LOG_FILE
	fi
}

# ---------------------------------------------------------------------------- #

# ----------------- Test Suite ----------------------------------------------- #

test_executable_exists()
{
	init_test

	basic_test test -e $exec_name

	cleanup_test
}

test_executable_runs()
{
	init_test

	ps | grep 'aws$' > /dev/null 2>&1
	basic_test test $? -eq 0

	cleanup_test
}

# Use netstat to see whether server is listening on proper port
test_is_listening_on_port()
{
	init_test

	netstat /an | grep "0.0.0.0:$aws_listen_port" | grep LISTENING | \
		awk -F '[ \t]+' '{print $3;}' | grep $aws_listen_port > /dev/null 2>&1
	basic_test test $? -eq 0

	cleanup_test
}

# Use netcat for connecting to server
test_accepts_connections()
{
	init_test

	nc localhost $aws_listen_port 0<&- &
	nc_pid=$!
	sleep 2

	netstat /an | grep ":$aws_listen_port" | grep TCP | \
		grep ESTABLISHED > /dev/null 2>&1
	basic_test test $? -eq 0

	/usr/bin/kill -9 $nc_pid > /dev/null 2>&1
	wait $nc_pid > /dev/null 2>&1

	cleanup_test
}

test_accepts_multiple_connections()
{
	init_test

	nc localhost $aws_listen_port 0<&- &
	nc_pid1=$!
	nc localhost $aws_listen_port 0<&- &
	nc_pid2=$!
	sleep 2

	n_conns=$(netstat /an | grep ":$aws_listen_port" | grep TCP | \
		grep ESTABLISHED | wc -l 2> /dev/null)
	DEBUG echo "n_conns: $n_conns"
	basic_test test $n_conns -eq 4

	/usr/bin/kill -9 $nc_pid1 $nc_pid2 > /dev/null 2>&1
	wait $nc_pid1 $nc_pid2 > /dev/null 2>&1

	cleanup_test
}

# Use nm to list undefined symbols in object file
test_uses_iocp()
{
	init_test

	nm -u $obj_name | grep CreateIoCompletionPort > /dev/null 2>&1
	basic_test test $? -eq 0

	cleanup_test
}

test_disconnect()
{
	init_test

	nc localhost $aws_listen_port 0<&- &
	nc_pid=$!
	sleep 2

	n_conns=$(netstat /an | grep ":$aws_listen_port" | grep TCP | \
		grep ESTABLISHED | wc -l 2> /dev/null)

	/usr/bin/kill -9 $nc_pid > /dev/null 2>&1
	wait $nc_pid > /dev/null 2>&1

	netstat /an | grep ":$aws_listen_port" | grep TCP | \
		grep ESTABLISHED > /dev/null 2>&1
	basic_test test $n_conns -eq 2 -a $? -ne 0

	cleanup_test
}

test_multiple_disconnect()
{
	init_test

	nc localhost $aws_listen_port 0<&- &
	nc_pid1=$!
	nc localhost $aws_listen_port 0<&- &
	nc_pid2=$!
	sleep 2

	n_conns=$(netstat /an | grep ":$aws_listen_port" | grep TCP | \
		grep ESTABLISHED | wc -l 2> /dev/null)

	/usr/bin/kill -9 $nc_pid1 $nc_pid2 > /dev/null 2>&1
	wait $nc_pid1 $nc_pid2 > /dev/null 2>&1

	netstat /an | grep ":$aws_listen_port" | grep TCP | \
		grep ESTABLISHED > /dev/null 2>&1
	basic_test test $n_conns -eq 4 -a $? -ne 0

	cleanup_test
}

test_connect_disconnect_connect()
{
	init_test

	nc localhost $aws_listen_port 0<&- &
	nc_pid=$!
	/usr/bin/kill -9 $nc_pid > /dev/null 2>&1
	wait $nc_pid > /dev/null 2>&1
	nc localhost $aws_listen_port 0<&- &
	nc_pid=$!
	sleep 2

	netstat /an | grep ":$aws_listen_port" | grep TCP | \
		grep ESTABLISHED > /dev/null 2>&1
	basic_test test $? -eq 0

	/usr/bin/kill -9 $nc_pid > /dev/null 2>&1
	wait $nc_pid > /dev/null 2>&1

	cleanup_test
}

test_multiple_connect_disconnect_connect()
{
	init_test

	nc localhost $aws_listen_port 0<&- &
	nc_pid1=$!
	/usr/bin/kill -9 $nc_pid1 > /dev/null 2>&1
	wait $nc_pid1 > /dev/null 2>&1
	nc localhost $aws_listen_port 0<&- &
	nc_pid1=$!
	nc localhost $aws_listen_port 0<&- &
	nc_pid2=$!
	/usr/bin/kill -9 $nc_pid2 > /dev/null 2>&1
	wait $nc_pid2 > /dev/null 2>&1
	nc localhost $aws_listen_port 0<&- &
	nc_pid2=$!
	sleep 2

	n_conns=$(netstat /an | grep ":$aws_listen_port" | grep TCP | \
		grep ESTABLISHED | wc -l 2> /dev/null)
	DEBUG echo "n_conns: $n_conns"
	basic_test test $n_conns -eq 4

	/usr/bin/kill -9 $nc_pid1 $nc_pid2 > /dev/null 2>&1
	wait $nc_pid1 $nc_pid2 > /dev/null 2>&1

	cleanup_test
}

test_unordered_connect_disconnect_connect()
{
	init_test

	nc localhost $aws_listen_port 0<&- &
	nc_pid1=$!
	nc localhost $aws_listen_port 0<&- &
	nc_pid2=$!
	/usr/bin/kill -9 $nc_pid1 $nc_pid2 > /dev/null 2>&1
	wait $nc_pid1 $nc_pid2 > /dev/null 2>&1
	nc localhost $aws_listen_port 0<&- &
	nc_pid1=$!
	nc localhost $aws_listen_port 0<&- &
	nc_pid2=$!
	sleep 2

	n_conns=$(netstat /an | grep ":$aws_listen_port" | grep TCP | \
		grep ESTABLISHED | wc -l 2> /dev/null)
	DEBUG echo "n_conns: $n_conns"
	basic_test test $n_conns -eq 4

	/usr/bin/kill -9 $nc_pid1 $nc_pid2 > /dev/null 2>&1
	wait $nc_pid1 $nc_pid2 > /dev/null 2>&1

	cleanup_test
}

test_replies_http_request()
{
	init_test

	echo -ne "GET /$(basename $static_folder)/small00.dat HTTP/1.0\r\n\r\n" | \
		nc localhost $aws_listen_port > small00.dat 2> /dev/null

	head -1 small00.dat | grep '^HTTP/' > /dev/null 2>&1
	basic_test test $? -eq 0

	rm small00.dat
	cleanup_test
}

test_replies_http_request_1()
{
	init_test

	echo -ne "GET /$(basename $static_folder)/small00.dat HTTP/1.1\r\n\r\n" | \
		nc localhost $aws_listen_port > small00.dat 2> /dev/null

	head -1 small00.dat | grep '^HTTP/' > /dev/null 2>&1
	basic_test test $? -eq 0

	rm small00.dat
	cleanup_test
}

test_uses_transmitfile()
{
	init_test

	nm -u $obj_name | grep TransmitFile > /dev/null 2>&1
	basic_test test $? -eq 0

	cleanup_test
}

test_get_small_file_wget()
{
	init_test

	wget "http://localhost:8888/$(basename $static_folder)/small00.dat" \
		-o $WGET_LOG -O small00.dat
	code1=$?

	[ -s small00.dat ]
	code2=$?
	basic_test test $code1 -eq 0 -a $code2 -eq 0

	rm small00.dat
	cleanup_test
}

test_get_small_file_wget_cmp()
{
	init_test

	wget "http://localhost:8888/$(basename $static_folder)/small00.dat" \
		-o $WGET_LOG -O small00.dat

	cmp small00.dat $static_folder/small00.dat > /dev/null 2>&1
	basic_test test $? -eq 0

	rm small00.dat
	cleanup_test
}

test_get_large_file_wget()
{
	init_test

	wget "http://localhost:8888/$(basename $static_folder)/large00.dat" \
		-o $WGET_LOG -O large00.dat
	code1=$?

	[ -s large00.dat ]
	code2=$?
	basic_test test $code1 -eq 0 -a $code2 -eq 0

	rm large00.dat
	cleanup_test
}

test_get_large_file_wget_cmp()
{
	init_test

	wget "http://localhost:8888/$(basename $static_folder)/large00.dat" \
		-o $WGET_LOG -O large00.dat

	cmp large00.dat $static_folder/large00.dat > /dev/null 2>&1
	basic_test test $? -eq 0

	rm large00.dat
	cleanup_test
}

test_get_bad_file_404()
{
	init_test

	echo -ne "GET /$(basename $static_folder)/abcdef.dat HTTP/1.0\r\n\r\n" | \
		nc localhost $aws_listen_port > abcdef.dat 2> /dev/null

	head -1 abcdef.dat | grep '^HTTP/' | grep '404' > /dev/null 2>&1
	basic_test test $? -eq 0

	rm abcdef.dat
	cleanup_test
}

# Use non-static and non-dynamic file path
test_bad_path_404()
{
	init_test

	echo -ne "GET /xyzt/abcdef.dat HTTP/1.0\r\n\r\n" | \
		nc localhost $aws_listen_port > abcdef.dat 2> /dev/null

	head -1 abcdef.dat | grep '^HTTP/' | grep '404' > /dev/null 2>&1
	basic_test test $? -eq 0

	rm abcdef.dat
	cleanup_test
}

test_get_one_file_then_another()
{
	init_test

	wget "http://localhost:8888/$(basename $static_folder)/large00.dat" \
		-o /dev/null -O large00.dat
	wget "http://localhost:8888/$(basename $static_folder)/large01.dat" \
		-o /dev/null -O large01.dat

	cmp large00.dat $static_folder/large00.dat > /dev/null 2>&1
	code1=$?
	cmp large01.dat $static_folder/large01.dat > /dev/null 2>&1
	code2=$?
	basic_test test $code1 -eq 0 -a $code2 -eq 0

	rm large00.dat large01.dat
	cleanup_test
}

test_get_two_simultaneous_files()
{
	init_test

	wget "http://localhost:8888/$(basename $static_folder)/large00.dat" \
		-o /dev/null -O large00.dat &
	pid1=$!
	wget "http://localhost:8888/$(basename $static_folder)/large01.dat" \
		-o /dev/null -O large01.dat &
	pid2=$!
	wait $pid1 $pid2 > /dev/null 2>&1

	cmp large00.dat $static_folder/large00.dat > /dev/null 2>&1
	code1=$?
	cmp large01.dat $static_folder/large01.dat > /dev/null 2>&1
	code2=$?
	basic_test test $code1 -eq 0 -a $code2 -eq 0

	rm large00.dat large01.dat
	cleanup_test
}

test_get_multiple_simultaneous_files()
{
	init_test

	ret_val=0
	declare -a pids

	for i in $(seq 0 19); do
		filename=large$(printf "%02g" $i).dat
		wget "http://localhost:8888/$(basename $static_folder)/$filename" \
			-o /dev/null -O $filename &
		pids[$i]=$!
	done
	for i in $(seq 0 19); do
		wait ${pids[$i]} > /dev/null 2>&1
	done

	for i in $(seq 0 19); do
		filename=large$(printf "%02g" $i).dat
		if ! cmp $static_folder/$filename $filename > /dev/null 2>&1; then
			ret_val=1
			break
		fi
	done
	basic_test test $ret_val -eq 0

	for i in $(seq 0 19); do
		filename=large$(printf "%02g" $i).dat
		rm $filename
	done
	cleanup_test
}

test_get_small_dyn_file_wget()
{
	init_test

	wget "http://localhost:8888/$(basename $dynamic_folder)/small00.dat" \
		-o $WGET_LOG -O small00.dat
	code1=$?

	[ -s small00.dat ]
	code2=$?
	basic_test test $code1 -eq 0 -a $code2 -eq 0

	rm small00.dat
	cleanup_test
}

test_get_small_dyn_file_wget_cmp()
{
	init_test

	wget "http://localhost:8888/$(basename $dynamic_folder)/small00.dat" \
		-o $WGET_LOG -O small00.dat

	cmp small00.dat $dynamic_folder/small00.dat > /dev/null 2>&1
	basic_test test $? -eq 0

	rm small00.dat
	cleanup_test
}

test_get_large_dyn_file_wget()
{
	init_test

	wget "http://localhost:8888/$(basename $dynamic_folder)/large00.dat" \
		-o $WGET_LOG -O large00.dat
	code1=$?

	[ -s large00.dat ]
	code2=$?
	basic_test test $code1 -eq 0 -a $code2 -eq 0

	rm large00.dat
	cleanup_test
}

test_get_large_dyn_file_wget_cmp()
{
	init_test

	wget "http://localhost:8888/$(basename $dynamic_folder)/large00.dat" \
		-o $WGET_LOG -O large00.dat

	cmp large00.dat $dynamic_folder/large00.dat > /dev/null 2>&1
	basic_test test $? -eq 0

	rm large00.dat
	cleanup_test
}

test_get_bad_dyn_file_404()
{
	init_test

	echo -ne "GET /$(basename $dynamic_folder)/abcdef.dat HTTP/1.0\r\n\r\n" | \
		nc localhost $aws_listen_port > abcdef.dat 2> /dev/null

	head -1 abcdef.dat | grep '^HTTP/' | grep '404' > /dev/null 2>&1
	basic_test test $? -eq 0

	rm abcdef.dat
	cleanup_test
}

test_get_one_dyn_file_then_another()
{
	init_test

	wget "http://localhost:8888/$(basename $dynamic_folder)/large00.dat" \
		-o /dev/null -O large00.dat
	wget "http://localhost:8888/$(basename $dynamic_folder)/large01.dat" \
		-o /dev/null -O large01.dat

	cmp large00.dat $dynamic_folder/large00.dat > /dev/null 2>&1
	code1=$?
	cmp large01.dat $dynamic_folder/large01.dat > /dev/null 2>&1
	code2=$?
	basic_test test $code1 -eq 0 -a $code2 -eq 0

	rm large00.dat large01.dat
	cleanup_test
}

test_get_two_simultaneous_dyn_files()
{
	init_test

	wget "http://localhost:8888/$(basename $dynamic_folder)/large00.dat" \
		-o /dev/null -O large00.dat &
	pid1=$!
	wget "http://localhost:8888/$(basename $dynamic_folder)/large01.dat" \
		-o /dev/null -O large01.dat &
	pid2=$!
	wait $pid1 $pid2 > /dev/null 2>&1

	cmp large00.dat $dynamic_folder/large00.dat > /dev/null 2>&1
	code1=$?
	cmp large01.dat $dynamic_folder/large01.dat > /dev/null 2>&1
	code2=$?
	basic_test test $code1 -eq 0 -a $code2 -eq 0

	rm large00.dat large01.dat
	cleanup_test
}

test_get_multiple_simultaneous_dyn_files()
{
	init_test

	ret_val=0
	declare -a pids

	for i in $(seq 0 19); do
		filename=large$(printf "%02g" $i).dat
		wget "http://localhost:8888/$(basename $dynamic_folder)/$filename" \
			-o /dev/null -O $filename &
		pids[$i]=$!
	done
	for i in $(seq 0 19); do
		wait ${pids[$i]} > /dev/null 2>&1
	done

	for i in $(seq 0 19); do
		filename=large$(printf "%02g" $i).dat
		if ! cmp $dynamic_folder/$filename $filename > /dev/null 2>&1; then
			ret_val=1
			break
		fi
	done
	basic_test test $ret_val -eq 0

	for i in $(seq 0 19); do
		filename=large$(printf "%02g" $i).dat
		rm $filename
	done
	cleanup_test
}

test_get_two_simultaneous_stat_dyn_files()
{
	init_test

	wget "http://localhost:8888/$(basename $static_folder)/large00.dat" \
		-o /dev/null -O large00.dat &
	pid1=$!
	wget "http://localhost:8888/$(basename $dynamic_folder)/large01.dat" \
		-o /dev/null -O large01.dat &
	pid2=$!
	wait $pid1 $pid2 > /dev/null 2>&1

	cmp large00.dat $static_folder/large00.dat > /dev/null 2>&1
	code1=$?
	cmp large01.dat $dynamic_folder/large01.dat > /dev/null 2>&1
	code2=$?
	basic_test test $code1 -eq 0 -a $code2 -eq 0

	rm large00.dat large01.dat
	cleanup_test
}

test_get_multiple_simultaneous_stat_dyn_files()
{
	init_test

	ret_val=0
	declare -a pids

	for i in $(seq 0 9); do
		filename=large$(printf "%02g" $i).dat
		wget "http://localhost:8888/$(basename $static_folder)/$filename" \
			-o /dev/null -O $filename &
		pids[$i]=$!
	done
	for i in $(seq 9 19); do
		filename=large$(printf "%02g" $i).dat
		wget "http://localhost:8888/$(basename $dynamic_folder)/$filename" \
			-o /dev/null -O $filename &
		pids[$i]=$!
	done
	for i in $(seq 0 19); do
		wait ${pids[$i]} > /dev/null 2>&1
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
	basic_test test $ret_val -eq 0

	for i in $(seq 0 19); do
		filename=large$(printf "%02g" $i).dat
		rm $filename
	done
	cleanup_test
}

# Specifies the tests, commands and points
test_fun_array=( \
	test_executable_exists "Test executable exists" 1
	test_executable_runs "Test executable runs" 1
	test_is_listening_on_port "Test listening on port" 2
	test_accepts_connections "Test accepts connections" 1
	test_accepts_multiple_connections "Test accepts multiple connections" 1
	test_uses_iocp "Test iocp usage" 1
	test_disconnect "Test disconnect" 1
	test_multiple_disconnect "Test multiple disconnect" 1
	test_connect_disconnect_connect "Test connect disconnect connect" 1
	test_multiple_connect_disconnect_connect "Test multiple connect disconnect connect" 1
	test_unordered_connect_disconnect_connect "Test unordered connect disconnect connect" 1
	test_replies_http_request "Test replies http request" 3
	test_replies_http_request_1 "Test second replies http request" 1
	test_uses_transmitfile "Test transmitfile usage" 2
	test_get_small_file_wget "Test small static file wget" 2
	test_get_small_file_wget_cmp "Test small static file wget cmp" 5
	test_get_large_file_wget "Test large static file wget" 2
	test_get_large_file_wget_cmp "Test large static file wget cmp" 5
	test_get_bad_file_404 "Test bad static file 404" 2
	test_bad_path_404 "Test bad path 404" 2
	test_get_one_file_then_another "Test get one static file then another" 3
	test_get_two_simultaneous_files "Test get two simultaneous static files" 4
	test_get_multiple_simultaneous_files "Test get multiple simultaneous static files" 5
	test_get_small_dyn_file_wget "Test small dynamic file wget" 3
	test_get_small_dyn_file_wget_cmp "Test small dynamic file wget cmp" 5
	test_get_large_dyn_file_wget "Test large dynamic file wget" 3
	test_get_large_dyn_file_wget_cmp "Test large dynamic file wget cmp" 5
	test_get_bad_dyn_file_404 "Test bad dynamic file 404" 2
	test_get_one_dyn_file_then_another "Test get one dynamic file then another" 4
	test_get_two_simultaneous_dyn_files "Test get two simultaneous dynamic files" 5
	test_get_multiple_simultaneous_dyn_files "Test get multiple simultaneous dynamic files" 6
	test_get_two_simultaneous_stat_dyn_files "Test get two simultaneous static and dynamic files" 4
	test_get_multiple_simultaneous_stat_dyn_files "Test get multiple simultaneous static and dynamic files" 5
)

# ---------------------------------------------------------------------------- #

# ----------------- Run test ------------------------------------------------- #

if test $# -ne 1; then
	echo "Usage: $0 <test_number> | init | cleanup" 1>&2
	exit 1
fi

test_index=$1

if test $test_index == "init"; then
	init_world
	exit 0
fi

if test $test_index == "cleanup"; then
	cleanup_world
	exit 0
fi

arr_index=$((($test_index - 1) * 3))
last_test=$((${#test_fun_array[@]} / 3))
description=${test_fun_array[$(($arr_index + 1))]}
points=${test_fun_array[$(($arr_index + 2))]}

if test $test_index -lt 1 -o $test_index -gt $last_test; then
	echo "Error: Test index is out range (1 <= test_index <= $last_test)." 1>&2
	exit 1
fi

# Run proper function
${test_fun_array[$(($arr_index))]}

exit 0

