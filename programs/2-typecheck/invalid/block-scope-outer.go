// Error: (line 26) int is not assignment compatible with []struc in return statement
// print statement on line 26 refers to the int array in the outer scope
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
		return a;
	}
	return a;
}

func main() {
	function(0)
}
