//~ -1
//~ -2
//~ -3
//~ 1
//~ 2
//~ 3

package main

func main() {

	var a = 1;
	var b = 2;
	var c = 3;

	if (true) {
		var a = -1;
		println(a);
	}

	if (true) {
		var b = -2;
		println(b);
	}

	if (true) {
		var c = -3;
		println(c);
	}

	println(a)
	println(b)
	print(c)
}
