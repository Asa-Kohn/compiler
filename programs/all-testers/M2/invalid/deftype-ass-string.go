// Error: (line 13) s is not assignment compatible with string in assign statement
package main

type s string;
// symbol table expected output
// defined types are distinct regardless of identifier and underlying type
// s [type] = s -> string

var x string = "word";
var y = s("word");

func assign() bool{ 
	x = y;
	return x;
}


func main() {
}
