#!/bin/bash

first_test=1
last_test=35
script=run_test.sh
timeout=30
log_file=test.log
CHECKPATCH_URL="https://raw.githubusercontent.com/torvalds/linux/master/scripts/checkpatch.pl"
CHECKPATCH_IGNORE_FLAGS="
	SPLIT_STRING,SSCANF_TO_KSTRTO,NEW_TYPEDEFS,VOLATILE,INLINE,USE_FUNC,AVOID_EXTERNS,CONST_STRUCT,MACRO_WITH_FLOW_CONTROL,SPDX_LICENSE_TAG"
CHECKPATCH_ARGS="
	--no-tree
	--no-summary
	--terse
	--ignore $CHECKPATCH_IGNORE_FLAGS
	--show-types"
CHECK_SKIP='http_parser.\(c\|h\)'

check_source()
{
	# SKIP some files from checking
	local SKIP
	[ -n "$CHECK_SKIP" ] && SKIP="grep -v $CHECK_SKIP" || SKIP="cat"

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
		'.*\.\(c\|h\|cpp\|hpp\|cc\|hh\|cxx\|hxx\)' | $SKIP)"
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
			'.*\.\(c\|h\|cpp\|hpp\|cc\|hh\|cxx\|hxx\)' | $SKIP)"
		if [ -z "$src_files" ]; then
			echo -e "No sources found in '$SRC_DIR'. " \
				"Skipping source check...\n"
			return
		fi
	fi
	# now we have sources in $SRC_DIR or .
	OUT=$(find "${SRC_DIR:-.}" -type f -iregex \
		'.*\.\(c\|h\|cpp\|hpp\|cc\|hh\|cxx\|hxx\)' | $SKIP | \
		xargs $check_patch $CHECKPATCH_ARGS -f 2>&1 | tail -n +3 | \
		sort -u -t":" -k4,4  | head -n 20)
	echo "$OUT"
	printf "00) Sources check......................................................" | tee check_source_result.txt
	if [ -z "$OUT" ]; then
		printf "passed  [00/95]\n" | tee check_source_result.txt
	else
		printf "failed  [00/95]\n" | tee check_source_result.txt
	fi
}

# Call init to set up testing environment
timeout $timeout bash ./_test/"$script" init

# Check the sources using checkpatch
check_source

for i in $(seq $first_test $last_test); do
	echo "=== Enter test $i ===" &>> $log_file
	timeout $timeout bash ./_test/"$script" $i 2>> $log_file
	exit_code=$?
	echo "=== Exit test $i ===" &>> $log_file
	if [ $exit_code -eq 124 ]; then
		printf "%02d) timeout............................................................failed  [00/95]\n" $i
	elif [ $exit_code -ne 0 ]; then
		printf "%02d) crash..............................................................failed  [00/95]\n" $i
	fi
done | tee results.txt

cat results.txt check_source_result.txt | grep '\[.*\]$' | awk -F '[] /[]+' '
BEGIN {
	sum = 0
}

{
	sum += $(NF-2);
}

END {
	printf "\n%77s  [%02d/95]\n", "Total:", sum;
}'

# Cleanup testing environment
timeout $timeout bash ./_test/"$script" cleanup
rm -f results.txt check_source_result.txt

