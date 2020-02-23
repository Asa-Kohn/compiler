/* test 1 */
/* tests a lot of if nesting and the like */

package main

var b bool = true;

// simple statement
func main() {
    // if else chain with simple statement
    if c := !b; c && b {
        println("branched to: c && b");
    } else if c := b; c {
        println("branched to: b");
    } else if c:= b; !c {
        println("branched to: !c");
    } else {
        println("branched to: else d");
    }


    // nested ifs
    if c := !b; c && b {
        println("branched to: c && b");
        if c := !b; c && b {
            println("branched to: c && b");
            if c := !b; c && b {
                println("branched to: c && b");
            } else if c := b; c {
                println("branched to: b");
            } else if c:= b; !c {
                println("branched to: !c");
            } else {
                println("branched to: else d");
            }
        } else if c := b; c {
            println("branched to: b");
        } else if c:= b; !c {
            println("branched to: !c");
        } else {
            println("branched to: else d");
        }
    } else if c := b; c {
        println("branched to: b");
        if c := !b; c && b {
            println("branched to: c && b");
        } else if c := b; c {
            println("branched to: b");
        } else if c:= b; !c {
            println("branched to: !c");
        } else {
            println("branched to: else d");
        }
    } else if c:= b; !c {
        println("branched to: !c");
        if c := !b; c && b {
            println("branched to: c && b");
        } else if c := b; c {
            println("branched to: b");
        } else if c:= b; !c {
            println("branched to: !c");
        } else {
            println("branched to: else d");
        }
    } else {
        println("branched to: else d");
        if c := !b; c && b {
            println("branched to: c && b");
        } else if c := b; c {
            println("branched to: b");
        } else if c:= b; !c {
            println("branched to: !c");
        } else {
            println("branched to: else d");
        }
    }
}
