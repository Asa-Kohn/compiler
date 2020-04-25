//~6
//~+6.000000e+000
//~394
//~abcd
//~-6
//~-6.000000e+000
//~-200
//~0
//~+0.000000e+000
//~94109400
//~0
//~+0.000000e+000
//~0
//~0 -1 -2 -3
//~-0.000000e+000 -1.000000e+000 -2.000000e+000 -3.000000e+000
//~-97 -98 -99 -100
//~0 1 2 3
//~+0.000000e+000 +1.000000e+000 +2.000000e+000 +3.000000e+000
//~979899100

package main

func main() {

	var ai int = 0
	var bi int = 1
	var ci int = 2
	var di int = 3

	var af float64 = 0.
	var bf float64 = 1.
	var cf float64 = 2.
	var df float64 = 3.

	var as string = "a"
	var bs string = "b"
	var cs string = "c"
	var ds string = "d"

	var ar rune = 'a'
	var br rune = 'b'
	var cr rune = 'c'
	var dr rune = 'd'

	// add
	println(ai + bi + ci + di)
	println(af + bf + cf + df)
	println(ar + br + cr + dr)
	println(as + bs + cs + ds)

	// sub
	println(ai - bi - ci - di)
	println(af - bf - cf - df)
	println(ar - br - cr - dr)

	// mult
	println(ai * bi * ci * di)
	println(af * bf * cf * df)
	println(ar * br * cr * dr)

	// div
	println(ai / bi / ci / di)
	println(af / bf / cf / df)
	println(ar / br / cr / dr)

	// umin
	println(-ai, -bi, -ci, -di)
	println(-af, -bf, -cf, -df)
	println(-ar, -br, -cr, -dr)

	// uplus
	println(+ai, +bi, +ci, +di)
	println(+af, +bf, +cf, +df)
	print(+ar, +br, +cr, +dr)

}
