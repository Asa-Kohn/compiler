// Error: (line 11) assigment lhs type is incompatible with rhs type [bool != b]
package main

type b bool;
// symbol table expected output
// defined types are distinct regardless of identifier and underlying type
// b [type] = b -> bool

var x bool = false;
var y = b(false);

func compare() bool{ return x == y;}


func main() {
}
