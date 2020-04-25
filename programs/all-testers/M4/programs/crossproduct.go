//~Cross product: ( +1.000000e+001 +2.000000e+001 +3.000000e+001 ) x ( +5.000000e+000 -5.000000e+000 +0.000000e+000 ) is: ( +1.500000e+002 +1.500000e+002 -1.500000e+002 )

/* Computes the cross product of two vectors in R^3 (up to 64 bit precision)
 * https://en.wikipedia.org/wiki/Cross_product
 */

package main

type point struct {
    x, y, z float64
}

func crossprod(u point, v point) point{
    var c point;
    c.x = u.y * v.z - u.z * v.y;
    c.y = -(u.x * v.z - u.z * v.x);
    c.z = u.x * v.y - u.y * v.x;

    return c;
}

 func main() {
    var u point;
    u.x = 10.0;
    u.y = 20.0;
    u.z = 30.0;

    var v point;
    v.x = 5.0;
    v.y = -5.0;
    v.z = 0.0;

    // Compute the cross product and print to stdout
    var c point = crossprod(u, v);
    println("Cross product:",  "(", u.x, u.y, u.z, ") x (", v.x, v.y, v.z, ") is:", "(", c.x, c.y, c.z, ")");
 }
