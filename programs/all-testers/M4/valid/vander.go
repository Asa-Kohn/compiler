//~+1.000000e+000 +7.300000e+000 +5.329000e+001 +3.890170e+002 +2.839824e+003 +1.000000e+000 +4.100000e+000 +1.681000e+001 +6.892100e+001 +2.825761e+002 +1.000000e+000 +3.600000e+000 +1.296000e+001 +4.665600e+001 +1.679616e+002 +1.000000e+000 +1.900000e+000 +3.610000e+000 +6.859000e+000 +1.303210e+001 +1.000000e+000 +9.930000e+001 +9.860490e+003 +9.791467e+005 +9.722926e+007

/* Generate the n x n Vandermonde matrix
 * https://en.wikipedia.org/wiki/Vandermonde_matrix
 */

package main

func vandermonde(data [5]float64) [5][5]float64 {
	// Constructs a Vandermonde matrix from the data provided
	var V [5][5]float64
	var entry float64

	for i := 0; i < 5; i++ {
		for j := 0; j < 5; j++ {
			entry = 1.
			// Calculate the exponentiation using a loop
			for k := 0; k < i; k++ {
				entry *= data[j]
			}
			V[j][i] = entry
		}
	}

	return V
}

func detvan(V [5][5]float64) float64 {
	var det float64 = 1.

	for i := 0; i < 5; i++ {
		for j := 0; j < 5; j++ {
			if i < j {
				// \prod_{1 \leq i < j \leq n} (a_j - a_i)
				det *= (V[j][1] - V[i][1])
			}
		}
	}
	return det
}

func main() {
	var col [5]float64 // int literal due to GoLite restrictions
	col[0] = 7.3
	col[1] = 4.1
	col[2] = 3.6
	col[3] = 1.9
	col[4] = 99.3

	// populate the column that we'll be using and output while doing so
	// print("Vandermonde of data ", "(")
	// for i := 0; i < 5; i++ {
	// 	col[i] = float64(i + 1)
	// 	print(col[i])
	// 	if i < 4 {
	// 		print(" ")
	// 	}
	// }
	// print(")", " is\n\n")
	var V [5][5]float64 = vandermonde(col)
	// for i := 0; i < 5; i++ {
	//     for j := 0; j < 5; j++ {
	//         print(V[i][j], " ");
	//     }
	//     // print("\n");
	// }

	print("It has the following determinant: ", detvan(V))
}
