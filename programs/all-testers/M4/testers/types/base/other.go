//~ 03880240129900-1-98097

package main

func main() {

	var ai int = 0

	var ar rune = 'a'

	// left
	print(ai << 2)
	print(ar << 2)

	// right
	print(ai >> 2)
	print(ar >> 2)

	// mod
	print(ai % 2)
	print(ar % 2)

	// or
	print(ai | 2)
	print(ar | 2)

	// and
	print(ai & 2)
	print(ar & 2)

	// bitwise not
	print(^ai)
	print(^ar)

	// bitwise not and
	print(ai &^ 2)
	print(ar &^ 2)

}
