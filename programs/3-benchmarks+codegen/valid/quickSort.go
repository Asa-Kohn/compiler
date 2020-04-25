//~sorted: 1 3 4 5 7 8 10 13 55 99 

package main

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

func quickSort(array []int, lo int, hi int) {
	if lo < hi {
		p := partition(array, lo, hi);
		quickSort(array, lo, p-1);
		quickSort(array, p+1, hi);
	}
}

func main() {
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
	quickSort(array, 0, len(array)-1);

	print("sorted: ");
	for i := 0; i < len(array); i++ {
		print(array[i], " ");
	}
	print("\n");
}
