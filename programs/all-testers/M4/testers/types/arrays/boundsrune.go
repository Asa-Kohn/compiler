//! out of bounds

package main

func main() {

	var s [3]rune

	s[3] = '0';

	print(-1);

}
