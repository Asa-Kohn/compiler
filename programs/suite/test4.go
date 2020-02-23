/* test 4 */
/* printing */

package main

func main() {

    print();
    println();

    print("word");
    print(1);
    print('k');
    print(true);

    println("word");
    println(1);
    println('k');
    println(true);


    // more than one
    print("word", 'r', 1, true, false);
    print(1, 'r', 1, true, false);
    print('k', 'r', 1, true, false);
    print(true, 'r', 1, true, false);

    println("word", 'r', 1, true, false);
    println(1, 'r', 1, true, false);
    println('k', 'r', 1, true, false);
    println(true, 'r', 1, true, false);

    /* error if you want to try it

    print("word", 'r', 1, true, false,);
    print(1, 'r', 1, true, false,);
    print('k', 'r', 1, true, false,);
    print(true, 'r', 1, true, false,);

    println("word", 'r', 1, true, false,);
    println(1, 'r', 1, true, false,);
    println('k', 'r', 1, true, false,);
    println(true, 'r', 1, true, false,);

    */

}
