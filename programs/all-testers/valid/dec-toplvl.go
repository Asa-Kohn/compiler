//top-level declarations
//variable declarations
//type declarations
//function declarations
package main

//var
var aa int;
// var bb float32;
var bb float64;
var cc rune;
var dd string;
var ee bool;

var (
	lilstring, bigstring string
	lilint               int
	bigfloat             float64
	yesno         		 bool
)

//funky funcs
var ff = calculation;
func calculation(){}
func more(){}
var gg = more;
var hh = ff;
var ii = gg;

func intfunc() int{ return 0;}
func intfuncc() int{ return 0;}
func floatfunc() float64{ return 0;}
func floatfuncc() float64{ return 0;}
func stringfunc() string{ return "oh no";}
func boolfunc() bool{ return true;}

var ll = intfunc() + intfuncc();
var mm = floatfunc() + floatfuncc();
var nn = stringfunc() + stringfunc();
//var oo = boolfunc() + boolfunc();	//operator + not defined

//typey types
var jj btype;
type btype = bool;
var kk btype;
type floattype = float64;
type inttype = int;
type stringtype = string;

type (
	lots int
	and float64
	lotz string
	types bool
)
type structed struct {	//struct requires curly brackets
	echo, delta int
	zeta, chi float64
	bondi, zolo string
	lambda, mango, query bool
}
type (
	yam float64
	structedtwice struct{
		echo string
	}
)


var a = b;
var b = c;
var c = 0;		// this requires expression. cannot be type or identifier.
var d = e + f;
var e = 0;
var f = 0;

func main() {
	//note : swapped declaration
	// var aa bool;
	// var bb string;
	// var cc int;
	// var dd float64;
	// var ee rune;
	println("dec-toplvl executed");
}