// GoLite does not support for range!, unexpected range 

package main

func main() {
    var phrase string = "mia nonna disse che non avrebbe funzionato";
    for _, r := range phrase {
        print(string(r));
    }
    println("Listen to your grandmother!");
}
