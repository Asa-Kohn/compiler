package main

type num int
type struc struct{ a, b int; }

var x int
var y = num(0)

var a struc
var b = num(2)

func main() {
	var c, d, e int = a.a, x, a.b
}