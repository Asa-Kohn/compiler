// Error: (line 9) incompatible type in logical op && [received int, expected bool]
package main

var d int = 0;
var e int = 0;


func compare() bool{
	return d && e;
}

func main() {
	compare();
}
