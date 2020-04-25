//! out of bounds

package main

func main() {

	var s [3]bool

	s[3] = true;

	print(-1);

}
