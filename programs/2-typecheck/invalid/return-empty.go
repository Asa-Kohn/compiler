// a return statement with no expression is well-typed if the enclosing function has no return type
package main

var x int
var y float64
var z bool
var a rune
var b string
var avar int

func function() int {
	return
}

func main() {
	function();
}
