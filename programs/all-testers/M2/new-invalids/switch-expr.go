// Error: (line 32) assigment lhs type is incompatible with rhs type [float64 != int]
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

	var i int;

	switch i := 0 ; true {

	}

	switch i := 0.0 ; i == 0 {

	}

	switch i := true ; true {

	}

	switch i := '\n' ; true {

	}

	switch i := "string" ; true {

	}

}


func main() {
	function();
}
