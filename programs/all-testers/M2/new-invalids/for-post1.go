// Error: (line 36) incompatible type in increment [received string, expected numeric (int, rune, float64)]
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
	for i := c ; i == 0 ; i++ {

	}
	for i := d ; i == 0.0 ;  i ++ {

	}
	for i := e ; true ; {

	}
	for i := f ; i == '\n' ; i++ {

	}
	for i := g ; i == "wow" ; i++ {

	}
	return 0;
}


func main() {
	function();
}
