// Error: (line 12) assigment lhs type is incompatible with rhs type [float64 != f]
package main

type f float64;
// symbol table expected output
// defined types are distinct regardless of identifier and underlying type
// f [type] = f -> float64

var x float64 = 0.0;
var y = f(0.0);

func compare() bool{ return x == y;}


func main() {
}
