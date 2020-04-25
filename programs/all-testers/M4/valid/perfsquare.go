//~false

/* Checks if a number is a perfect square using elementary properties.
 * \sum_{i=0}^n (2i + 1) = (n + 1)^2
 */

package main

func perf(num int) bool {
    // Start removing from the input
    for i := 0; num > 0; i++ {
        num -= 2  * i + 1
    }

    if(num == 0) {
        return true;
    }

    return false;
}

func main() {
    println(perf(20));
}
