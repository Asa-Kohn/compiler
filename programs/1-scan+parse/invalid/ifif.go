// Malformed if statement, unexpected if, expecting ';'

package main

func main() {
    if if true{
        println("Do something?");
    }
}
