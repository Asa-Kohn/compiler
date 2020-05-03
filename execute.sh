#!/bin/bash

gcc -g -o ${1%.go} ${1%.go}.c
${1%.go}
