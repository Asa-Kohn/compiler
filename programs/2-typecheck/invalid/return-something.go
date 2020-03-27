// Error: (line 12) invalid return [function has void return type]
package main

var x int
var y float64
var z bool
var a rune
var b string
var avar int

func function() {
	return 0
}

func main() {
	function();
}
