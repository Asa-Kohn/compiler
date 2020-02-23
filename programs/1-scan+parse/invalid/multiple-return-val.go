// Too many return statements! GoLite allows zero or one expression as a return, unexpected ',', expecting ')'

package main

func twoistoomuch() (string, string){
    return ("no", "good");
}

func main() {
    var str1, str2 := twoistoomuch();
}
