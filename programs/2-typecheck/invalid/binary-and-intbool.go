// Error: (line 11) assigment lhs type is incompatible with rhs type [int != bool]
package main

var d int = 0;
var e bool = true;


func compare() bool{
	return d && e;
}

func main() {
	compare();
}
