#pragma once
#include <vector>
#include <iostream>

class costring {
	char* _strPtr = nullptr;
	unsigned long _capacity = 1;
	unsigned long _size = 1;

	void addlen(unsigned long len) {
		_capacity += len;
		char* _newPtr = new char[_capacity]{};
		if (!_newPtr) return;
		memcpy((void*)_newPtr, _strPtr, _size-1);
		delete[] _strPtr;
		_strPtr = _newPtr;
		_strPtr[_size - 1] = '\0';
	}
	void setStr(const char* other, unsigned long size) {
		if (_strPtr) delete[]_strPtr;
		_strPtr = new char[size + 1];
		memcpy(_strPtr, other, size);
		_strPtr[size] = '\0';
		_size = size + 1;
		_capacity = _size;
	}
	void setStr(const char* other) {
		unsigned long size = strlen(other);
		setStr(other, size);
	}
	void append(char other) {
		if (_size + 1 > _capacity) {
			addlen(1);
		}
		_size += 1;
		_strPtr[_size - 2] = other;
		_strPtr[_size - 1] = '\0';
	}
	unsigned long* createPI(const char* pattern, unsigned long len) {
		unsigned long* piArr = new unsigned long[len] {};
		if (!piArr) return nullptr;

		for (int i = 1, j = 0; i < len; i++) {
			while (j > 0 and pattern[i] != pattern[j]) {
				j = piArr[j - 1];
			}

			if (pattern[i] == pattern[j]) {
				j++;
			}

			piArr[i] = j;
		}

		return piArr;
	}
public:
	inline unsigned long size() { return _size; }
	inline unsigned long capacity() { return _capacity; }
	inline const char* c_str() { return _strPtr; }
	inline void clear() { delete[] _strPtr; _capacity = 1; _size = 1; _strPtr = new char[1]; _strPtr[0] = '\0'; }
	void reserve(unsigned long len) { 
		if (len == 0) return;
		addlen(len);
	}
	void append(const char* other) {
		unsigned long len = strlen(other);
		if (_size + len > _capacity) {
			addlen(len);
		}
		_size += len;
		memcpy((void*)(_strPtr+_size-len-1), other, len);
		_strPtr[_size-1] = '\0';
	}
	inline char at(unsigned long index) { return (index < _size) ? _strPtr[index] : '\0'; }
	inline char* begin() { return &_strPtr[0]; }
	inline char* end() { return &_strPtr[_size]; }
	inline costring copy() { return costring{_strPtr}; }
	inline bool compare(const char* other) { return strcmp(_strPtr, other) == 0; }
	inline bool compare(const char* other, unsigned long position, unsigned long size) {  return (strlen(other) >= size and position+size <= _size ? !memcmp(other, _strPtr + position, size) : 0); }
	inline char* data() { return _strPtr; }
	inline bool empty() { return (_size == 1) ? true : false; }
	void resize(unsigned long newSize) { setStr(_strPtr, newSize-1); }
	inline char& front() { return _strPtr[0]; }
	inline char& back() { return _strPtr[_size-2]; } // \0 not included 
	void push_back(char c) { append(c); }
	void pop_back() { if (_size > 1) _size--; _strPtr[_size - 1] = '\0'; }
	void to_lower() { for (int i = 0; i < _size - 1; i++) _strPtr[i] = tolower(_strPtr[i]); }
	void to_upper() { for (int i = 0; i < _size - 1; i++) _strPtr[i] = toupper(_strPtr[i]); }

	const costring substr(unsigned long position, unsigned long size) {
		if (position >= _size - 1 or size == 0 or position + size > _size - 1) return { "" };
		costring subStr;
		subStr.setStr(_strPtr+position, size);
		return subStr;
	}

	long find(const char* pattern) {
		unsigned long n = strlen(pattern);
		if (n < 1) return -1;
		unsigned long* piArr = createPI(pattern, n);
		
		int j = 0;

		for (int i = 0; i < _size - 1; i++) {
			while (j > 0 and _strPtr[i] != pattern[j]) {
				j = piArr[j - 1];
			}

			if (_strPtr[i] == pattern[j]) {
				j++;
			}

			if (j == n) {
				delete[] piArr;
				return i - n + 1;
			}
		}

		delete[] piArr;

		return -1;
	}

	std::vector<unsigned long> findAll(const char* pattern) {
		unsigned long n = strlen(pattern);
		if (n < 1) return {};
		unsigned long* piArr = createPI(pattern, n);
		unsigned long maxMatches = _size / n;
		std::vector<unsigned long> matches(maxMatches, 0);
		unsigned long count = 0;

		int j = 0;

		for (int i = 0; i < _size-1; i++) {
			while (j > 0 and _strPtr[i] != pattern[j]) {
				j = piArr[j - 1];
			}

			if (_strPtr[i] == pattern[j]) {
				j++;
			}

			if (j == n) {
				matches[count++] = i - n + 1;
				j = piArr[j - 1];
			}
		}

		delete[] piArr;
		matches.resize(count);
		return matches;
	}

	void replace(const char* a, const char* b) {
		unsigned long lenA = strlen(a);
		unsigned long lenB = strlen(b);
		if (lenA < 1 or lenB < 1) return;
		long pos = find(a);
		if (pos == -1) return;
		char* newStr = new char[_size - lenA + lenB] {};
		newStr[_size - lenA + lenB-1] = '\0';

		memcpy(newStr, _strPtr, pos);
		memcpy(newStr+pos, b, lenB);
		memcpy(newStr + pos + lenB, _strPtr + pos + lenA, _size - pos - lenA);

		delete[] _strPtr;
		_strPtr = newStr;
		_size = _size - lenA + lenB;
		_capacity = _size - lenA + lenB;
	}

	void replaceAll(const char* a, const char* b) {
		unsigned long lenA = strlen(a);
		unsigned long lenB = strlen(b);
		if (lenA < 1 or lenB < 1) return;
		std::vector<unsigned long> allPos = findAll(a);
		if (!allPos.size()) return;
		unsigned long newSize = _size - allPos.size() * lenA + allPos.size() * lenB;
		char* newStr = new char[newSize] {};
		if (!newStr) return;
		newStr[newSize - 1] = '\0';

		unsigned long srcIndex = 0;
		unsigned long dstIndex = 0;

		for (unsigned long pos : allPos) {

			while (srcIndex < pos) {
				newStr[dstIndex++] = _strPtr[srcIndex++];
			}

			memcpy(newStr + dstIndex, b, lenB);
			dstIndex += lenB;
			srcIndex += lenA;
		}

		while (srcIndex < _size - 1) {
			newStr[dstIndex++] = _strPtr[srcIndex++];
		}

		delete[] _strPtr;
		_strPtr = newStr;
		_size = newSize;
		_capacity = newSize;
	}
	
	void reverse() {
		for (int i = 0; i < (_size - 1) / 2; i++) {
			char temp = _strPtr[i];
			_strPtr[i] = _strPtr[_size - i - 2];
			_strPtr[_size - 2 - i] = temp;
		}
	}

	bool starts_with(const char* pattern) {
		unsigned long len = strlen(pattern);
		if (len > _size - 1) return false;
		for (int i = 0; i < len; i++) {
			if (pattern[i] != _strPtr[i]) {
				return false;
			}
		}
		return true;
	}

	bool ends_with(const char* pattern) {
		unsigned long len = strlen(pattern);
		if (len > _size - 1) return false;
		int index = 0;
		for (int i = _size-1-len; i < _size; i++) {
			if (pattern[index++] != _strPtr[i]) {
				return false;
			}
		}
		return true;
	}

	void insert(unsigned long position, const char* str) {
		unsigned long len = strlen(str);
		if (len < 1 or position < 0 or position > _size - 1) return;
		unsigned long index = 0;
		
		if (_size + len > _capacity) {
			char* newStr = new char[_size + len];
			if (position > 0) { memcpy(newStr + index, _strPtr, position); index += position; }
			memcpy(newStr + index, str, len);
			index += len;
			if (len + position < _size + len) memcpy(newStr + index, _strPtr + position, _size - position);

			delete[] _strPtr;
			_strPtr = newStr;
			_size += len;
			_capacity = _size;
		} else {
			_size += len;
			memmove(_strPtr+position+len, _strPtr+position, _size-1-len-position);
			memmove(_strPtr + position, str, len);
		}
	}

	void erase(unsigned long position, unsigned long size) {
		if (position < 0 or position > _size - 1 or position + size > _size - 1 or size < 1) return;
		memmove(_strPtr + position, _strPtr + position + size, _size - 1 - position - size);
		_size -= size;
		memset(_strPtr+_size-1, '\0', size);
	}

	void trim(char symbol = ' ') {
		int indexLeft = 0;
		while (indexLeft < _size - 1) {
			if (_strPtr[indexLeft] != symbol) break;
			indexLeft++;
		}
		
		int indexRight = _size-2;
		while (indexRight > 0) {
			if (_strPtr[indexRight] != symbol) break;
			indexRight--;
		}

		indexRight = _size - 2 - indexRight;

		memmove(_strPtr, _strPtr + indexLeft, _size-1-indexRight);
		memset(_strPtr + _size - 1 - indexLeft - indexRight, '\0', indexLeft + indexRight);
		_size -= indexLeft; _size -= indexRight;
	}

	void ltrim(char symbol = ' ') {
		int indexLeft = 0;
		while (indexLeft < _size - 1) {
			if (_strPtr[indexLeft] != symbol) break;
			indexLeft++;
		}

		memmove(_strPtr, _strPtr + indexLeft, _size - 1);
		memset(_strPtr + _size - 1 - indexLeft, '\0', indexLeft);
		_size -= indexLeft;
	}

	void rtrim(char symbol = ' ') {
		int indexRight = _size - 2;
		while (indexRight > 0) {
			if (_strPtr[indexRight] != symbol) break;
			indexRight--;
		}

		indexRight = _size - 2 - indexRight;

		memmove(_strPtr, _strPtr, _size - 1 - indexRight);
		memset(_strPtr + _size - 1 - indexRight, '\0', indexRight);
		_size -= indexRight;
	}

	void swap(costring& other) {
		std::swap(_strPtr, other._strPtr);
		std::swap(_size, other._size);
		std::swap(_capacity, other._capacity);
	}

	void operator=(const char* other) { setStr(other); }
	void operator+=(const char* other) { append(other); }
	void operator-=(unsigned long len) {
		if (len == 0) return;
		if (_size - len < 1) len = _size - 1;
		char* toBack = new char[_size - len] {};
		if (!toBack) return;
		memcpy((void*)toBack, _strPtr, _size - 1 - len);
		toBack[_size - len - 1] = '\0';
		delete[] _strPtr;
		_strPtr = toBack;
		_size -= len;
	}
	void operator*=(unsigned long multiplier) {
		if (multiplier == 1) return;
		if (multiplier == 0) { clear(); return; }
		char* newPtr = new char[(_size - 1) * multiplier + 1] {};
		if (!newPtr) return;
		for (int i = 0; i < multiplier; i++) {
			memcpy(&newPtr[i * (_size - 1)], _strPtr, _size - 1);
		}
		newPtr[(_size - 1) * multiplier] = '\0';
		delete[] _strPtr;
		_strPtr = newPtr;
		_size = (_size - 1) * multiplier + 1;
		_capacity = _size;
	}

	costring(const costring& other) {
		_size = other._size;
		_capacity = other._capacity;
		_strPtr = new char[_capacity] {};
		if (!_strPtr or !other._strPtr) return;
		memcpy(_strPtr, other._strPtr, _size);
	}
	costring(costring&& other) noexcept {
		_strPtr = other._strPtr;
		_size = other._size;
		_capacity = other._capacity;

		other._strPtr = nullptr;
		other._size = 0;
		other._capacity = 0;
	}
	const costring& operator=(const costring& other) {
		if (this != &other) {
			delete[] _strPtr;
			_size = other._size;
			_capacity = other._capacity;
			_strPtr = new char[_capacity] {};
			if (!_strPtr or !other._strPtr) return *this;
			memcpy(_strPtr, other._strPtr, _size);
		}
		return *this;
	}
	const costring& operator=(costring&& other) noexcept {
		if (this != &other) {
			delete[] _strPtr;
			_strPtr = other._strPtr;
			_size = other._size;
			_capacity = other._capacity;

			other._strPtr = new char[1] {'\0'};
			other._size = 1;
			other._capacity = 1;
		}
		return *this;
	}
	bool operator==(const char* other) { return strcmp(_strPtr, other) == 0; }
	bool operator!=(const char* other) { return strcmp(_strPtr, other) != 0; }
	costring(const char* other) { setStr(other); }
	costring() : _strPtr(new char[1]{'\0'}), _size(1), _capacity(1) { }
	~costring() { delete[] _strPtr; _strPtr = nullptr; }
};

std::ostream& operator<<(std::ostream& os, costring& m) {
	return os << m.c_str();
}

std::istream& operator>>(std::istream& is, costring& m) {
	char buf[1024];
	is >> buf;
	m = buf;
	return is;
}

namespace cst {
	int to_int(costring& str) {
		const char* z = str.c_str();
		bool negative = z[0] == '-';
		int s = 0;
		int index = negative;
		while (index < 11) {
			if (z[index] < '0' or z[index] > '9') break;
			s *= 10;
			s += (z[index] - '0');
			index++;
		}
		if (negative) s *= -1;
		return s;
	}
	float to_float(costring& str) {
		const char* z = str.c_str();
		bool negative = z[0] == '-';
		float s = 0;
		int index = negative;
		while (index < 11) {
			if (z[index] < '0' or z[index] > '9') { index++; break; }
			s *= 10;
			s += (z[index] - '0');
			index++;
		}
		int count = 1;
		while (true) {
			if (z[index] < '0' or z[index] > '9') break;
			s += (z[index] - '0') / (pow(10, count++));
			index++;
		}
		if (negative) s *= -1;
		return s;
	}
	costring to_costring(int a) {
		char chars[12]{};
		chars[11] = '\0';

		if (a != 0) {
			int len = 0;
			int num = a; if (a < 0) num = -num;
			int c = (a < 0) ? 1 : 0;
			while (num >= 1) { len++; num /= 10; }
			num = a; if (a < 0) num = -num;
			for (int i = len - 1 + c; i >= 0 + c; i--) {
				chars[i] = '0' + num % 10;
				num /= 10;
			}
			if (a < 0) chars[0] = '-';
			memset(chars + len + c, '\0', 11 - len - c);
		} else {
			chars[0] = '0';
			memset(chars+ 1, '\0', 12 - 1);
		}

		costring str = chars;
		return str;
	}
}