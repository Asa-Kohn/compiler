// Error: (line 4) rune is not assignment compatible with string in variable declaration
// cannot use '\u000d' (type rune) as type string in assignment
package main

var s string = '\r';


func main() {
}
