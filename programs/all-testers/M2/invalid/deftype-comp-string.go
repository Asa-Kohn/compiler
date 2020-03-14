// Error: (line 12) assigment lhs type is incompatible with rhs type [string != s]
package main

type s string;
// symbol table expected output
// defined types are distinct regardless of identifier and underlying type
// s [type] = s -> string

var x string = "word";
var y = s("word");

func compare() bool{ return x == y;}


func main() {
}
