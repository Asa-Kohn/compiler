// Error: (line 14) selector target must be a struct type [received []struc]
package main

type num int
type struc struct{ a, b int; }

func main() {
	var x []int
	var y = num(0)
	// var z struc

	var a []struc
	var b = num(2)
	x, y, z := a.a, b, a;
}