//operator + not defined on bool
package main

func boolfunc() bool{ return true;}
var oo = boolfunc() + boolfunc();
