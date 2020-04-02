package main

type num int
type struc struct{ a, b int; }

func main() {
	var x []struc
	var y = num(0)
	// var z struc

	var a []struc
	var b = num(2)
	x, y, z := a, b, a;
}