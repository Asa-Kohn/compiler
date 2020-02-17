//recursively compute x factorial
package main


func recFactorial(x int) int {
	if x == 0 {
		return 1;
	}
	return x * recFactorial(x - 1);
}

func main() {
	var x int = 10;
	println("recFactorial:", recFactorial(x));
}