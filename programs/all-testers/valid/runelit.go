//print all runes
package main

import "fmt"

func main() {
	fmt.Print("alert: ");
	fmt.Println('\a');

	fmt.Print("backspace: ");
	fmt.Println('\b');

	fmt.Print("form feed: ");
	fmt.Println('\f');

	fmt.Print("line feed or newline: ");
	fmt.Println('\n');

	fmt.Print("carriage return: ");
	fmt.Println('\r');

	fmt.Print("horizontal tab: ");
	fmt.Println('\t');

	fmt.Print("vertical tab: ");
	fmt.Println('\v');

	fmt.Print("backslash: ");
	fmt.Println('\\');

	fmt.Print("single quote: ");
	fmt.Println('\'');

	// fmt.Print("double quote(legal only in string literals): ");
	// fmt.Println('\');
}