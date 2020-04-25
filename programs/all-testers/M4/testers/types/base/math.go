// generated from template.go

package main

func main() {

	var ai : int = 0
	var bi : int = 1
	var ci : int = 2
	var di : int = 3

	var af : float = 0.
	var bf : float = 1.
	var cf : float = 2.
	var df : float = 3.

	var as : string = "a"
	var bs : string = "b"
	var cs : string = "c"
	var ds : string = "d"

	var ar : rune = 'a'
	var br : rune = 'b'
	var cr : rune = 'c'
	var dr : rune = 'd'

	// add
	print(ai + bi + ci + di)
	print(af + bf + cf + df)
	print(ar + br + cr + dr)
	print(as + bs + cs + ds)

	// sub
	print(ai - bi - ci - di)
	print(af - bf - cf - df)
	print(ar - br - cr - dr)

	// mult
	print(ai * bi * ci * di)
	print(af * bf * cf * df)
	print(ar * br * cr * dr)

	// div
	print(ai / bi / ci / di)
	print(af / bf / cf / df)
	print(ar / br / cr / dr)

	// umin
	print(-ai, -bi, -ci, -di)
	print(-af, -bf, -cf, -df)
	print(-ar, -br, -cr, -dr)

	// uplus
	print(+ai, +bi, +ci, +di)
	print(+af, +bf, +cf, +df)
	print(+ar, +br, +cr, +dr)

}
