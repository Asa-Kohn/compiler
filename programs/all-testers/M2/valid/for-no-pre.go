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

func function() int {
	i := c
	for ; i == 0 ; {

	}

	j := d 
	for ; j == 0.0 ; {

	}

	k := e
	for ; k == true ; {

	}

	l := f
	for ; l == '\n' ;  {

	}
	
	m := g
	for ; m == "wow" ;  {

	}
	return 0;
}


func main() {
	function();
}
