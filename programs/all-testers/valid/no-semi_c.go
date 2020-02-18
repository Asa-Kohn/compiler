//print numerical lits
package main

func main() {
	var a int
	var b int = 5
	// var c float32 = 6.6
	// var d float32 = .7
	// var e float32 = 8.
	var f float64 = 9.9
	var g float64 = .10
	var h float64 = 11.
	var i rune = '\n'
	var j string = "string"

	for k := 0; k == 0; k++ {
		continue
	}

	for {
		break
	}

	switch 1 {
	case 1:
		fallthrough
	case 0:
	}

	a++
	a--
	var k = (a + a)

	var l = []int{3, 5, 7, 9, 11, 13, 17}
	var m = l[0]

	println("no-semi_c executed")
	// println(a, b, f, g, h, i, j, k, l, m)
	// fmt.Println(a, b, f, g, h, i, j, k, l, m)
	// fmt.Println(a, b, c, d, e, f, g, h, i, j, k, l, m)
	return
}
