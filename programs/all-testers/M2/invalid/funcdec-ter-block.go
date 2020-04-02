// Error: (line 13) function function does not have a terminating statement [final statement not terminating]
package main

var x int
var y float64
var z bool
var a rune
var b string
var avar int

func function(x int) int {
	{
		var a int = 0
		// return a;
	}
}

func main() {
}
