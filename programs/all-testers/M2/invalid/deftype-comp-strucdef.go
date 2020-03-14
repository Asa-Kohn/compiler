// Error: (line 12) assigment lhs type is incompatible with rhs type [num != int]
package main

type num int;
type struc struct{ a, b int; }	// syntax error if semi-colon not inserted here
// however, https://golang.org/ref/spec#Type_identity, golang specs did not use semi-colon for struct def

var x int = 0;
var y = num(0);
var z struc;

func assign() bool{ 
	return y == z.a;
}


func main() {
}
