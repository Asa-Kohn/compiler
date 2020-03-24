// The variables already declared in the current scope are assigned expressions of the
// same type. E.g. if the symbol table contains the mapping x1 -> T1, then it must be
// the case that typeof(e1) = T1.
package main


func function() {
	var x int = 0;
	a, b, c, d, e := x, 0.0, false, '\n', "wow";
}

func main() {
}
