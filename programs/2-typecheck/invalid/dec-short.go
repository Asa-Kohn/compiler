// Error: (line 11) short declaration contains no new variables
package main


func function() {
	var a int; 
	var b float64;
	var c bool;
	var d rune;
	var e string;
	a, b, c, d, e := 0, 0.0, false, '\n', "wow";
}

func main() {
}
