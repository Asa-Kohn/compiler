// At least one variable on the left-hand side is not declared in the current scope;
package main


func function() {
	var a int; 
	var b float64;
	var c bool;
	var d rune;
	a, b, c, d, e := 0, 0.0, false, '\n', "wow";
}

func main() {
}
