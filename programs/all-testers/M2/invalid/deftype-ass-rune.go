// Error: (line 13) r is not assignment compatible with rune in assign statement
package main

type r rune;
// symbol table expected output
// defined types are distinct regardless of identifier and underlying type
// r [type] = r -> rune

var x rune = '\n';
var y = r('\n');

func assign() bool{ 
	x = y;
	return x;
}


func main() {
}
