// Error: (line 12) assigment lhs type is incompatible with rhs type [num != int]
package main

type num int;
type struc struct{ a, b int; }

var x int = 0;
var y = num(0);
var z struc;

func assign() bool{ 
	return y == z.a;
}


func main() {
}
