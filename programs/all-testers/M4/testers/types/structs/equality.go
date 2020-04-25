//~ 1

package main

type point struct {
	x float
	y float
}

func main() {
	var a point
	a.x, a.y = 0.0, 0.0

	var b point
	b.x, b.y = 1.0, -1.0

	b.x, b.y = b.x + b.y, b.x + b.y

	if a == b {
		print(1)
	}
}
