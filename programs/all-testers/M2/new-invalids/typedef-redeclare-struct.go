// Error: (line 8) struc already declared on line 5
package main

type num int
type struc struct{ a, b, _ bool; }
type confusion struc

type struc float64

var x []int
var y = num(0)

var a []struc
var aa struc
var b = num(2)

var confused confusion

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

func main() {
}
