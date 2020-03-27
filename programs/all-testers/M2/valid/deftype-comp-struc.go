package main

type num int;
type struc struct{ a, b int; }

var x int = 0;
var y = num(0);
var z struc;

//z.a has type int
func compare() bool{ 
	return x == z.a;
}

func comapare1() bool{ 
	return z == z;
}


func main() {
}
