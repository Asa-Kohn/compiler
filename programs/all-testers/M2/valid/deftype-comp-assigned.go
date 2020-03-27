package main

type num int;
type struc struct{ a, b int; }

var x int = 0;
var y = num(0);
var z = y;

//assigned z as y 
func compare() bool{ 
	return y == z;
}


func main() {
}
