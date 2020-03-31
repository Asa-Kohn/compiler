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

func nonBool() string {
	return "string"
}

func function() int {

	if true {
		
	}

	if i := 0 ; i == 0 {

		i := true;

	}

	if j := 0 ; j == 0 {

		j := true;
		
	} else {

		j := "j redeclared"

	}

	return 0;
}


func main() {
	function();
}
