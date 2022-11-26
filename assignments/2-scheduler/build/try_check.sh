#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

preprocess() {
    printf "Creating the libscheduler.so ................ "
    make

    if [[ $? != 0 ]]; then
        printf "${RED}FAILED${NC}\n"
        exit 1
    fi

    printf "${GREEN}SUCCESS${NC}\n"

    printf "Copying the library into checker-lin ........ "
    cp ../libs/libscheduler.so ../checker-lin

    if [[ $? != 0 ]]; then
        printf "${RED}FAILED${NC}\n"
        exit 1
    fi

    printf "${GREEN}SUCCESS${NC}\n"
}

main() {
    if [[ $? != 0 ]]; then
        printf "${RED}Could not run tests${NC}\n"
        exit 1
    fi

    printf "${GREEN}Running tests${NC}\n\n"
    cd ../checker-lin && make -f Makefile.checker && make -f Makefile.checker clean && cd - > /dev/null
}

preprocess
main