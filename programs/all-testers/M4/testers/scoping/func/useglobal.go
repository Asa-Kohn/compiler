//~+0.000000e+000

package main

var a = 0.;

func somefunc(b float64) float {
	return b + a + 1.
}

func main() {
	print(somefunc(-1.));
}
