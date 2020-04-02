// Error: (line 35) syntax error, unexpected '{', expecting ';'
// golite does not allow short declaration for structs 
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


func init() {
	aa.a = false;
	confused.a = false;

	incredible := confusion{};
	print(incredible.a);
}

func main() {
}