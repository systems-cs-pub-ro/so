#!/bin/bash

#
# Tema3 Test Suite
#
# 2015, Operating Systems
#

first_test=1
last_test=34
script=run_test
TEST_TIMEOUT=10
CHECKPATCH_URL="https://raw.githubusercontent.com/torvalds/linux/master/scripts/checkpatch.pl"
CHECKPATCH_IGNORE_FLAGS="
	SPLIT_STRING,SSCANF_TO_KSTRTO,NEW_TYPEDEFS,VOLATILE,INLINE,USE_FUNC,AVOID_EXTERNS,CONST_STRUCT,SYMBOLIC_PERMS,FUNCTION_ARGUMENTS"
CHECKPATCH_ARGS="
	--no-tree
	--no-summary
	--terse
	--ignore $CHECKPATCH_IGNORE_FLAGS
	--show-types"

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
		printf "passed  [05/95]\n" | tee check_source_result.txt
	else
		printf "failed  [00/95]\n" | tee check_source_result.txt
	fi
}

# Call init to set up testing environment
./_test/"$script" init

# Check the sources using checkpatch
check_source

for i in $(seq $first_test $last_test); do
	timeout $TEST_TIMEOUT ./_test/"$script" $i
done | tee results.txt

cat results.txt check_source_result.txt | grep '\[.*\]$' | awk -F '[] /[]+' '
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
./_test/"$script" cleanup
rm -f results.txt check_source_result.txt

