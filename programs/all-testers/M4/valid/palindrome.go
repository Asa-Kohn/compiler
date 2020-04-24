//~true true true false false false true 

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
	print(isPalindrome("hannah"), " ")
	print(isPalindrome("yeley"), " ")
	print(isPalindrome("a"), " ")
	print(isPalindrome("goofy"), " ")
	print(isPalindrome("hello"), " ")
	print(isPalindrome("honey"), " ")
	print(isPalindrome("hannah"), " ")
}
