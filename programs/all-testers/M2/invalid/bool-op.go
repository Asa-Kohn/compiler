// (line 5) incomatible type in arithmetic op + [received bool, expected numeric (int, rune, float64) or string]
// operator + not defined on bool
package main

func boolfunc() bool{ return true;}
var oo = boolfunc() + boolfunc();
