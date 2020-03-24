// Error: (line 9) float64 is not assignment compatible with []int in short declaration
package main


func function() {
	var x int = 0;
	var b []int;

	a, b, c, d, e := x, 0.0, false, '\n', "wow";
}

func main() {
}
