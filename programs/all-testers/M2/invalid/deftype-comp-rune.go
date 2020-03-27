// Error: (line 12) assigment lhs type is incompatible with rhs type [rune != r]
package main

type r rune;
// symbol table expected output
// defined types are distinct regardless of identifier and underlying type
// r [type] = r -> rune

var x rune = '\n';
var y = r('\n');

func compare() bool{ return x == y;}


func main() {
}
