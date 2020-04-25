//~ 01

package main

func main() {
	var a [2]int
	a[0], a[1] = 0, 1

	var b, c = a, a;
	print(b[0], c[1])
}
