#ifndef ARRAYOFSTR_H
#define ARRAYOFSTR_H

#include "STRING.h"

template <typename T>
class ARRAYOFSTR
{
public:
	ARRAYOFSTR(); // Конструктор по умолчанию.
	ARRAYOFSTR(int size); // Создать массив размера size.
	ARRAYOFSTR(const ARRAYOFSTR& other); // Конструктор копии.

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

	void insert(const STRING<T>& str, int pos); // Вставить строку в позицию pos.
	void insert(const ARRAYOFSTR& other, int pos); // Вставить массив в позицию pos.
	void erase(int pos = 0, int len = npos); // Стереть часть массива, начиная с позиции pos длиной len.
	ARRAYOFSTR subarr(int pos = 0, int len = npos) const; // Взять подмассив, начиная с позиции pos длиной len.
	void clear(); // Очистить массив.
	bool isEmpty() const // Пуст ли массив.
		{ return len == 0; }
	int length() const // Получить длину массива.
		{ return len; }

	void setOutWithNums(bool own)
		{ outWithNums = own; }
	~ARRAYOFSTR();
private:
	STRING<T> *arr; // Буфер для массива.
	int len; // Длина массива.
	static const int npos = -1;
	bool outWithNums;
};

// Констуктор по умолчанию.
template <typename T>
ARRAYOFSTR<T>::ARRAYOFSTR()
: outWithNums(false)
{
	arr = NULL;
	len = 0;
}

// Создать массив размера size.
template <typename T>
ARRAYOFSTR<T>::ARRAYOFSTR(int size)
: outWithNums(false)
{
	if(size <= 0)
		throw std::invalid_argument("ARRAYOFSTR(int size): Ошибка. Размер массива не может быть отрицательным числом.");
	len = size;
	arr = new STRING<T>[len];
	if(arr == NULL)
		throw std::bad_alloc("ARRAYOFSTR(int size): Ошибка. Недостаточно памяти.");
}

// Конструктор копии.
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
		throw std::bad_alloc("ARRAYOFSTR(ARRAYOFSTR &): Ошибка. Недостаточно памяти.");
	for(int i = 0; i < len; i++)
		arr[i] = other[i];
}

template <typename T>
STRING<T>& ARRAYOFSTR<T>::operator[] (int i) const
{
	if( (i < 0) || (i >= len) )
		throw std::out_of_range("ARRAYOFSTR::operator[]: Ошибка. Выход за границу массива.");
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
			throw std::bad_alloc("ARRAYOFSTR::operator=: Ошибка. Недостаточно памяти.");
	}
	len = other.length();
	for(int i = 0; i < len; i++)
		arr[i] = other[i];
	return *this;
}

// Вставить строку в позицию pos.
// Параметры:
// str - строка, которую необходимо вставить в массив
// pos - возиция в которую необходимо вставить строку
template <typename T>
void ARRAYOFSTR<T>::insert(const STRING<T>& str, int pos)
{
	if( (pos < 0) || (pos > len) )
		throw std::out_of_range("ARRAYOFSTR::insert: Ошибка. Выход за границу массива.");
	STRING<T> *tmpArr = new STRING<T>[++len];
	if(tmpArr == NULL)
		throw std::bad_alloc("ARRAYOFSTR::insert: Ошибка. Недостаточно памяти.");
	for(int i = 0; i < pos; i++)
		tmpArr[i] = arr[i];
	tmpArr[pos] = str;
	for(int i = (pos + 1); i < len; i++)
		tmpArr[i] = arr[i-1];
	delete [] arr;
	arr = tmpArr;
}

// Вставить массив в позицию pos.
// Параметры:
// other - массив, который необходимо вставить
// pos - позиция, в которую необходимо вставить массив
template <typename T>
void ARRAYOFSTR<T>::insert(const ARRAYOFSTR& other, int pos)
{
	if( (pos < 0) || (pos > len) )
		throw std::out_of_range("ARRAYOFSTR::insert: Ошибка. Выход за границу массива.");
	if(other.isEmpty())
		return;
	STRING<T> *tmpArr = new STRING<T>[len + other.len];
	if(tmpArr == NULL)
		throw std::bad_alloc("ARRAYOFSTR::insert: Ошибка. Недостаточно памяти.");
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

// Стереть часть массива, начиная с позиции pos длиной len.
// Параметры:
// pos - позиция, начиная с которой необходимо стереть часть массива
// len - длина массива, которую необходимо удалить
template <typename T>
void ARRAYOFSTR<T>::erase(int pos = 0, int len = npos)
{
	if( (pos < 0) || (pos >= this->len) )
		throw std::out_of_range("ARRAYOFSTR::erase: Ошибка. Выход за границу массива.");
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
		throw std::bad_alloc("ARRAYOFSTR::erase: Ошибка. Недостаточно памяти.");
	for(int i = 0; i < pos; i++)
		tmpArr[i] = this->arr[i];
	for(int i = pos, j = pos + len; j < this->len; i++, j++)
		tmpArr[i] = this->arr[j];
	this->len = this->len - len;
	delete [] this->arr;
	this->arr = tmpArr;
}

// Взять подмассив, начиная с позиции pos длиной len.
// Параметры:
// pos - позиция, начиная с которой необходимо взять подмассив
// len - длина подмассива
template <typename T>
ARRAYOFSTR<T> ARRAYOFSTR<T>::subarr(int pos = 0, int len = npos) const
{
	if( (pos < 0) || (pos >= this->len) )
		throw std::out_of_range("ARRAYOFSTR::subarr: Ошибка. Выход за границу массива.");
	if(len == 0)
		return ARRAYOFSTR<T>();
	if( (len < 0) || ((pos + len) >= this->len) )
		len = this->len - pos;
	ARRAYOFSTR<T> tmpArr(len);
	for(int i = 0, j = pos; i < len; i++, j++)
		tmpArr[i] = arr[j];
	return tmpArr;
}

// Очистить массив.
template <typename T>
void ARRAYOFSTR<T>::clear()
{
	if(arr != NULL)
		delete [] arr;
	len = 0;
	arr = NULL;
}

// Деструктор.
template <typename T>
ARRAYOFSTR<T>::~ARRAYOFSTR()
{
	if(arr != NULL)
		delete [] arr;
}

typedef ARRAYOFSTR<char> dzArray;
typedef ARRAYOFSTR<wchar_t> dzWArray;

#endif // ARRAYOFSTR_H