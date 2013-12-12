#ifndef ARRAYOFSTR_H
#define ARRAYOFSTR_H

#include "STRING.h"

template <typename T>
class ARRAYOFSTR
{
public:
	ARRAYOFSTR(); // ����������� �� ���������.
	ARRAYOFSTR(int size); // ������� ������ ������� size.
	ARRAYOFSTR(const ARRAYOFSTR& other); // ����������� �����.

	friend std::basic_istream<T>& operator>>(std::basic_istream<T>& in, ARRAYOFSTR& other)
	{ 
		STRING<T> tmpStr;
		do {
			in >> tmpStr;
			if(!tmpStr.isEmpty())
				other += tmpStr;
		} while(!in.eof());
		return in;
	}
	friend std::basic_ostream<T>& operator<<(std::basic_ostream<T>& out, const ARRAYOFSTR& other)
	{ 
		for(int i = 0; i < other.len; i++)
			if(other.outWithNums)
				out << i+1 << " - " << other.arr[i] << std::endl;
			else
				out << other.arr[i] << std::endl;
		return out;
	}
	friend ARRAYOFSTR operator+(const ARRAYOFSTR<T>& x, const ARRAYOFSTR<T>& y)
	{
		ARRAYOFSTR<T> tmpArr(x.len + y.len);
		for(int i = 0; i < x.len; i++)
			tmpArr[i] = x[i];
		for(int i = x.len, j = 0; j < y.len; i++, j++)
			tmpArr[i] = y[j];
		return tmpArr;
	}
	friend ARRAYOFSTR operator+(const ARRAYOFSTR<T>& x, const STRING<T>& y)
	{
		ARRAYOFSTR<T> tmpArr(x.len + 1);
		for(int i = 0; i < x.len; i++)
			tmpArr[i] = x[i];
		tmpArr[x.len] = y;
		return tmpArr;
	}
	friend ARRAYOFSTR operator+(const STRING<T>& y, const ARRAYOFSTR<T>& x)
		{ return x + y; }

	STRING<T>& operator[] (int i) const;
	ARRAYOFSTR& operator=(const ARRAYOFSTR& other);
	ARRAYOFSTR& operator+=(const ARRAYOFSTR& other)
		{ return *this = *this + other; } 
	ARRAYOFSTR& operator+=(const STRING<T>& other)
		{ return *this = *this + other; }

	void insert(const STRING<T>& str, int pos); // �������� ������ � ������� pos.
	void insert(const ARRAYOFSTR& other, int pos); // �������� ������ � ������� pos.
	void erase(int pos = 0, int len = npos); // ������� ����� �������, ������� � ������� pos ������ len.
	ARRAYOFSTR subarr(int pos = 0, int len = npos) const; // ����� ���������, ������� � ������� pos ������ len.
	void clear(); // �������� ������.
	bool isEmpty() const // ���� �� ������.
		{ return len == 0; }
	int length() const // �������� ����� �������.
		{ return len; }

	void setOutWithNums(bool own)
		{ outWithNums = own; }
	~ARRAYOFSTR();
private:
	STRING<T> *arr; // ����� ��� �������.
	int len; // ����� �������.
	static const int npos = -1;
	bool outWithNums;
};

// ���������� �� ���������.
template <typename T>
ARRAYOFSTR<T>::ARRAYOFSTR()
: outWithNums(false)
{
	arr = NULL;
	len = 0;
}

// ������� ������ ������� size.
template <typename T>
ARRAYOFSTR<T>::ARRAYOFSTR(int size)
: outWithNums(false)
{
	if(size <= 0)
		throw std::invalid_argument("ARRAYOFSTR(int size): ������. ������ ������� �� ����� ���� ������������� ������.");
	len = size;
	arr = new STRING<T>[len];
	if(arr == NULL)
		throw std::bad_alloc("ARRAYOFSTR(int size): ������. ������������ ������.");
}

// ����������� �����.
template <typename T>
ARRAYOFSTR<T>::ARRAYOFSTR(const ARRAYOFSTR<T>& other)
{
	if(other.isEmpty())
	{
		len = 0;
		arr = NULL;
		return;
	}
	len = other.length();
	arr = new STRING<T>[len];
	if(arr == NULL)
		throw std::bad_alloc("ARRAYOFSTR(ARRAYOFSTR &): ������. ������������ ������.");
	for(int i = 0; i < len; i++)
		arr[i] = other[i];
}

template <typename T>
STRING<T>& ARRAYOFSTR<T>::operator[] (int i) const
{
	if( (i < 0) || (i >= len) )
		throw std::out_of_range("ARRAYOFSTR::operator[]: ������. ����� �� ������� �������.");
	return arr[i];
}

template <typename T>
ARRAYOFSTR<T>& ARRAYOFSTR<T>::operator=(const ARRAYOFSTR<T>& other)
{
	if(this == &other)
		return *this;
	if(other.isEmpty())
	{
		len = 0;
		if(arr != NULL)
			delete [] arr;
		arr = NULL;
		return *this;
	}
	if(len < other.length())
	{
		if(arr != NULL)
			delete [] arr;
		arr = new STRING<T>[other.length()];
		if(arr == NULL)
			throw std::bad_alloc("ARRAYOFSTR::operator=: ������. ������������ ������.");
	}
	len = other.length();
	for(int i = 0; i < len; i++)
		arr[i] = other[i];
	return *this;
}

// �������� ������ � ������� pos.
// ���������:
// str - ������, ������� ���������� �������� � ������
// pos - ������� � ������� ���������� �������� ������
template <typename T>
void ARRAYOFSTR<T>::insert(const STRING<T>& str, int pos)
{
	if( (pos < 0) || (pos > len) )
		throw std::out_of_range("ARRAYOFSTR::insert: ������. ����� �� ������� �������.");
	STRING<T> *tmpArr = new STRING<T>[++len];
	if(tmpArr == NULL)
		throw std::bad_alloc("ARRAYOFSTR::insert: ������. ������������ ������.");
	for(int i = 0; i < pos; i++)
		tmpArr[i] = arr[i];
	tmpArr[pos] = str;
	for(int i = (pos + 1); i < len; i++)
		tmpArr[i] = arr[i-1];
	delete [] arr;
	arr = tmpArr;
}

// �������� ������ � ������� pos.
// ���������:
// other - ������, ������� ���������� ��������
// pos - �������, � ������� ���������� �������� ������
template <typename T>
void ARRAYOFSTR<T>::insert(const ARRAYOFSTR& other, int pos)
{
	if( (pos < 0) || (pos > len) )
		throw std::out_of_range("ARRAYOFSTR::insert: ������. ����� �� ������� �������.");
	if(other.isEmpty())
		return;
	STRING<T> *tmpArr = new STRING<T>[len + other.len];
	if(tmpArr == NULL)
		throw std::bad_alloc("ARRAYOFSTR::insert: ������. ������������ ������.");
	for(int i = 0; i < pos; i++)
		tmpArr[i] = arr[i];
	for(int i = pos, j = 0; j < other.len; i++, j++)
		tmpArr[i] = other[j];
	for(int i = pos + other.len, j = pos; j < len; i++, j++)
		tmpArr[i] = arr[j];
	len = len + other.len;
	delete [] arr;
	arr = tmpArr;
}

// ������� ����� �������, ������� � ������� pos ������ len.
// ���������:
// pos - �������, ������� � ������� ���������� ������� ����� �������
// len - ����� �������, ������� ���������� �������
template <typename T>
void ARRAYOFSTR<T>::erase(int pos = 0, int len = npos)
{
	if( (pos < 0) || (pos >= this->len) )
		throw std::out_of_range("ARRAYOFSTR::erase: ������. ����� �� ������� �������.");
	if(len == 0)
		return;
	if( (len < 0) || ((pos + len) >= this->len) )
		len = this->len - pos;
	if(this->len == len)
	{
		clear();
		return;
	}
	STRING<T> *tmpArr = new STRING<T>[this->len - len];
	if(tmpArr == NULL)
		throw std::bad_alloc("ARRAYOFSTR::erase: ������. ������������ ������.");
	for(int i = 0; i < pos; i++)
		tmpArr[i] = this->arr[i];
	for(int i = pos, j = pos + len; j < this->len; i++, j++)
		tmpArr[i] = this->arr[j];
	this->len = this->len - len;
	delete [] this->arr;
	this->arr = tmpArr;
}

// ����� ���������, ������� � ������� pos ������ len.
// ���������:
// pos - �������, ������� � ������� ���������� ����� ���������
// len - ����� ����������
template <typename T>
ARRAYOFSTR<T> ARRAYOFSTR<T>::subarr(int pos = 0, int len = npos) const
{
	if( (pos < 0) || (pos >= this->len) )
		throw std::out_of_range("ARRAYOFSTR::subarr: ������. ����� �� ������� �������.");
	if(len == 0)
		return ARRAYOFSTR<T>();
	if( (len < 0) || ((pos + len) >= this->len) )
		len = this->len - pos;
	ARRAYOFSTR<T> tmpArr(len);
	for(int i = 0, j = pos; i < len; i++, j++)
		tmpArr[i] = arr[j];
	return tmpArr;
}

// �������� ������.
template <typename T>
void ARRAYOFSTR<T>::clear()
{
	if(arr != NULL)
		delete [] arr;
	len = 0;
	arr = NULL;
}

// ����������.
template <typename T>
ARRAYOFSTR<T>::~ARRAYOFSTR()
{
	if(arr != NULL)
		delete [] arr;
}

typedef ARRAYOFSTR<char> dzArray;
typedef ARRAYOFSTR<wchar_t> dzWArray;

#endif // ARRAYOFSTR_H