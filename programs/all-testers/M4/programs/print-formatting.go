//~255 255 +1.200000e-001 true 76 hello
//~hello\n 

package main

func main() {
	// Integers
	print(255, " ")
	print(0377, " ")

	// Floats
	print(0.12, " ")

	// Booleans
	print(true, " ")

	// Runes
	print('L', " ")

	// Strings
	print("hello\n")
	print(`hello\n`, " ")
}