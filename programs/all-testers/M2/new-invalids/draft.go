// Error: (line 27) incompatible type in loop condition [received float64, expected bool]
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

func function() int {
	for i := c ; i == 0 ;  {

	}
	for i := d ; 0.0 + 0.0 ;  {

	}
	for i := e ; aa.a ;  {

	}
	for i := f ; i == '\n' ;  {

	}
	for i := g ; i == "wow" ;  {

	}
	return 0;
}


func main() {
	function();
}
