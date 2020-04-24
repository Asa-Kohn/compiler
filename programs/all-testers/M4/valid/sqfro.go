//~Result:+4.200000e+003

/* Squared Frobenius matrix norm of a 5 x 5 matrix
 * https://en.wikipedia.org/wiki/Matrix_norm#Frobenius_norm
 */

package main

func abs(x float64) float64 {
    if(x < 0) {
        return -x
    }

    return x
}

func fro(A [5][8]float64) float64 {
    var norm float64 = 0.
    for i := 0; i < 5; i++ {
        for j := 0; j < 8; j++ {
            norm += abs(A[i][j]) * abs(A[i][j])
        }
    }

    return norm
}

func main() {
    var A [5][8]float64;

    for i := 0; i < 5; i++ {
        for j := 0; j < 8; j++ {
             // Putting in whatever
            A[i][j] = float64(i * j)
        }
    }

    // println("Calculating Squared Frobenius norm of:")
    // for i := 0; i < 5; i++ {
    //     for j := 0; j < 5; j++ {
    //         print(A[i][j], " ");
    //     }
    //     print("\n");
    // }
    print("Result:", fro(A))
}
