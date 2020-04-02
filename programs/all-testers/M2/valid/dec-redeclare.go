// 2.2 Adds the type mapping T1 -> def(T2) to the type symbol table (i.e., T1 is a defined type
// 	inheriting from T2). If T1 is already declared in the current scope, an error is raised. If
// 	T1 is already declared, but in an outer scope, the new T1 -> def(T2) type mapping will
// 	shadow the previous mapping.
package main

type b bool;

var x bool = false;
var y = b(false);

func assign(){ 
	var x int;	// redeclaration of x in a different scope. shadowing the outer x
}


func main() {
}
