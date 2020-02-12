//print numerical lits
package main

import "fmt"

func main() {
	//INT LITS
	//decimal
	fmt.Print(255);
	fmt.Print(" : ");
	fmt.Println(255);
	//octal
	fmt.Print(0377);
	fmt.Print(" : ");
	fmt.Println(0377);
	//hex
	fmt.Print(0xff);
	fmt.Print(" : ");
	fmt.Println(0xff);

	//FLOAT LITS
	//int+dec
	fmt.Print(0.12);
	fmt.Print(" : ");
	fmt.Println(0.12);
	//dec only
	fmt.Print(.12);
	fmt.Print(" : ");
	fmt.Println(.12);
	//int only
	fmt.Print(12.);
	fmt.Print(" : ");
	fmt.Println(12.);
}