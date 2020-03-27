// Error: (line 9) incompatible type in logical op && [received bool, expected bool]
package main

var d bool = false;
var e bool = false;


func compare() bool{
	return d == e;
}

func main() {
	compare();
}
