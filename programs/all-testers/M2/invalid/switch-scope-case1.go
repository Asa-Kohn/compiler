// Error: (line 20) j is not declared
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
		j := 0
		return 1;
	default:
		// "j" is undefined here
		return 0;
	}
	return j;
	// "j" is undefined here
}

func main() {
}
