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

var aaa, bbb, ccc int;
var ddd, eee, fff float64;
var ggg, hhh, iii rune;
var jjj, kkk, lll string;
var mmm, nnn, ooo bool;

var ()
var (
	lilstring, bigstring string
	lilint               int
	bigfloat             float64
	yesno         		 bool
	runned 				 rune
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

//typey types
var jj btype;
type btype = bool;
var kk btype;
var oo floattype;
type floattype = float64;
var pp inttype;
type inttype = int;
var qq stringtype;
type stringtype = string;

type ()
type (
	lots int
	and float64
	lotz string
	types bool
)
type structed struct {
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
