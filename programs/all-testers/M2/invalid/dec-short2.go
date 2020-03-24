// Error: (line 7) int is not assignment compatible with float64 in short declaration
package main


func function() {
	var a float64; 
	a, b, c, d, e := 0, 0.0, false, '\n', "wow";
}

func main() {
}
