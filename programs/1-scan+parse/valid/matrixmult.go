/* Computes the dot product of two vectors provided, uses this to calculate
 * multiplication of two matrices. This is done very inefficiently.
 * https://en.wikipedia.org/wiki/Dot_product
 */

package main

type point struct {
    x, y, z int
}

func dot(u point, v point) int {
    return u.x * v.x + u.y * v.y + u.z * v.z
}

func matmul(A, B [3][3]int) [3][3]int {
    // Computes A * B

    var col point;
    var row point;

    var C [3][3]int;

    for i := 0; i < 3; i++ {
        for j := 0; j < 3; j++ {
            row.x = A[i][0];
            row.y = A[i][1];
            row.z = A[i][2];

            col.x = B[0][j];
            col.y = B[1][j];
            col.z = B[2][j];

            C[i][j] = dot(row, col); // although it doesn't matter as it commutes
        }
    }
    return C
}

func main() {

    // populate the matrices that we'll be using
    var A [3][3]int;
    var B [3][3]int;

    // A will be the identity for now
    A[0][0] = 1;
    A[0][1] = 0;
    A[0][2] = 0;

    A[1][0] = 0;
    A[1][1] = 1;
    A[1][2] = 0;

    A[2][0] = 0;
    A[2][1] = 0;
    A[2][2] = 1;

    // Change at will
    B[0][0] = 1;
    B[0][1] = 1;
    B[0][2] = 1;

    B[1][0] = 1;
    B[1][1] = 1;
    B[1][2] = 1;

    B[2][0] = 1;
    B[2][1] = 1;
    B[2][2] = 1;

    var C [3][3]int = matmul(A, B);

    // Output
    println("Result")
    for i := 0; i < 3; i++ {
        for j := 0; j < 3; j++ {
            print(C[i][j], " ");
        }
        print("\n");
    }
}
