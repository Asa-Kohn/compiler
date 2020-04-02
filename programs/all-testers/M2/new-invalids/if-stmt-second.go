// Error: (line 41) int is not assignment compatible with bool in assign statement
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

	}

	if j := 0 ; j == 0 {
		
	} else {
		var j bool = false;
		j = 0;
	}

	return 0;
}


func main() {
	function();
}
