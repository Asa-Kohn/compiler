// Operator &&= does not exist, unexpected and
package main

func main() {
    var b = true;
    var c = false;
    var d &&= c && b;
}
