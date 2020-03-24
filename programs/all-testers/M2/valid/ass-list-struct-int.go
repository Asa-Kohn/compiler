package main

type num int
type struc struct{ a, b int; }

var x []int
var y = num(0)

var a []struc
var aa struc
var b = num(2)

func main() {
	x = append(x, 1)
	x = append(x, 2)
	x = append(x, aa.a)	//aa.a => int

	var c, d, e []struc
	var f, g, h []int

	c, d, e = a, a, a
	f, g, h = x, x, x
}