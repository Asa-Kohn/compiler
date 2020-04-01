// Error: (line 33) append expression slice type is incompatible with element type [int != float64]
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


func function() {
	//slice type using append
	var a_slice []int;
	a_slice = append(a_slice, 0.0);
	a_slice = append(a_slice, 4);
	a_slice = append(a_slice, 3);
	a_slice = append(a_slice, 1);
	a_slice = append(a_slice, 99);
	a_slice = append(a_slice, 13);
	a_slice = append(a_slice, 5);
	a_slice = append(a_slice, 8);
	a_slice = append(a_slice, 10);
	a_slice = append(a_slice, 55);
}


func main() {
	function();
}
