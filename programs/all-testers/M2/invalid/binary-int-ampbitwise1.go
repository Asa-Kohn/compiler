// Error: (line 33) incompatible type in arithmetic op &^ [received bool, expected integer (int, rune)]
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
	
	// integer &^ integer
	c = e &^ e;
	
	// integer ^ integer
	c = c ^ c;
}


func main() {
	function();
}