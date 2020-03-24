// a return statement with an expression is well-typed if its expression is well-typed 
// and the type of this expression is the same as the return type of the enclosing function 
package main

var x int
var y float64
var z bool
var a rune
var b string
var avar int

func function() int {
	return 0
}

func main() {
	function();
}
