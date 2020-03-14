/* test 6 */
/* native types with expressions */

package main

func main() {
    // decimal
    var a int = 0;
    var aa int = 2394820;
    var aaa int = -2834728;

    var aaaa int = a + aa + aaa * a - aa  / aaa

    //octal
    a = 0444;
    aa = 0666;
    aaa= -0888;

    aaaa = a + aa + aaa * a - aa  / aaa

    //hex
    a = 0xff;
    aa = 0x00;
    aaa= -0xff;

    aaaa = a + aa + aaa * a - aa  / aaa


    // bools

    var b bool = true;
    var c bool = false
    var d bool = c && b || c && !c


    // strings

    var str string = "slkjs";
    var str1 string = str;


    // runes and their literals
    var sjdf rune = '3'
    var sdfsdf rune = 'k'
    var sdkfjsf  rune = '\a'
    sdkfjsf = '\a'
    sdkfjsf = '\b'
    sdkfjsf = '\f'
    sdkfjsf = '\n'
    sdkfjsf = '\r'
    sdkfjsf = '\t'
    sdkfjsf = '\v'
    sdkfjsf = '\\'
    sdkfjsf = '\''

    // floats

    var asdjksdfsdf float = 0.1;
    asdjksdfsdf = 0.1
    asdjksdfsdf = .1
    asdjksdfsdf = 1.100
    asdjksdfsdf = 1.
    asdjksdfsdf = 823834.

    asdjksdfsdf = -0.1
    asdjksdfsdf = -.1
    asdjksdfsdf = -1.100
    asdjksdfsdf = -1.
    asdjksdfsdf = -823834.

}
