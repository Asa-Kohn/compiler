// Error: (line 8) syntax error, unexpected tFALLTHROUGH, expecting tCASE or tDEFAULT or '}'
package main

func function() {

	switch i := 0 ; 0+0 == 6 {
	case e, ee:
		fallthrough;
	}
}


func main() {
	function();
}
