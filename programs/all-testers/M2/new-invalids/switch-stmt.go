//Error: (line 35) float64 is not assignment compatible with int in assign statement
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

	switch i := 0 ; 0+0 == 6 {
		case e, ee:
			var i int;
			i = 0.0;
	}

	switch i := 0.0 ; 0.0 < 9.0 {

	}

	switch i := true ; false == true {

	}

	switch i := '\n' ; '\n' {
		case '\n': 

	}

	switch i := "string" ; "string" {
		case "string":
		default:
	}

}


func main() {
	function();
}
