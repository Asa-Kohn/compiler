// Error: (line 12) rune is not assignment compatible with bool in return statement
package main

var x int
var y float64
var z bool
var a rune
var b string
var avar int

func function() rune {
	return true
}

func main() {
	function();
}
