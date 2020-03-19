// Error: (line 16) function function does not have a terminating statement [final statement not terminating]
package main

var x int
var y float64
var z bool
var a rune
var b string
var avar int

func f() int {
	return 0
}

func function(x int) int {
	f();
}

func main() {
}
