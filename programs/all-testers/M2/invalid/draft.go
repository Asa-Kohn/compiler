package main

type num int
type struc struct{ a, b int; }

func main() {
	var x int = 0
	var y = num(0)
	// var z struc

	var a struc
	var b = num(2)
	x, y, z := 0, b, a;
}

// var array []int

// func function() {

// 	array = append(array, 7)
// 	array = append(array, 4)

// 	//let b be int array
// 	var b []float64

// 	a, b, c, d, e := array, array, false, '\n', "wow"
// }

// func main() {
// }
