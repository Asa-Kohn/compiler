//~01

package main

func main() {
	var a []int;
	a=append(a, 0)
	a=append(a, 1)

	var b []int;
	b=append(a, 0)
	b=append(a, 1)
	print(a[0], b[1])
}
