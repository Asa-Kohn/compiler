//sorting tingz
package main

func quickSort(array []int, lo int, hi int) {
	if lo < hi {
		p := partition(array, lo, hi);
		quickSort(array, lo, p-1);
		quickSort(array, p+1, hi);
	}
}

func partition(array []int, lo int, hi int) int {
	pivot := array[hi];
	i := lo;
	for j := lo; j <= hi; j++ {
		if array[j] < pivot {
			temp := array[i];
			array[i] = array[j];
			array[j] = temp;
			i++;
		}

	}
	temp := array[i];
	array[i] = array[hi];
	array[hi] = temp;
	return i;
}

func main() {
	// var array [6]int;
	// array = append(array, 7);
	// array = append(array, 4);
	// array = append(array, 3);
	// array = append(array, 1);
	// array = append(array, 99);
	// array = append(array, 13);
	// array[0] = 7;
	// array[1] = 4;
	// array[2] = 3;
	// array[3] = 1;
	// array[4] = 99;
	// array[5] = 13;

	//make array here
	array := [10]int{7, 3, 4, 1, 99, 13, 5, 8, 10, 55};

	//printing without fmt import
	print("unsorted: ");
	for i := 0; i < len(array); i++ {
		print(array[i], " ");
	}
	println("\r");
	quickSort(array[:], 0, len(array)-1);

	print("sorted: ");
	for i := 0; i < len(array); i++ {
		print(array[i], " ");
	}
	println("\r");
}
