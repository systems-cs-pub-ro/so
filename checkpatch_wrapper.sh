#!/bin/bash

CHECKPATCH=$(which checkpatch.pl 2> /dev/null)

if [ -z "$CHECKPATCH" ]; then
	echo "checkpatch.pl (https://raw.githubusercontent.com/torvalds/linux/master/scripts/checkpatch.pl) not in \$PATH"
	exit 1
fi

COMMON_IGNORE_FLAGS=SPLIT_STRING,SSCANF_TO_KSTRTO,NEW_TYPEDEFS,VOLATILE,INLINE,USE_FUNC,AVOID_EXTERNS,CONST_STRUCT
LIN_IGNORE_FLAGS=$COMMON_IGNORE_FLAGS
WIN_IGNORE_FLAGS=$COMMON_IGNORE_FLAGS,DOS_LINE_ENDINGS

if [ $(uname -s) = "Linux" ]; then
	IGNORE_FLAGS=$LIN_IGNORE_FLAGS
else
	IGNORE_FLAGS=$WIN_IGNORE_FLAGS
fi

CHECKPATCH_ARGS="--no-tree --no-summary --terse --show-types
	--ignore $IGNORE_FLAGS"

if [ $# -eq 1 ]; then
	$CHECKPATCH $CHECKPATCH_ARGS -f $1
else
	find . -type f -iregex '.*\.\(c\|h\|cpp\|hpp\|cc\|hh\|cxx\|hxx\)' | \
		xargs $CHECKPATCH $CHECKPATCH_ARGS -f
fi

