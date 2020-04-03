//
package main

type num int
type struc struct{ a, b bool; }
type confusion struc

var _ []int
var _ = num(0)

var _ []struc
var _ struc
var _ = num(2)

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
