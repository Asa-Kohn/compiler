// Error: (line 31) incompatible type in increment [received []int, expected numeric (int, rune, float64)]
package main

type num int
type struc struct{ a, b bool; }
type confusion struc;

var x []int
var y = num(0)

var a []struc
var aa struc
var b = num(2)

var confused confusion;

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


func inc() {
	x++;
}


func main() {
	inc();
}
