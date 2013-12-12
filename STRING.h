#ifndef STRING_H
#define STRING_H

#include <limits>
#include <iostream>

template <typename T>
class STRING
{
public:
	STRING() // ����������� �� ���������.
		{ throw std::bad_typeid("STRING(): ������. ������������� ���."); }
	STRING(T p) // �����������. ������������� ��������.
		{ throw std::bad_typeid("STRING(T p): ������. ������������� ���."); }
	STRING(const T *init) // �����������. ������������� ������.
		{ throw std::bad_typeid("STRING(T *): ������. ������������� ���."); }
	STRING(const STRING& other); // ���������� �����.

	friend std::basic_ostream<T>& operator<<(std::basic_ostream<T>& out, const STRING& other)
	{
		out << other.c_str();
		return out;
	}
	friend std::basic_istream<T>& operator>>(std::basic_istream<T>& in, STRING& other)
	{
		int bufsCount = INT_MAX / bufSize;
		bool nullFound = false;
		T *buf = NULL, *tmpBuf = NULL;
		for(int i = 1; i <= bufsCount; i++)
		{
			buf = new T[bufSize * i];
			if(buf == NULL)
			{
				if(tmpBuf != NULL)
					delete [] tmpBuf;
				throw std::bad_alloc("STRING::operator>>: ������. ������������ ������.");
			}
			sstrcpy(buf, tmpBuf);
			in.getline(buf + (bufSize-1) * (i-1), bufSize);
			if(in.bad())
			{
				delete [] buf;
				if(tmpBuf != NULL)
					delete [] tmpBuf;
				throw std::ios_base::failure("STRING::operator>>: ������ ������ �����-������.");
			}
			if(!in.gcount())
			{
				delete [] buf;
				if(tmpBuf != NULL)
					delete [] tmpBuf;
				other.clear();
				return in;
			}
			if(!in.fail())
			{
				nullFound = true;
				if(tmpBuf != NULL)
					delete [] tmpBuf;
				break;
			}
			in.clear();
			if(tmpBuf != NULL)
				delete [] tmpBuf;
			tmpBuf = buf;
		}
		if(!nullFound)
		{
			delete [] buf;
			if(tmpBuf != NULL)
				delete [] tmpBuf;
			throw std::out_of_range("STRING::operator>>: ������. ������� ������� ������ �� �����.");
		}
		other.len = sstrlen(buf)+1;
		delete [] other.str;
		other.str = new T[other.len];
		if(other.str == NULL)
			throw std::bad_alloc("STRING::operator>>: ������. ������������ ������.");
		sstrcpy(other.str, buf);
		delete [] buf;
		return in;
	}

	friend STRING operator+(const STRING& x, const STRING& y)
	{
		T *tmpStr = new T[x.len + y.len - 1];
		if(tmpStr == NULL)
			throw std::bad_alloc("STRING::operator+: ������. ������������ ������.");
	    sstrcpy(tmpStr, x.str);
	    sstrcat(tmpStr, y.str);
	    STRING<T> result(tmpStr);
	    delete [] tmpStr;
	    return result;
	}
	
	friend bool operator==(const STRING& x, const STRING& y)
		{ return sstrcmp(x.str, y.str) == 0; }
	friend bool operator!=(const STRING& x, const STRING& y)
		{ return sstrcmp(x.str, y.str) != 0; }
	friend bool operator<(const STRING& x, const STRING& y)
		{ return sstrcmp(x.str, y.str) < 0; }
    friend bool operator>(const STRING& x, const STRING& y)
		{ return sstrcmp(x.str, y.str) > 0; }
    friend bool operator<=(const STRING& x, const STRING& y)
		{ return sstrcmp(x.str, y.str) <= 0; }
    friend bool operator>=(const STRING& x, const STRING& y)
		{ return sstrcmp(x.str, y.str) >= 0; }

	STRING& operator*(int n); // ��������� ������ n ���.
	STRING& operator+=(const STRING& other)
		{ return *this = *this + other; }
	STRING& operator=(const STRING& other);
	T& operator[] (int i) const;

	int find(const STRING& s, int n = 0) const; // ����� ��������� � ������, ������� � ������� n.
	bool replace(const STRING& from, const STRING& to, bool replaceAll = false, int n = 0); // ������ ��������� �� ������ , ������� � ������� n.
	STRING substr(int pos, int len = npos) const; // ����� ���������.
	void insert(const STRING& str, int pos); // �������� ������ � ������� pos.
	void erase(int pos, int len = npos); // �������� ����� ������ � ������� pos ������ len.
	STRING reverse() const; // �������� ������������ ������.
	void clear(); // �������� ������.
	bool isEmpty() const; // ��������� ������ �� �������.

	const T* c_str() const // �������� ��������� �� �����.      
		{ return str; }
	int length() const  // �������� ����� ������.        
		{ return len; }

	~STRING(); // ����������.
private:
	T *str; // ����� ��� �������� ������.
	int len; // ����� ������.
	static const int npos = -1;
	static const int bufSize = 256; // ������ ������ ��� �����.

	static int sstrlen(const T *p); // �������� ����� ������.
	static T* sstrcpy(T *dest, const T *source); // ��������� ����������� ������.
	static T* sstrncpy(T *dest, const T *source, int size); // ��������� ����������� n �������� ������.
	static int sstrcmp(const T *str1, const T *str2); // �������� ��� ������ �����������������.
	static T* sstrcat(T *dest, const T *source); // ��������� ������� �������.
};

template <>
STRING<char>::STRING()
{
	len = 1;
	str = new char[len];
	if(str == NULL)
		throw std::bad_alloc("STRING<char>(): ������. ������������ ������.");
	str[0] = 0;
}

template <>
STRING<wchar_t>::STRING()
{
	len = 1;
	str = new wchar_t[len];
	if(str == NULL)
		throw std::bad_alloc("STRING<wchar_t>(): ������. ������������ ������.");
	str[0] = 0;
}

template <>
STRING<char>::STRING(const char *init)
{
	if(init == NULL)
		throw std::invalid_argument("STRING<char>(char *): ������. NULL ��� ��������.");
	len = sstrlen(init) + 1;
	str = new char[len];
	if(str == NULL)
		throw std::bad_alloc("STRING<char>(char *): ������. ������������ ������.");
	sstrcpy(str, init);
}

template <>
STRING<wchar_t>::STRING(const wchar_t *init)
{
	if(init == NULL)
		throw std::invalid_argument("STRING<wchar_t>(wchar_t *): ������. NULL ��� ��������.");
	len = sstrlen(init) + 1;
	str = new wchar_t[len];
	if(str == NULL)
		throw std::bad_alloc("STRING<wchar_t>(wchar_t *): ������. ������������ ������.");
	sstrcpy(str, init);
}

template <>
STRING<char>::STRING(char p)
{
	if(p <= 0)
		throw std::invalid_argument("STRING<char>(char p): ������. ������������ ������.");
	len = 2;
	str = new char[len];
	if(str == NULL)
		throw std::bad_alloc("STRING<char>(char p): ������. ������������ ������.");
	str[0] = p;
	str[1] = 0;
}

template <>
STRING<wchar_t>::STRING(wchar_t p)
{
	if(p <= 0)
		throw std::invalid_argument("STRING<wchar_t>(wchar_t p): ������. ������������ ������.");
	len = 2;
	str = new wchar_t[len];
	if(str == NULL)
		throw std::bad_alloc("STRING<wchar_t>(wchar_t p): ������. ������������ ������.");
	str[0] = p;
	str[1] = 0;
}

// ���������� �����.
template <typename T>
STRING<T>::STRING(const STRING<T>& other)
{
	len = other.length();
	str = new T[len];
	if(str == NULL)
		throw std::bad_alloc("STRING(STRING& ): ������. ������������ ������.");
	sstrcpy(str, other.c_str());
}

template <typename T>
STRING<T>& STRING<T>::operator=(const STRING<T>& other)
{
	if (this == &other)
		return *this;
	delete [] str;
	len = other.length();
	str = new T[len];
	if(str == NULL)
		throw std::bad_alloc("STRING::operator=: ������. ������������ ������.");
	sstrcpy(str, other.c_str());
	return *this;
}

// ��������� ������ n ���.
template <typename T>
STRING<T>& STRING<T>::operator*(int n)
{
	if(n <= 0)
		throw std::invalid_argument("STRING::operator*: ������. ���������� ���������� �� ����� ���� ������ ���� ��� ����.");
	len = (len - 1) * n + 1;
	T *tmpStr = new T[len];
	if(tmpStr == NULL)
		throw std::bad_alloc("STRING::operator*: ������. ������������ ������.");
	sstrcpy(tmpStr, str);
	for(int i = 0; i < (n - 1); i++)
		sstrcat(tmpStr, str);
	delete [] str;
	str = tmpStr;
	return *this;
}

template <typename T>
T& STRING<T>::operator[] (int i) const
{
	if( (i < 0) || (i >= len) )
		throw std::out_of_range("STRING::operator[]: ������. ����� �� ������� ������.");
	return str[i];
}

// ����� ��������� � ������, ������� � ������� n.
// ���������:
// s - ������, ������� ��������� �����
// n - ������� � ������� ������� ��������� �����
// ������������ ��������:
// ������� � ������� ������� ������ s
// ��� -1 ���� ������ �� �������
template <typename T>
int STRING<T>::find(const STRING<T>& s, int n) const
{
	if(n < 0)
		throw std::out_of_range("STRGING::find: ������. ����� �� ������� ������.");
	int lenx = s.length()-1;
    if( (lenx == 0) || (lenx > len) || (n >= len) || ((n + lenx) > len) )
		return npos;
	for(; n <= len - lenx; ++n)
    {
		int i = 0;
		while(i < lenx && str[n + i] == s[i])
			++i;
		if(i == lenx)
			return n;
	}
	return npos;
}

// ������ ��������� �� ������, ������� � ������� n.
// ���������:
// from - ������, ������� ���������� ��������
// to - ������ �� ������� ���������� ��������
// replaceAll - �������� ��� ������ from �� to, ��� ������ ������
// n - �������, � ������� ���������� ������ ������ from
// ������������ ��������:
// true, ���� ������ ���� �����������
// ����� false
template <typename T>
bool STRING<T>::replace(const STRING<T>& from, const STRING<T>& to, bool replaceAll, int n)
{
	if(n < 0)
		throw std::out_of_range("STRING::replace: ������. ����� �� ������� ������.");
	const int len1 = from.length()-1;
	const int len2 = to.length()-1;
	if(len1 == 0 || len1 > len || n + len1 > len)
		return false;
	int *numpos = new int[len / len1 + 1];
	if(numpos == NULL)
		throw std::bad_alloc("STRING::replace: ������. ������������ ������.");
	int pos1 = 0;
	do
		if((n = find(from, n)) != npos)
			numpos[pos1++] = n++;
	while(n != npos && replaceAll);
	if(!pos1)
	{
		delete [] numpos;
		return false;
	}
	int p = len + (len2 - len1) * pos1 + 1;
	T *newstr = new T[len + (len2 - len1) * pos1 + 1];
	if(newstr == NULL)
		throw std::bad_alloc("STRING::replace: ������. ������������ ������.");
	int i, pos2;
	for(n = 0, i = 0, pos2 = 0; n < len; n++, i++)
	{
		if(pos2 == pos1 || n < numpos[pos2])
			newstr[i] = str[n];
		else
		{
			for(int j = 0; j < len2; i++, j++)
				newstr[i] = to[j];
			++pos2;
			--i;
			n += len1 - 1;
		}
	}
	delete [] str;
	len = sstrlen(newstr) + 1;
	str = newstr;
	delete [] numpos;
	return true;
}

// ����� ���������.
// ���������:
// pos - �������, ������� � ������� ������� ���������
// len - ����� ���������
// ������������ ��������:
// ���������
template <typename T>
STRING<T> STRING<T>::substr(int pos, int len) const
{
	if( (pos < 0) || (pos >= this->len) )
		throw std::out_of_range("STRING::substr: ������. ����� �� ������� ������.");
	if( (len < 0) || ((pos + len) >= this->len) )
		len = this->len - pos - 1;
	T *tmpStr = new T[len + 1];
	if(tmpStr == NULL)
		throw std::bad_alloc("STRING::substr: ������. ������������ ������.");
	sstrncpy(tmpStr, str+pos, len);
	tmpStr[len] = 0;
	STRING<T> resultStr(tmpStr);
	delete [] tmpStr;
	return resultStr;
}

// �������� ������ � ������� pos.
// ���������:
// str - ������, ������� ���������� ��������
// pos - �������, � ������� ���������� �������� ������ str
template <typename T>
void STRING<T>::insert(const STRING<T>& str, int pos)
{
	if( (pos < 0) || (pos >= len) )
		throw std::out_of_range("STRING::insert: ������. ����� �� ������� ������.");
	if(str.isEmpty())
		return;
	T *tmpStr = new T[len + str.length() - 1];
	if(tmpStr == NULL)
		throw std::bad_alloc("STRING::insert: ������. ������������ ������.");
	for(int i = 0; i < pos; i++)
		tmpStr[i] = this->str[i];
	for(int i = pos, j = 0; j < str.length() - 1; i++, j++)
		tmpStr[i] = str[j];
	for(int i = pos + str.length() - 1, j = pos; j < len; i++, j++)
		tmpStr[i] = this->str[j];
	len = len + str.length() - 1;
	delete [] this->str;
	this->str = tmpStr;
}

// �������� ����� ������ � ������� pos ������ len.
// ���������:
// pos - �������, ������� � ������� ��������� ���������
// len - ����� ���������, ������� ����� ��������
template <typename T>
void STRING<T>::erase(int pos, int len)
{
	if( (pos < 0) || (pos >= this->len) )
		throw std::out_of_range("STRING::erase: ������. ����� �� ������� ������.");
	if(len == 0)
		return;
	if( (len < 0) || ((pos + len) >= this->len) )
		len = this->len - pos - 1;
	T *tmpStr = new T[this->len - len];
	if(tmpStr == NULL)
		throw std::bad_alloc("STRING::erase: ������. ������������ ������.");
	for(int i = 0; i < pos; i++)
		tmpStr[i] = this->str[i];
	for(int i = pos, j = pos + len; j < this->len; i++, j++)
		tmpStr[i] = this->str[j];
	this->len = this->len - len;
	delete [] this->str;
	this->str = tmpStr;
}

// �������� ������������ ������.
template <typename T>
STRING<T> STRING<T>::reverse() const
{
	T *tmpStr = new T[len];
	if(tmpStr == NULL)
		throw std::bad_alloc("STRING::reverse: ������. ������������ ������.");
	for(int i = (len-2), j = 0; i >= 0; i--, j++)
		tmpStr[j] = str[i];
	tmpStr[len-1] = 0;
	return STRING<T>(tmpStr);
}

// �������� ������.
template <typename T>
void STRING<T>::clear()
{
	delete [] str;
	len = 1;
	str = new T[len];
	if(str == NULL)
		throw std::bad_alloc("STRING::clear: ������. ������������ ������.");
	str[0] = 0;
}

// ��������� ������ �� �������.
template <typename T>
bool STRING<T>::isEmpty() const
{
	if(len == 1 && str[0] == 0)
		return true;
	return false;
}

// ����������.
template <typename T>
STRING<T>::~STRING()
{
	delete [] str;
}

// ��������������� �������

// �������� ����� ������.
// ���������:
// p - ������, �������������� ����-��������
// ������������ ��������:
// ����� ������ 
template <typename T>
int STRING<T>::sstrlen(const T *p)
{
	if(p == NULL)
		return -1;
	int i = 0;
	while(p[i])
		i++;
	return i;
}

// ��������� ����������� ������.
// ���������:
// dest - �����, � ������� ���������� ��������� �����������
// source - �����, ������� ���������� ����������
// ������������ ��������:
// dest
template <typename T>
T* STRING<T>::sstrcpy(T *dest, const T *source)
{
	if(dest == NULL)
		return NULL;
	if(source == NULL)
		return dest;
	int i = 0;
	while(dest[i] = source[i])
		i++;
	return dest;
}

// ��������� ����������� n �������� ������.
// ���������������� ��� � sstrcpy, ��
// ���������� �� ���� size ��������
template <typename T>
T* STRING<T>::sstrncpy(T *dest, const T *source, int size)
{
	if(dest == NULL)
		return NULL;
	if(source == NULL)
		return dest;
	if(size <= 0)
		return NULL;
	int i = 0;
	while( (dest[i] = source[i]) && (i < size) )
		i++;
	return dest;
}

// �������� ��� ������ �����������������.
template <typename T>
int STRING<T>::sstrcmp(const T *str1, const T *str2)
{
	if( (str1 == NULL) && (str2 == NULL) )
		return 0;
	if(str1 == NULL)
		return -1;
	if(str2 == NULL)
		return 1;
	int i = 0;
	while( str1[i] && str2[i] && (str1[i] == str2[i]) )
        i++;
	if(str1[i] < str2[i])
		return -1;
	else if(str1[i] > str2[i])
		return 1;
	else
		return 0;
}

// ��������� ������� �������.
// ���������:
// dest - ������, �������������� ����-�������� � �������
// ���������� ���������� �����������
// source - ������, ������� ���������� �����������
// ������������ ��������: 
// dest
template <typename T>
T* STRING<T>::sstrcat(T *dest, const T *source)
{
	if(dest == NULL)
		return NULL;
	if(source == NULL)
		return dest;
	int i = 0, j = 0;
	while(dest[i])
		i++;
	while(source[j])
	{
		dest[i] = source[j];
		i++;
		j++;
	}
	dest[i] = 0;
	return dest;
}

typedef STRING<char> dzString;
typedef STRING<wchar_t> dzWString;

#endif // STRING_H