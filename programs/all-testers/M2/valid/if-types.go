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


func function() {
	if i := 0 ; true {

	}

	if i := 0.0 ; true {

	}

	if i := true ; true {

	}

	if i := '\n' ; true {

	}

	if i := "string" ; true {

	}

}


func main() {
	function();
}
