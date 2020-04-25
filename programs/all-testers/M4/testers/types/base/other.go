//~0
//~0
//~0
//~0
//~2
//~97
//~0
//~97
//~-1
//~-98
//~0
//~0

package main

func main() {

	var ai int = 0

	var ar rune = 'a'

	// left
	println(ai << 2)
	// println(ar << 'a') //ar (32 bits) too small for shift of 97

	// right
	println(ai >> 2)
	// println(ar >> 'a') //ar (32 bits) too small for shift of 97

	// mod
	println(ai % 2)
	println(ar % 'a')

	// or
	println(ai | 2)
	println(ar | 'a')

	// and
	println(ai & 2)
	println(ar & 'a')

	// bitwise not
	println(^ai)
	println(^ar)

	// bitwise not and
	println(ai &^ 2)
	println(ar &^ 'a')

}
