// Error: (line 22) print expects base types [received []struc]
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


func main() {
	print(a)
}
