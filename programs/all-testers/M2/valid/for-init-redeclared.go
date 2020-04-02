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

	i := 0;


	for i := 0 ; i == 0 ; i++ {
		
		i := 0;

	}
	for i := d ; i == 0.0 ;  i ++ {

		i := 0

	}
	for i := e ; true ; i = false {

		i := "redeclared in new scope"

	}
	for i := f ; i == '\n' ; i++ {

	}
	for i := g ; i == "wow" ; i = "new" {

	}
	return 0;
}


func main() {
	function();
}
