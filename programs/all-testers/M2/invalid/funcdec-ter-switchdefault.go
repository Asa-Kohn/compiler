// Error: (line 11) function function does not have a terminating statement [not returned on all paths]
package main

var x int
var y float64
var z bool
var a rune
var b string
var avar int

func function(x int) int {
	switch i := 2; i * 4 {
	case 8:
		return 1;
	default:
	}
}

func main() {
}
