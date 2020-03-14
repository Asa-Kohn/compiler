// Error: (line 13) f is not assignment compatible with float64 in assign statement
package main

type f float64;
// symbol table expected output
// defined types are distinct regardless of identifier and underlying type
// f [type] = f -> float64

var x float64 = 64.64;
var y = f(64.64);

func assign() bool{ 
	x = y;
	return x;
}


func main() {
}
