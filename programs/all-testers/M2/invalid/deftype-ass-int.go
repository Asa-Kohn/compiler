// Error: (line 13) i is not assignment compatible with int in assign statement
package main

type i int;
// symbol table expected output
// defined types are distinct regardless of identifier and underlying type
// i [type] = i -> int

var x int = 0;
var y = i(0);

func assign() bool{ 
	x = y;
	return x;
}


func main() {
}
