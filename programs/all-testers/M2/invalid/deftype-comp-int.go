// Error: (line 11) assigment lhs type is incompatible with rhs type [int != i]
package main

type i int;
// symbol table expected output
// defined types are distinct regardless of identifier and underlying type
// i [type] = i -> int

var x int = 0;
var y = i(0);

func compare() bool{ return x == y;}


func main() {
}
