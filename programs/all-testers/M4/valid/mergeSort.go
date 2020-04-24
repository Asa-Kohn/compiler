//~sorted: 1 3 4 5 7 8 10 13 55 99

//sorting tingz
package main

//recursive merge & sort
func mergeSort(array []int, left int, right int) {
	if left < right {

		// get middle
		// var middle int = (left + right)/2;
        var middle int = left + (right - left)/2; 

		// recursively call left and right
		mergeSort(array, left, middle);
		mergeSort(array, middle+1, right);

		// merge
		merge(array, left, middle, right);
	}
}

// merging 2 slices left-middle and middle-right
func merge(array []int, left int, middle int, right int) {
	//temp slices
	var lefta []int;
	var righta []int;


	var leftlen int = middle - left + 1;
	var rightlen int = right - middle;

	//init temp slices
	for i := 0; i < leftlen; i++ {
		lefta = append(lefta, array[left + i]);
	}
	for i := 0; i < rightlen; i++ {
		righta = append(righta, array[middle + i + 1]);
	}

	//merging temp slices to main array in order
	i := 0; //for left
	j := 0; //for right
	k := left; //for main 

	for i < len(lefta) && j < len(righta) {
		if lefta[i] <= righta[j] {
			array[k] = lefta[i];
			i++;
		} else {
			array[k] = righta[j];
			j++;
		}
		k++;
	}

	//for remaining elements left & right
	for i < len(lefta) {
		array[k] = lefta[i];
		i++;
		k++;
	}
	for i < len(righta) {
		array[k] = righta[j];
		j++;
		k++;
	}
}

func main() {
	//slice type using append
	var array []int;
	array = append(array, 7);
	array = append(array, 4);
	array = append(array, 3);
	array = append(array, 1);
	array = append(array, 99);
	array = append(array, 13);
	array = append(array, 5);
	array = append(array, 8);
	array = append(array, 10);
	array = append(array, 55);
	//array type using indexing
	// var array [6]int;
	// array[0] = 7;
	// array[1] = 4;
	// array[2] = 3;
	// array[3] = 1;
	// array[4] = 99;
	// array[5] = 13;

	//make array here, array Literal, not supported in goLite
	// array := [10]int{7, 3, 4, 1, 99, 13, 5, 8, 10, 55};

	//printing without fmt import
	// print("unsorted: ");
	// for i := 0; i < len(array); i++ {
	// 	print(array[i], " ");
	// }
	// println("\r");

	mergeSort(array, 0, len(array)-1);

	print("sorted: ");
	for i := 0; i < len(array); i++ {
		print(array[i], " ");
	}
	// println("\r");
}
