#!/bin/bash

for kw in $(cat keywords.txt)
do
    echo -e "//~this is not a reserved word in GoLite\n\npackage main\n\nvar ${kw} string = \"this is not a reserved word in GoLite\";\n\nfunc main() {\n\tprint(${kw});\n}" > "${kw}".go
done
