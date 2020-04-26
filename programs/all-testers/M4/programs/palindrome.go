//~true true true false false true 

//checks whether string is palindrome
package main

func isPalindrome(s []rune) bool {
	// both ends of a string
	var l int = 0
	var h int = len(s) - 1

	//convert into rune slice
	r := s
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
	var pal []rune
	var empty []rune
	// hannah
	pal = append(pal, 'h')
	pal = append(pal, 'a')
	pal = append(pal, 'n')
	pal = append(pal, 'n')
	pal = append(pal, 'a')
	pal = append(pal, 'h')
	print(isPalindrome(pal), " ")
	pal = empty
	// yeley
	pal = append(pal, 'y')
	pal = append(pal, 'e')
	pal = append(pal, 'l')
	pal = append(pal, 'e')
	pal = append(pal, 'y')
	print(isPalindrome(pal), " ")
	pal = empty
	// a
	pal = append(pal, 'a')
	print(isPalindrome(pal), " ")
	pal = empty
	// goofy
	pal = append(pal, 'g')
	pal = append(pal, 'o')
	pal = append(pal, 'o')
	pal = append(pal, 'f')
	pal = append(pal, 'y')
	print(isPalindrome(pal), " ")
	pal = empty
	// hello
	pal = append(pal, 'h')
	pal = append(pal, 'e')
	pal = append(pal, 'l')
	pal = append(pal, 'l')
	pal = append(pal, 'o')
	print(isPalindrome(pal), " ")
	pal = empty
	// empty
	print(isPalindrome(pal), " ")
	pal = empty
}
