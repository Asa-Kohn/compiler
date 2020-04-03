// Error: (line 33) indexing list may not contain the blank identifier
package main

type num int
type struc struct{ a, b bool; }
type confusion struc

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

//a_index type using indexing
var a_index [6]int;

func function() {
	_[0] = 7
	a_index[1] = 4
	a_index[2] = 3
	a_index[3] = 1
	a_index[4] = 99
	a_index[5] = 13
}

func main() {
}
