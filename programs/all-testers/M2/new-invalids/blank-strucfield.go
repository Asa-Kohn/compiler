// Error: (line 30) selector field may not contain the blank identifier
package main

type num int
type struc struct{ a, b, _ bool; }
type confusion struc

var x []int
var y = num(0)

var a []struc
var aa struc
var b = num(2)

var confused confusion

var c int
var d float64
var e bool
var f rune
var g string

var cc int
var dd float64
var ee bool
var ff rune
var gg string

func main() {
	print(aa._);
}
