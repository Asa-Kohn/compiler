// Error: (line 50) incompatible type in unary op + [received bool, expected numeric (int, rune, float64)]
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


func unar() {
	// int float rune plus
	c = +c;
	d = +d;
	f = +f;

	// int float rune neg
	c = -c;
	d = -d;
	f = -f;

	// bool negation
	e = !e;

	// int rune bitwise negation
	cc = ^cc;
	ff = ^ff;


	// bool string plus
	e = +e;
	// g = +g;

	// bool string neg

	// int float rune string negation

	// float bool string bitwise negation
}


func main() {
	unar();
}
