/* test 2 */
/* loops */

package main

func main() {
    for {
        println("ad infinitum!");
    }

    for !(true && false || true && !false) && (true && false || true && !false) {
        println("printed the complex expression");
    }

    var x = -10;
    for x < 10 {
        x++;
        if(x < 5) {
            print("p");
        } else {
            continue //some comment here
        }
    }
    println();

    // classic for loop

    for i:=0 ; 2*i - 1 < 100; i += 2 {
        //do something?
        if i == 0 {
            break;
        }
        a := ((((((((((1)+1))+2)))+5))+6)) // unusual parentheses nesting

        var b bool = true;
        b = !b

        a = +a
        a = -a;
    }
}
