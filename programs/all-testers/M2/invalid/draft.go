// Error: (line 14) assigment lhs type is incompatible with rhs type [num != natural]
package main

type num int;
type natural num;
// symbol table expected output
// defined types are distinct regardless of identifier and underlying type
// num [type] = num -> int
// natural [type] = natural -> num -> int

var x int = 0;
var y = num(0);
var z = natural(num(0));

func assign() bool{ 
	y = z;
	return y;
}


func main() {
}
