// A short declaration type checks if:
// 1. All the expressions on the right-hand side are well-typed;
// 2. At least one variable on the left-hand side is not declared in the current scope;
// 3. The variables already declared in the current scope are assigned expressions of the
// same type. E.g. if the symbol table contains the mapping x1 -> T1, then it must be
// the case that typeof(e1) = T1.
package main

func function() {
	var a int;
	a, b, c, d, e := 0, 0.0, false, '\n', "wow";
}

func main() {
}
