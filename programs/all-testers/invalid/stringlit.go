//print double quote(legal only in string literals), unknown escape sequence
package main

import "fmt"

func main() {
	fmt.Print("double quote: ");
	fmt.Print('\"');
	fmt.Println();
}