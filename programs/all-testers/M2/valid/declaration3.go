package main

type num int
type struc struct{ a, b int; }

var x []int
var y = num(0)

var a []struc
var b = num(2)

func main() {
	x = append(x, 1)
	x = append(x, 2)
	var c, d, e []struc = a, a, a
	var f, g, h []int = x, x, x
}