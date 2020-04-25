//! out of bounds error

package main

func main() {

	var s []bool
	s = append(s, true)

	print(s[1]);

}
