//!

/* Simple Caesar cipher implementation. Do not use this for sensitive data!
 * https://en.wikipedia.org/wiki/Caesar_cipher
 */

package main

func caesar(shift byte, message string, mlength int) {
    // Iterate over the runes and change them according to shift
    // Shift can be any int, we will simply find its equivalent class mod 26
    shift %= 26;
    print("Scrambled message: ");
    for i := 0; i < mlength; i++ {
        print(string(shift_rune(shift, message[i])));
    }
    // print("\n");
}

func shift_rune(shift byte, r byte) rune {
    shift %= 26; // Just in case!
    // Given that we are just dealing with ASCII in GoLite
    switch {
        case 97 <= r && r <= 122:
            r = (r + shift - 19) % 26 + 97 // Account for where it starts
        case 65 <= r && r <= 90:
            r = (r + shift - 13) % 26 + 65 // Bring it back again!
        default:
            _ = r
    }
    return rune(r)
}

func main() {
    var message string = "I have a marvelous proof for P = NP, although it is too large for my floppy disk.";
    var mlength int = 81 // I had to hard code it :(
    // The Caesar function will print it out as it goes, due to GoLite limitations.
    caesar(0, message, mlength);
}
