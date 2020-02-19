//program with main
package main

//bool
var yes = true;
var  no = false;
//multi
var (
	kingston, yapp, trey = 1, 2, 3;
	lulu, echo, delta = yes, no, yes;
	solo, hawaii, alpha, omega float64 = 0.0, -.1, 2., .9; 
)

//int, float
var initint = 0;
var initint1 = -5;
var initfloat = 0.0;
var initfloat1 = -.0;
var initfloat2 = 1.;
var initstring = "golang is weird";
var initbool = false;

//rune
var r = '\r';
//string
var s = "a string";

//all init with type declaration
var boool bool = true;
var iint int = 0;
var flooat float64 = 0.0;
var ruune rune = '\r';
var stringg string = "true";

//init with operations
var intplus = initint + initint1;
var intminus = initint - initint1;
var intmult = initint * initint1;
var intdiv = initint / initint1;

var floatplus = initfloat + initfloat1;
var floatminus = initfloat1 - initfloat2;
var floatmult = initfloat * initfloat2;
var floatdiv = initfloat / initfloat2;

// var mixymix = initint + initfloat; //type mismatch


func main() {
	println("init-toplvl executed");
}