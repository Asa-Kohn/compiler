// Error: (line 12) bool is not assignment compatible with int in return statement
package main

var x int
var y float64
var z bool
var a rune
var b string
var avar int

func function() bool {
	return 0
}

func main() {
	function();
}
