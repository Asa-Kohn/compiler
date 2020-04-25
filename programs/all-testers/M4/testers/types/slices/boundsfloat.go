//! out of bounds error

package main

func main() {

	var s []float
	s = append(s, 0.)

	print(s[1]);

}
