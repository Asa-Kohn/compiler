// Error: (line 13) void cannot be used as a value in short declaration
package main

var x int
var y float64
var z bool
var a rune
var b string
var avar int

func afunc(){}
func bfunc(){
	a := afunc();
}


func main(){
}
