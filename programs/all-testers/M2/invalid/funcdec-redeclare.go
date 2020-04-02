// Error: (line 12) function already declared on line 11
package main

var x int;
var y float64;
var z bool;
var a rune;
var b string;
var avar int;

func function(function int) {
	var function float64;	// Redeclares function (the formal parameter)
}


func main() {
}
