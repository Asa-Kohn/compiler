//! out of bounds error

package main

func main() {

	var s []rune
	s = append(s, '0')

	print(s[1]);

}
