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


func function() {
	switch i := 0 ; {

	}

	switch i := 0.0 ; {

	}

	switch i := true ; {

	}

	switch i := '\n' ; {

	}

	switch i := "string" ; {

	}

}


func main() {
	function();
}
