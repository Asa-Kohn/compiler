//
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
	// bool
	e = e || e;
	e = e && e;

	//comparable
	e = c == c;
	e = c != c;
	e = d == d;
	e = d != d;
	e = e == e;
	e = e != e;
	e = f == f;
	e = f != f;
	e = g == g;
	e = g != g;

	//ordered
	// in ordered file

	//numeric string
	// in add file

	//numeric, int rune float
	c = c - c;
	c = c * c;
	c = c / c;
	d = d - d;
	d = d * d;
	d = d / d;
	f = f - f;
	f = f * f;
	f = f / f;

	//integer
	// in intops file
}


func main() {
	function();
}
