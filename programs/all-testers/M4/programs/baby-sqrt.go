//~babySqrt: +3.464102e+000

/*
approximating square root using babylonian method
https://en.wikipedia.org/wiki/Methods_of_computing_square_roots#Babylonian_method
*/
package main

func babySqrt(x float64) float64 {
    var precision float64 = 0.0000000000001;
    var approx float64 = x;

    for (approx - x / approx) > precision {
        approx = (approx + x / approx) / 2.0;
	}
	// trunc := float64(int(approx * 100)) / 100
	// return trunc;
	return approx;
}

func main() {
	var x float64 = 12.0;
	println("babySqrt:", babySqrt(x));
}
