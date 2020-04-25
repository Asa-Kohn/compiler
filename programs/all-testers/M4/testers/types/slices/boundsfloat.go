//!out of bounds error

package main

func main() {

	var s []float64
	s = append(s, 0.)

	print(s[1]);

}
