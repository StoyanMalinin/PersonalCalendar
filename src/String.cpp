#include "String.h"

const size_t String::MIN_CAP = 8;

void String::deleteDynamic()
{
	delete[] data;
}

void String::cutLinkToDynamic()
{
	data = nullptr;
}

String::String()
{
	this->data = new char[MIN_CAP];
	this->data[0] = '\0';

	this->cap = MIN_CAP;
	this->len = 0;
}

String::String(const char* s)
{
	if (s == nullptr)
	{
		std::cerr << "String cannot be null" << '\n';
		return;
	}

	this->cap = MIN_CAP;
	this->len = strlen(s);
	while (checkTooSmall(this->len, this->cap) == true) this->cap *= 2;
	
	this->data = new char[this->cap];
	strcpy(this->data, s);
}

size_t String::getLen() const
{
	return len;
}

const char* String::getData() const
{
	return data;
}

void String::concat(const char* s, size_t sLen)
{
	len += sLen;
	size_t newCap = cap;
	while (checkTooSmall(len, newCap) == true) newCap *= 2;
	
	resize(newCap);
	strcat(data, s);
}

void String::concat(const String& other)
{
	concat(other.data, other.len);
}

bool String::checkTooSmall(size_t len, size_t cap)
{
	if (cap < 8) return true;
	return (len >= (cap / 4) * 3);
}

bool String::checkTooLarge(size_t len, size_t cap)
{
	return (cap >= 8 && len <= cap / 4);
}

size_t* String::getPrefixFunction(const char* s, size_t n)
{
	size_t* f = new size_t[n];
	if (n == 0) return f;

	f[0] = 0;
	size_t len = 0;
	for (size_t i = 1; i < n; i++)
	{
		while (len != 0 && s[len] != s[i]) len = f[len - 1];
		
		if (s[len] == s[i]) len++;
		f[i] = len;
	}

	return f;
}

String& String::operator+=(char c)
{
	char addition[2] = { c, '\0'};
	*this += addition;

	return *this;
}

bool String::findSubstr(String& s) const
{
	size_t* f = getPrefixFunction(s.data, s.len);
	bool res = findSubstr(s.data, s.len, f);

	delete[] f;
	return res;
}

bool String::findSubstr(const char* s) const
{
	return findSubstr(s, strlen(s));
}

bool String::findSubstr(const char* s, size_t n) const
{
	size_t* f = getPrefixFunction(s, n);
	bool res = findSubstr(s, n, f);

	delete[] f;
	return res;
}

bool String::findSubstr(const char* s, size_t n, const size_t* f) const
{
	if (n == 0) return true;

	size_t matchedLen = 0;
	for (size_t i = 0; i < len; i++)
	{
		while (matchedLen != 0 && data[i] != s[matchedLen]) matchedLen = f[matchedLen - 1];

		if (data[i] == s[matchedLen]) matchedLen++;
		if (matchedLen == n) return true;
	}

	return false;
}

bool String::findSubstr(const String& s, const size_t* f) const
{
	return findSubstr(s.data, s.len, f);
}

void String::resize(size_t newCap)
{
	if (cap == newCap) return;

	char* newData = new char[newCap];
	if (len + 1 <= newCap) strcpy(newData, data);
	else memcpy(newData, data, cap * sizeof(char));

	delete[] data;

	data = newData;
	cap = newCap;
}

void String::deepCopyFromOther(const String& other)
{
	len = other.len;
	cap = other.cap;

	data = new char[other.cap];
	strcpy(data, other.data);
}

void String::shallowCopyFromOther(const String& other)
{
	data = other.data;
	len = other.len;
	cap = other.cap;
}

String& String::operator =(const String & other)
{
	if (this == &other)
		return *this;

	deleteDynamic();
	deepCopyFromOther(other);

	return *this;
}

String& String::operator =(String&& other)
{
	if (this == &other)
		return *this;

	deleteDynamic();
	shallowCopyFromOther(other);
	other.cutLinkToDynamic();

	return *this;
}

String::String(const String& other)
{
	deepCopyFromOther(other);
}

String::String(String&& other)
{
	shallowCopyFromOther(other);
	other.cutLinkToDynamic();
}

String::~String()
{
	deleteDynamic();
}

String operator+(char c, const String& s)
{
	char addition[2] = { c, '\0' };
	return addition + s;
}

String operator+(const String& s, char c)
{
	String ans = s;
	ans += c;

	return ans;
}

void getline(std::istream& is, String& s)
{
	s = String();
	
	char c;
	while (is)
	{
		c = is.peek();
		if (c == '\n')
		{
			break;
		}

		is.get(c);
		s += c;
	}
}

std::istream& operator >>(std::istream& is, String& s)
{
	s = String();
	
	char c;
	while (is)
	{
		is.get(c);
		if (isspace(c) == 0)
		{
			s += c;
			break;
		}
	}
	while (is)
	{
		c = is.peek();
		if (isspace(c) != 0)
		{
			break;
		}

		is.get(c);
		s += c;
	}

	return is;
}

std::ostream& operator <<(std::ostream& os, const String& s)
{
	os << s.data;
	return os;
}

String& String::operator +=(const String& other)
{
	concat(other);
	return *this;
}

String operator +(const String& lhs, const String& rhs)
{
	String res = lhs;
	res += rhs;

	return res;
}

bool operator <(const String& lhs, const String& rhs)
{
	return (strcmp(lhs.data, rhs.data) < 0);
}

bool operator >(const String& lhs, const String& rhs)
{
	return (strcmp(lhs.data, rhs.data) > 0);
}

bool operator <=(const String& lhs, const String& rhs)
{
	return (strcmp(lhs.data, rhs.data) <= 0);
}

bool operator >=(const String& lhs, const String& rhs)
{
	return (strcmp(lhs.data, rhs.data) >= 0);
}

bool operator ==(const String& lhs, const String& rhs)
{
	return (strcmp(lhs.data, rhs.data) == 0);
}

bool operator !=(const String& lhs, const String& rhs)
{
	return (strcmp(lhs.data, rhs.data) != 0);
}

void String::swap(String& other)
{
	std::swap(len, other.len);
	std::swap(cap, other.cap);
	std::swap(data, other.data);
}

String String::toString(size_t num)
{
	if (num == 0) return "0";

	String ans = "";
	while (num != 0)
	{
		ans = (num % 10 + '0') + ans;	
		num /= 10;
	}

	return ans;
}

String String::format(String s, size_t len, char c, bool pos)
{
	while (s.len < len)
	{
		if (pos == false) s = c + s;
		else s += c;
	}

	return s;
}
