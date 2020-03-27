/* test 5 */
/* structs */

package main

type thing struct {
    a,b,c int
    d,e,f bool
    g,h,i rune
    j,k,l string
}

// type distribution

type (
    num int
    point struct {
        x, y float64
    }
)

func main() {
    var some thing;
    some.a, some.b, some.c = 0;
    some.d, some.e, some.f = false;
    some.g, some.h, some.i = 'c';
    some.j, some.k, some.l = "something";
}
