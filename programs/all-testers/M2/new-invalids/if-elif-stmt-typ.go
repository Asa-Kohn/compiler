// Error: (line 29) float64 is not assignment compatible with int in assign statement
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

	if true {

	} else if c == 0 || d == 0.0 && f != f {
		var invalid int = 0;
		invalid = 0.0;
		
	}

}


func main() {
	function();
}
