
## Usage

```cpp
#include "costring.h"

void main() {
 costring strName;
 costring strName = "123";
 costring strName = otherCostring; // copy costring
 strName -= 3; // pop back 3 times
 strName += "123"; // append string to back
 const char* c_str = strName.c_str(); // for C strings
 strName *= 10; // multiply by 10 times
 strName == "1488"; // equality
 strName != "1488"; // inequality 
}
```

## Functions
```cpp
cst::to_int(costring&)   // cast costring to int
cst::to_float(costring&) // cast costring to float
cst::to_costring(int)    // cast int to costring
```

## Methods
```cpp
UL == unsigned long

- size() // returns size with '\0'

-- capacity() // returns capacity with '\0'

- c_str() // return const char* of string

-- clear() // resizing costring to "\0"

- reserve(UL len) // increases capacity by len

-- append(const char* other) // push other to back

- at(UL index) // safe char getter

-- begin() // returns start iterator 

- end() // returns end iterator

-- copy() // returns copy of costring

- compare(const char* other) // returns equaling of strings

-- compare(const char* other, UL pos, UL size) // returns equaling of substr and other

- data() // returns pointer to string

-- empty() // returns 1 if string == '\0'

- resize(UL newSize) // resizing string, cuts excess

-- front() // first char&

- back() // last char&

-- push_back(char c) // append c to back

- pop_back() // pop last element

-- to_lower() // makes string to lowercase

- to_upper() // makes string to uppercase

-- substr(UL pos, UL size) // returns new costring with substring from pos to pos+size

- find(const char* pattern) // returns index of pattern in string (-1 if not exists)

-- findAll(const char* pattern) // returns vector of indexes of pattern in string (vector.size() == 0 if not exists)

- replace(const char* a, const char* b) // replaces first a-pattern to b-pattern

-- replaceAll(const char* a, const char* b) // replaces all a-patterns to b-pattern

- reverse() // reverses string

-- starts_with(const char* pattern) // returns pattern == string start

- ends_with(const char* pattern) // returns pattern == string end

-- insert(UL position, const char* str) // inserts str in position

- erase(UL position, UL size) // erases substr from position to position+size

-- trim(char symbol = ' ') // cuts symbols on the sides of string (default ' ')

- ltrim(char symbol = ' ') // cuts symbols on the left side of string (default ' ')

-- rtrim(char symbol = ' ') // cuts symbols on the right side of string (default ' ')

- swap(costring& other) // swap costrings
```
