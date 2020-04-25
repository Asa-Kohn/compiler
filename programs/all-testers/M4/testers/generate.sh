#!/bin/bash

# in the case that you want to copy template.go to every leaf subdirectory use:

# AWKCOMMAND='$0 !~ last "/" {print last} {last=$0} END {print last}'
#
# for folder in $(find . -type d | awk $AWKCOMMAND)
# do
#     cp ./template.go "$folder"
# done
