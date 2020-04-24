// Error: (line 53) capacity builtin expects slice or array type as argument [received int]
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
	a_slice = append(a_slice, 7);
	a_slice = append(a_slice, 4);
	a_slice = append(a_slice, 3);
	a_slice = append(a_slice, 1);
	a_slice = append(a_slice, 99);
	a_slice = append(a_slice, 13);
	a_slice = append(a_slice, 5);
	a_slice = append(a_slice, 8);
	a_slice = append(a_slice, 10);
	a_slice = append(a_slice, 55);
	
	//a_index type using indexing
	var a_index [6]int;
	a_index[0] = 7;
	a_index[1] = 4;
	a_index[2] = 3;
	a_index[3] = 1;
	a_index[4] = 99;
	a_index[5] = 13;

	print(cap(0));
}


func main() {
	function();
}
