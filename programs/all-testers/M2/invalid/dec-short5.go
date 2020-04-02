// Error: (line 13) int is not assignment compatible with num in short declaration
package main

type num int
type struc struct{ a, b int; }

func main() {
	var x num
	var y = num(0)
	// var z struc

	var a struc
	var b = num(2)
	x, y, z := a.a, b, a;
}