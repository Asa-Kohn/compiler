// Error: (line 12) function function does not have a terminating statement [loop condition not empty]
package main

var x int
var y float64
var z bool
var a rune
var b string
var avar int

func function(x int) int {
	for i := 0 ; i < 10 ; i++ {
		return 0;
	}
}

func main() {
}
