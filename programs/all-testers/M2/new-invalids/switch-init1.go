// Error: (line 38) float64 is not assignment compatible with int in assign statement
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

	var i int;

	switch i := 0; {

	}

	switch i = 0.0; {

	}

	switch i := true; {
		case true:

	}

	switch i := '\n'; {

	}

	switch i := "string"; {
		default:
	}

}


func main() {
	function();
}
