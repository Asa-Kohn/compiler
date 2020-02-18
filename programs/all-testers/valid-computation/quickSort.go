//sorting tingz
package main

import "fmt"
func main() {
	var unsorted [6]int;
	// unsorted = append(unsorted, 7);
	// unsorted = append(unsorted, 4);
	// unsorted = append(unsorted, 3);
	// unsorted = append(unsorted, 1);
	// unsorted = append(unsorted, 99);
	// unsorted = append(unsorted, 13);
	unsorted[0] = 7;
	unsorted[1] = 4;
	unsorted[2] = 3;
	unsorted[3] = 1;
	unsorted[4] = 99;
	unsorted[5] = 13;
	// unsorted := [6]int{7, 3, 4, 1, 99, 13}
	fmt.Println("unsorted:", unsorted);
	quickSort(unsorted[:], 0, len(unsorted)-1);
	fmt.Println("sorted:", unsorted);
}

func quickSort(unsorted []int, lo int, hi int) {
	for lo < hi {
		p := partition(unsorted, lo, hi);
		quickSort(unsorted, lo, p-1);
		quickSort(unsorted, p+1, hi);
	}
}

func partition(unsorted []int, lo int, hi int) int {
	pivot := unsorted[hi];
	i := lo;
	for j := lo; j <= hi; j++ {
		for unsorted[j] < pivot {
			temp := unsorted[i];
			unsorted[i] = unsorted[j];
			unsorted[j] = temp;
			i++;
		}

	}
	temp := unsorted[i]
	unsorted[i] = unsorted[hi];
	unsorted[hi] = temp;
	return i;
}