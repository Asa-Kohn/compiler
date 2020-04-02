// Error: (line 25) unexpected escape sequence '''
package main

type num int
type struc struct{ a, b int; }

var x []int
var y = num(0)

var a []struc
var aa struc
var b = num(2)

var op int = 0;
var op1 float64 = 0.0;
var op2 bool = false;
var op3 rune = '\n';
var op4 string = "wow"

func main() {
	op += 0;
	op1 += 0.0;
	// op2 += true;
	op3 += '\n';
	op4 += "\'";
}
