#!/bin/bash

for kw in $(cat keywords.txt)
do
    echo -e "// generated from template.go\n\npackage main\n\nvar ${kw} : string =\"this is not a reserved word in GoLite\";\n\nfunc main() {\n\tprintln(${kw});\n}" > "${kw}".go
done
