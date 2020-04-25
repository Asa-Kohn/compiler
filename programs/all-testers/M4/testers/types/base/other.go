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

	var ar : rune = 'a'
	var br : rune = 'b'
	var cr : rune = 'c'
	var dr : rune = 'd'

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
	print(&^ai)
	print(&^ar)

}
