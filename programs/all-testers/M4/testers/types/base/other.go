//~03880240129900-1-98097

package main

func main() {

	var ai int = 0

	var ar rune = 'a'

	// left
	print(ai << 2)
	print(ar << 'a')

	// right
	print(ai >> 2)
	print(ar >> 'a')

	// mod
	print(ai % 2)
	print(ar % 'a')

	// or
	print(ai | 2)
	print(ar | 'a')

	// and
	print(ai & 2)
	print(ar & 'a')

	// bitwise not
	print(^ai)
	print(^ar)

	// bitwise not and
	print(ai &^ 2)
	print(ar &^ 'a')

}
