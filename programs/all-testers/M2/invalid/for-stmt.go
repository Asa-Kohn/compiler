// Error: (line 26) int is not assignment compatible with bool in assign statement
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
		var j bool;
		j = 0;
	}
	for i := d ; i == 0.0 ;  i ++ {

	}
	for i := e ; true ; i = false {

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
