// Error: (line 33) switch statement expresion type is incompatible with case type [rune != float64]
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

	switch i := 0.0; i {
		case '\n':

	}

	switch i := true; i {
		case true:
	}

	switch i := '\n'; i {
		case '\n': 

	}

	switch i := "string"; i {
		case "string":
		default:
	}

}


func main() {
	function();
}
