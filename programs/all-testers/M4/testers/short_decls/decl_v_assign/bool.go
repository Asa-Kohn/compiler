//~true
//~true
//~true

package main

func main() {
	var a = true
	a, b, c := a, a, a
	println(a)
	println(b)
	print(c)
}
