//~ 2

package main

func main() {
	var a [2]int
	a[0], a[1] = 0, 1

	print(cap(a))
}
