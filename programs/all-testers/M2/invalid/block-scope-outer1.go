// Error: (line 25) int is not assignment compatible with []int in return statement
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

func function() int {
	{
		var a int
		return a;
	}
	return x;
}

func main() {
	function()
}
