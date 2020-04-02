// Error: (line 29) assigment lhs type is incompatible with rhs type [float64 != int]
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

func nonBool() string {
	return "string"
}

func function() int {

	if j := 0.0 ; j%5 == 0.0 {
		
	} else {

	}

	return 0;
}


func main() {
	function();
}
