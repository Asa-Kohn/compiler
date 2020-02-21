//checks whether string is palindrome
package main

func isPalindrome(s string) bool {
	// both ends of a string
	var l int = 0
	var h int = len(s) - 1

	//convert into rune slice
	r := []rune(s)
	// for i := 0; i < len(r) ; i++ {
	// 	print(r[i]);
	// }

	// comparing both ends
	for h > 1 {
		if r[l] != r[h] {
			// print("not palindrome");
			l++
			h--
			return false
		}
		l++
		h--
	}
	return true
}

// Driver program to test above function
func main() {
	var isPal = isPalindrome("hannah")
	println("palindrome:", isPal)
	var isPal1 = isPalindrome("yeley")
	println("palindrome:", isPal1)
	var isPal2 = isPalindrome("a")
	println("palindrome:", isPal2)
	var isPal3 = isPalindrome("baaaab")
	println("palindrome:", isPal3)
	var isPal4 = isPalindrome("goofy")
	println("palindrome:", isPal4)
	var isPal5 = isPalindrome("hello")
	println("palindrome:", isPal5)
	var isPal6 = isPalindrome("honey")
	println("palindrome:", isPal6)
}
