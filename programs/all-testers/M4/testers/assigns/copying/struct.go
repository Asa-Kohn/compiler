//~0

package main

type integer struct {
	value int
}

func main() {
	var a integer
	a.value = 0

	var b = a;
	print(b.value)
}
