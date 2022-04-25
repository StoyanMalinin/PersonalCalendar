#ifndef __STRING_H
#define __STRING_H

#include <iostream>

class String
{
private:
	char* data;
	size_t len, cap;

private:
	static const size_t MIN_CAP;

private:
	void deleteDynamic();
	void cutLinkToDynamic();
	void deepCopyFromOther(const String& other);
	void shalllowCopyFromOther(const String& other);

public:
	String();
	String(const char* s);
	String& operator =(const String& other);
	String& operator =(String&& other);
	String(const String& other);
	String(String&& other);
	~String();

public:
	size_t getLen() const;
	const char* getData() const;

private:
	void concat(const char* s, size_t sLen);
	void concat(const String& other);

	void resize(size_t newCap);
	static bool checkTooSmall(size_t len, size_t cap);
	static bool checkTooLarge(size_t len, size_t cap);
	static size_t* getPrefixFunction(const char* s, size_t len);

public:
	bool findSubstr(String& s) const;
	bool findSubstr(const char* s, size_t n) const;
	bool findSubstr(const char* s, size_t n, const size_t* f) const;
	bool findSubstr(const String& s, const size_t* f) const;

public:
	String& operator +=(const String& other);
	friend String operator +(const String& lhs, const String& rhs);
	
	friend std::istream& operator >>(std::istream& is, String& s);
	friend std::ostream& operator <<(std::ostream& os, const String& s);
};

#endif // !__STRING_H
