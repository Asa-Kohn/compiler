// Error: (line 34) incomatible type in arithmetic op + [received bool, expected numeric (int, rune, float64) or string]
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


func function() {
	// numeric or string + numeric or string
	c = c + c;
	d = d + d;
	e = e + e;
	f = f + f;
	g = g + g;
}


func main() {
	function();
}
