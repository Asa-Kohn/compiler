// Error: (line 25) assigment lhs type is incompatible with rhs type [int != float64]
// block within function does not typecheck
package main

type num int
type struc struct{ a, b int; }

var x []int
var y = num(0)

var a []struc
var aa struc
var b = num(2)

var c int
var d float64
var e bool
var f rune
var g string

func function(x int) int {
	{
		var a int = 0
		var b float64 = 0.0
		return a + b;
	}
	// print(a)
}

func main() {
	function(0)
}
