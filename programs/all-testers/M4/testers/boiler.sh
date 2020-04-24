#!/bin/bash

# outputs the appropriate string based on arguments

# $1 : number of variables
# $2 : variable type (omit, int, string, rune, float)
# $3 : include printing? (y/n)

for i in $(seq 1 $1)
do
    str="var"

    if [[ $i = "1" ]]; then
        strid="a"
        str="${str} ${strid}"
    fi
    if [[ $i = "2" ]]; then
        strid="b"
        str="${str} ${strid}"
    fi
    if [[ $i == "3" ]]; then
        strid="c"
        str="${str} ${strid}"
    fi
    if [[ $i == "4" ]]; then
        strid="d"
        str="${str} ${strid}"
    fi
    if [[ $i == "5" ]]; then
        strid="e"
        str="${str} ${strid}"
    fi
    if [[ $2 == "omit" ]]; then
        str="${str}"
    fi
    if [[ $2 != "omit" ]]; then
        str="${str} : ${2}"
    fi

    str="${str} ="

    if [[ $2 == "string" ]]; then
        str="${str} \"${i}\""
    fi
    if [[ $2 == "rune" ]]; then
        str="${str} '${i}'"
    fi
    if [[ $2 != "string" && $2 != "rune" ]]; then
        str="${str} ${i}"
    fi
    if [[ $3 == "y" ]]; then
        str="${str}\nprintln(${strid})"
    fi

    echo -e "${str}"
done
