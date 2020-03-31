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


func function() {

	switch i := 0 ; 0+0 == 6 {
	case e, ee:

	}

	switch i := 0.0 ; 0.0 < 9.0 {

	}

	switch i := true ; false == true {

	}

	switch i := '\n' ; '\n' {

	}

	switch i := "string" ; "string" {

	}

}


func main() {
	function();
}
