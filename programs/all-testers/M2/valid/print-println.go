//
package main

type num int
type struc struct{ a, b int; }

var x []int
var y = num(0)

var a []struc
var aa struc
var b = num(2)

var c int
var d float64
var e bool
var f rune
var g string


func main() {
	print(c)
	println(c)
	print(d)
	println(d)
	print(e)
	println(e)
	print(f)
	println(f)
	print(g)
	println(g)

	//typedef
	print(y)
	print(aa.a)
	print(x[0])
	print(a[0].a)
}
