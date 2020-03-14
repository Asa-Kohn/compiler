// Error: (line 13) b is not assignment compatible with bool in assign statement
package main

type b bool;
// symbol table expected output
// defined types are distinct regardless of identifier and underlying type
// b [type] = b -> bool

var x bool = false;
var y = b(false);

func assign() bool{ 
	x = y;
	return x;
}


func main() {
}
