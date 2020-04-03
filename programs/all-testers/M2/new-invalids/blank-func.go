// Error: (line 12) function name may not contain the blank identifier
package main

func _() {
}

var a struct {
	_ int
}

func main() {
	_() // Error: Cannot call blank function
}
