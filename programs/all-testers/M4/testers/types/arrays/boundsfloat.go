//!out of bounds

package main

func main() {

	var s [3]float64

	s[3] = 0.;

	print(-1);
}
