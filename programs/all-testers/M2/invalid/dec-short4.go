// Error: (line 15) []int is not assignment compatible with []float64 in short declaration
package main

//make array
var array []int

func function() {

	array = append(array, 7)
	array = append(array, 4)

	//let b be int array
	var b []float64

	a, b, c, d, e := array, array, false, '\n', "wow"
}

func main() {
}
