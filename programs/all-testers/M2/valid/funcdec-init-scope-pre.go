//
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


//slice type using append
var aSlice []int;
//aIndex type using indexing
var aIndex [6]int;

func init() {

	aSlice = append(aSlice, 7);
	aSlice = append(aSlice, 4);
	aSlice = append(aSlice, 3);

	aIndex[0] = 7;
	aIndex[1] = 4;
	aIndex[2] = 3;
}


func main() {
	println(aSlice[0], aSlice[1], aSlice[2]);
	println(aIndex[0], aIndex[1], aIndex[2]);
}

func init() {

}
