//
package main

type num int
type struc struct{ a, b int; }
type confusion struc;

var x []int
var y = num(0)

var a []struc
var aa struc
var b = num(2)

var confused confusion;

var c int
var d float64
var e bool
var f rune
var g string

func function() int {
	{
		{
			{
				{
					return 0;
				}
			}
		}
	}
}


func main() {
	function();
}
