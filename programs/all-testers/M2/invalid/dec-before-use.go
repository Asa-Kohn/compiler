// Error: (line 6) e is not declared
// valid in Go, but not in GoLite. must have declaration before use 
package main

var d = e + f
var e = 0
var f = 0

func main() {
}
