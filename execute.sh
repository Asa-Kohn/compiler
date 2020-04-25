#!/bin/bash

./src/winnipeg codegen < $1 | python3
exit $?
