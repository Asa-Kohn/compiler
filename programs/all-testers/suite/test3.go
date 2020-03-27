/* test 3 */
/* indexing  + appending*/

package main

func main() {
    var arr []int;

    a := 123123;

    arr.append(0);
    arr.append(1);
    arr.append(2);
    arr.append(3);
    arr.append(4);
    arr.append("what");
    arr.append("no?");
    arr.append("why?");
    arr.append(true);
    arr.append(false);
    arr.append(a);
    arr.append(-234234);
    arr.append('b');

    // slicing

    var somevalue := arr[2:3];
    var other int = arr[2:3];

    cap(somevalue)
    len(somevalue)

    cap(other)
    len(other)
}
