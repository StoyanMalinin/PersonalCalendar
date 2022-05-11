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
	void shallowCopyFromOther(const String& other);

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

public:
	bool findSubstr(String& s) const;
	bool findSubstr(const char* s) const;
	/// <summary>
	/// Checks if <b>s</b> is a substring of this string
	/// </summary>
	/// <param name="s"></param>
	/// <param name="n">length of <b>s</b></param>
	/// <returns></returns>
	bool findSubstr(const char* s, size_t n) const;
	/// <summary>
	/// Checks if <b>s</b> is a substring of this string
	/// </summary>
	/// <param name="s"></param>
	/// <param name="n">length of <b>s</b></param>
	/// <param name="f">The prefix function from the Knuth–Morris–Pratt algorithm of <b>s</b></param>
	/// <returns></returns>
	bool findSubstr(const char* s, size_t n, const size_t* f) const;
	/// <summary>
	/// Checks if <b>s</b> is a substring of this string
	/// </summary>
	/// <param name="s"></param>
	/// <param name="f">The prefix function from the Knuth–Morris–Pratt algorithm of <b>s</b></param>
	/// <returns></returns>
	bool findSubstr(const String& s, const size_t* f) const;
	/// <summary>
	/// Gets the prefix function of <b>s</b> from the Knuth–Morris–Pratt algorithm
	/// </summary>
	/// <param name="s"></param>
	/// <param name="len">length of <b>s</b></param>
	/// <returns></returns>
	static size_t* getPrefixFunction(const char* s, size_t len);

public:
	String& operator +=(const String& other);
	String& operator +=(char c);
	friend String operator +(const String& lhs, const String& rhs);
	friend String operator +(char c, const String& s);
	friend String operator +(const String& s, char c);
	
	friend void getline(std::istream& is, String& s);
	friend std::istream& operator >>(std::istream& is, String& s);
	friend std::ostream& operator <<(std::ostream& os, const String& s);

	friend bool operator <(const String& lhs, const String& rhs);
	friend bool operator >(const String& lhs, const String& rhs);
	friend bool operator <=(const String& lhs, const String& rhs);
	friend bool operator >=(const String& lhs, const String& rhs);
	friend bool operator ==(const String& lhs, const String& rhs);
	friend bool operator !=(const String& lhs, const String& rhs);
	
	void swap(String& other);
	static String toString(size_t num);
	/// <summary>
	/// Addes characters to the left or to the right of <b>s</b> to make it at least as long as <b>len</b>
	/// </summary>
	/// <param name="s"></param>
	/// <param name="len">minimum length of resulting string</param>
	/// <param name="c">character to add</param>
	/// <param name="pos">false - left, true - right</param>
	/// <returns></returns>
	static String format(String s, size_t len, char c, bool pos);
};

#endif // !__STRING_H
