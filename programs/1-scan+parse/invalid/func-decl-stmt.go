// Cannot declare a function as a statement, unexpected function dec in main

package main

func main() {
    var a int = 42;
    {
        var me string;
        func whoami() string {
            return "Tyler Durden";
        }
        me = whoami();
    }
}
