//~recPower: 4

//recursively compute x to the power of y
package main

func recPower(x int, y int) int {
	if y == 0 {
		return 1;
	}
	return x * recPower(x, y - 1);
}

func main() {
	var x int = 2;
	var y int = 2;
	println("recPower:", recPower(x, y));
}
