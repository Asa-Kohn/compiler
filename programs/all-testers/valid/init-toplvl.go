//program with main
package main

var yes = true;
var  no = false;
var (
	kingston, yapp, trey = 1, 2, 3;
	lulu, echo, delta = yes, no, yes;
	solo, hawaii, alpha, omega float64 = 0.0, -.1, 2., .9; 
)

var initint = 0;
var initint1 = -5;
var initfloat = 0.0;
var initfloat1 = -.0;
var initfloat2 = 1.;
var initstring = "golang is weird";
var initbool = false;

var intplus = initint + initint1;
var intminus = initint - initint1;
var intmult = initint * initint1;
var intdiv = initint / initint1;

var floatplus = initfloat + initfloat1;
var floatminus = initfloat1 - initfloat2;
var floatmult = initfloat * initfloat2;
var floatdiv = initfloat / initfloat2;	//test division by zero?

// var mixymix = initint + initfloat; //type mismatch


func main() {
	println("init-toplvl executed");
}