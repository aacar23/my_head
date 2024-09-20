# my_head
MY_HEAD (by aacar23)

My implementation of unix head utility:

This utility in a default sense prints 10 lines of the given file(s)
and prints the header if arguments include more than one file.
Although with command line options this behavior can be changed.

For these options it is mandatory to give an argument.

-n / --lines : Print lines of a given count.
-c / --bytes : Print characters , or bytes as it is also called, of a given count.


These command line options must be used without any argument:

-v / --verbose : Print header of the file no matter what.

-q / --quiet / --silent : Don't print header for any of the files.

-z / --zero-terminated : Handle null character as new line and vice versa.


These command line options also get used without an argument but they terminate the program
and only display the additional information about the program.

--help : Displays help document.
--version : Displays version number.

Note: This document also serves as the help text used in the utility.
