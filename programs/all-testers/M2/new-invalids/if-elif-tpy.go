// Error: (line 29) assigment lhs type is incompatible with rhs type [rune != float64]
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

func function() {

	if false {

	} else if true {

	} else if c == 0 || d == 0.0 && f != d {
		
	} else {

	}

}


func main() {
	function();
}
