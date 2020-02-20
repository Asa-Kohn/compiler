/* Generate the n x n Vandermonde matrix
 * https://en.wikipedia.org/wiki/Vandermonde_matrix
 */

package main

func vandermonde(data [5]float64) [5][5]float64 {
    // Constructs a Vandermonde matrix from the data provided
    var V [5][5]float64;
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

func main() {
    var col [5]float64 // int literal due to GoLite restrictions

    // populate the column that we'll be using and output while doing so
    print("Vandermonde of data ", "(")
    for i := 0; i < 5; i++ {
        col[i] = float64(i + 1)
        print(col[i])
        if(i < 4) {
	    print(" ")
	}
    }
    print(")", " is\n\n")
    var V [5][5]float64 = vandermonde(col);
    for i := 0; i < 5; i++ {
        for j := 0; j < 5; j++ {
            print(V[i][j], " ");
        }
        print("\n");
    }
}
