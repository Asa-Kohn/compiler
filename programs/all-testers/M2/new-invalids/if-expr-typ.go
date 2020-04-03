// Error: (line 25) assigment lhs type is incompatible with rhs type [rune != string]
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

	if c == 0 || d == 0.0 && f != "string" {
		
	}

}


func main() {
	function();
}
