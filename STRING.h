#ifndef STRING_H
#define STRING_H

#include <limits>
#include <iostream>

template <typename T>
class STRING
{
public:
	STRING() // Конструктор по умолчанию.
		{ throw std::bad_typeid("STRING(): Ошибка. Неразрешенный тип."); }
	STRING(T p) // Конструктор. Инициализация символом.
		{ throw std::bad_typeid("STRING(T p): Ошибка. Неразрешенный тип."); }
	STRING(const T *init) // Конструктор. Инициализация срокой.
		{ throw std::bad_typeid("STRING(T *): Ошибка. Неразрешенный тип."); }
	STRING(const STRING& other); // Констуктор копии.

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
				throw std::bad_alloc("STRING::operator>>: Ошибка. Недостаточно памяти.");
			}
			sstrcpy(buf, tmpBuf);
			in.getline(buf + (bufSize-1) * (i-1), bufSize);
			if(in.bad())
			{
				delete [] buf;
				if(tmpBuf != NULL)
					delete [] tmpBuf;
				throw std::ios_base::failure("STRING::operator>>: Ошибка потока ввода-вывода.");
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
			throw std::out_of_range("STRING::operator>>: Ошибка. Слишком длинная строка на входе.");
		}
		other.len = sstrlen(buf)+1;
		delete [] other.str;
		other.str = new T[other.len];
		if(other.str == NULL)
			throw std::bad_alloc("STRING::operator>>: Ошибка. Недостаточно памяти.");
		sstrcpy(other.str, buf);
		delete [] buf;
		return in;
	}

	friend STRING operator+(const STRING& x, const STRING& y)
	{
		T *tmpStr = new T[x.len + y.len - 1];
		if(tmpStr == NULL)
			throw std::bad_alloc("STRING::operator+: Ошибка. Недостаточно памяти.");
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

	STRING& operator*(int n); // Повторить строку n раз.
	STRING& operator+=(const STRING& other)
		{ return *this = *this + other; }
	STRING& operator=(const STRING& other);
	T& operator[] (int i) const;

	int find(const STRING& s, int n = 0) const; // Поиск подстроки в строке, начиная с позиции n.
	bool replace(const STRING& from, const STRING& to, bool replaceAll = false, int n = 0); // Замена подстроки на строку , начиная с позиции n.
	STRING substr(int pos, int len = npos) const; // Взять подстроку.
	void insert(const STRING& str, int pos); // Вставить строку в позицию pos.
	void erase(int pos, int len = npos); // Очистить часть строки с позиции pos длиной len.
	STRING reverse() const; // Получить перевернутую строку.
	void clear(); // Очистить строку.
	bool isEmpty() const; // Проверить строку на пустоту.

	const T* c_str() const // Получить указатель на буфер.      
		{ return str; }
	int length() const  // Получить длину строки.        
		{ return len; }

	~STRING(); // Деструктор.
private:
	T *str; // Буфер для хранения строки.
	int len; // Длина строки.
	static const int npos = -1;
	static const int bufSize = 256; // Размер буфера для ввода.

	static int sstrlen(const T *p); // Получить длину буфера.
	static T* sstrcpy(T *dest, const T *source); // Выполнить копирование буфера.
	static T* sstrncpy(T *dest, const T *source, int size); // Выполнить копирование n символов буфера.
	static int sstrcmp(const T *str1, const T *str2); // Сравнить два буфера лексикографически.
	static T* sstrcat(T *dest, const T *source); // Выполнить слияние буферов.
};

template <>
STRING<char>::STRING()
{
	len = 1;
	str = new char[len];
	if(str == NULL)
		throw std::bad_alloc("STRING<char>(): Ошибка. Недостаточно памяти.");
	str[0] = 0;
}

template <>
STRING<wchar_t>::STRING()
{
	len = 1;
	str = new wchar_t[len];
	if(str == NULL)
		throw std::bad_alloc("STRING<wchar_t>(): Ошибка. Недостаточно памяти.");
	str[0] = 0;
}

template <>
STRING<char>::STRING(const char *init)
{
	if(init == NULL)
		throw std::invalid_argument("STRING<char>(char *): Ошибка. NULL как аргумент.");
	len = sstrlen(init) + 1;
	str = new char[len];
	if(str == NULL)
		throw std::bad_alloc("STRING<char>(char *): Ошибка. Недостаточно памяти.");
	sstrcpy(str, init);
}

template <>
STRING<wchar_t>::STRING(const wchar_t *init)
{
	if(init == NULL)
		throw std::invalid_argument("STRING<wchar_t>(wchar_t *): Ошибка. NULL как аргумент.");
	len = sstrlen(init) + 1;
	str = new wchar_t[len];
	if(str == NULL)
		throw std::bad_alloc("STRING<wchar_t>(wchar_t *): Ошибка. Недостаточно памяти.");
	sstrcpy(str, init);
}

template <>
STRING<char>::STRING(char p)
{
	if(p <= 0)
		throw std::invalid_argument("STRING<char>(char p): Ошибка. Недопустимый символ.");
	len = 2;
	str = new char[len];
	if(str == NULL)
		throw std::bad_alloc("STRING<char>(char p): Ошибка. Недостаточно памяти.");
	str[0] = p;
	str[1] = 0;
}

template <>
STRING<wchar_t>::STRING(wchar_t p)
{
	if(p <= 0)
		throw std::invalid_argument("STRING<wchar_t>(wchar_t p): Ошибка. Недопустимый символ.");
	len = 2;
	str = new wchar_t[len];
	if(str == NULL)
		throw std::bad_alloc("STRING<wchar_t>(wchar_t p): Ошибка. Недостаточно памяти.");
	str[0] = p;
	str[1] = 0;
}

// Констуктор копии.
template <typename T>
STRING<T>::STRING(const STRING<T>& other)
{
	len = other.length();
	str = new T[len];
	if(str == NULL)
		throw std::bad_alloc("STRING(STRING& ): Ошибка. Недостаточно памяти.");
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
		throw std::bad_alloc("STRING::operator=: Ошибка. Недостаточно памяти.");
	sstrcpy(str, other.c_str());
	return *this;
}

// Повторить строку n раз.
template <typename T>
STRING<T>& STRING<T>::operator*(int n)
{
	if(n <= 0)
		throw std::invalid_argument("STRING::operator*: Ошибка. Количество повторение не может быть меньше нуля или ноль.");
	len = (len - 1) * n + 1;
	T *tmpStr = new T[len];
	if(tmpStr == NULL)
		throw std::bad_alloc("STRING::operator*: Ошибка. Недостаточно памяти.");
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
		throw std::out_of_range("STRING::operator[]: Ошибка. Выход за границу строки.");
	return str[i];
}

// Поиск подстроки в строке, начиная с позиции n.
// Параметры:
// s - строка, которую требуется найти
// n - позиция с которой следует выполнить поиск
// Возвращаемое значение:
// позиция в которой найдена строка s
// или -1 если строка не найдена
template <typename T>
int STRING<T>::find(const STRING<T>& s, int n) const
{
	if(n < 0)
		throw std::out_of_range("STRGING::find: Ошибка. Выход за границу строки.");
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

// Замена подстроки на строку, начиная с позиции n.
// Параметры:
// from - строка, которую необходимо заменить
// to - строка на которую необходимо заменить
// replaceAll - заменить все строки from на to, или только первую
// n - позиция, с которой необходимо искать строку from
// Возвращаемое значение:
// true, если замены были произведены
// иначе false
template <typename T>
bool STRING<T>::replace(const STRING<T>& from, const STRING<T>& to, bool replaceAll, int n)
{
	if(n < 0)
		throw std::out_of_range("STRING::replace: Ошибка. Выход за границу строки.");
	const int len1 = from.length()-1;
	const int len2 = to.length()-1;
	if(len1 == 0 || len1 > len || n + len1 > len)
		return false;
	int *numpos = new int[len / len1 + 1];
	if(numpos == NULL)
		throw std::bad_alloc("STRING::replace: Ошибка. Недостаточно памяти.");
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
		throw std::bad_alloc("STRING::replace: Ошибка. Недостаточно памяти.");
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

// Взять подстроку.
// Параметры:
// pos - позиция, начиная с которой берется подстрока
// len - длина подстроки
// Возвращаемое значение:
// подстрока
template <typename T>
STRING<T> STRING<T>::substr(int pos, int len) const
{
	if( (pos < 0) || (pos >= this->len) )
		throw std::out_of_range("STRING::substr: Ошибка. Выход за границу строки.");
	if( (len < 0) || ((pos + len) >= this->len) )
		len = this->len - pos - 1;
	T *tmpStr = new T[len + 1];
	if(tmpStr == NULL)
		throw std::bad_alloc("STRING::substr: Ошибка. Недостаточно памяти.");
	sstrncpy(tmpStr, str+pos, len);
	tmpStr[len] = 0;
	STRING<T> resultStr(tmpStr);
	delete [] tmpStr;
	return resultStr;
}

// Вставить строку в позицию pos.
// Параметры:
// str - строка, которую необходимо вставить
// pos - позиция, в которую необходимо вставить строку str
template <typename T>
void STRING<T>::insert(const STRING<T>& str, int pos)
{
	if( (pos < 0) || (pos >= len) )
		throw std::out_of_range("STRING::insert: Ошибка. Выход за границу строки.");
	if(str.isEmpty())
		return;
	T *tmpStr = new T[len + str.length() - 1];
	if(tmpStr == NULL)
		throw std::bad_alloc("STRING::insert: Ошибка. Недостаточно памяти.");
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

// Очистить часть строки с позиции pos длиной len.
// Параметры:
// pos - позиция, начиная с которой очищается подстрока
// len - длина подстроки, которую нужно очистить
template <typename T>
void STRING<T>::erase(int pos, int len)
{
	if( (pos < 0) || (pos >= this->len) )
		throw std::out_of_range("STRING::erase: Ошибка. Выход за границу строки.");
	if(len == 0)
		return;
	if( (len < 0) || ((pos + len) >= this->len) )
		len = this->len - pos - 1;
	T *tmpStr = new T[this->len - len];
	if(tmpStr == NULL)
		throw std::bad_alloc("STRING::erase: Ошибка. Недостаточно памяти.");
	for(int i = 0; i < pos; i++)
		tmpStr[i] = this->str[i];
	for(int i = pos, j = pos + len; j < this->len; i++, j++)
		tmpStr[i] = this->str[j];
	this->len = this->len - len;
	delete [] this->str;
	this->str = tmpStr;
}

// Получить перевернутую строку.
template <typename T>
STRING<T> STRING<T>::reverse() const
{
	T *tmpStr = new T[len];
	if(tmpStr == NULL)
		throw std::bad_alloc("STRING::reverse: Ошибка. Недостаточно памяти.");
	for(int i = (len-2), j = 0; i >= 0; i--, j++)
		tmpStr[j] = str[i];
	tmpStr[len-1] = 0;
	return STRING<T>(tmpStr);
}

// Очистить строку.
template <typename T>
void STRING<T>::clear()
{
	delete [] str;
	len = 1;
	str = new T[len];
	if(str == NULL)
		throw std::bad_alloc("STRING::clear: Ошибка. Недостаточно памяти.");
	str[0] = 0;
}

// Проверить строку на пустоту.
template <typename T>
bool STRING<T>::isEmpty() const
{
	if(len == 1 && str[0] == 0)
		return true;
	return false;
}

// Деструктор.
template <typename T>
STRING<T>::~STRING()
{
	delete [] str;
}

// Вспомогательные функции

// Получить длину буфера.
// Параметры:
// p - строка, оканчивающаяся нуль-символом
// Возвращаемое значение:
// длина строки 
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

// Выполнить копирование буфера.
// Параметры:
// dest - буфер, в который необходимо выполнить копирование
// source - буфер, который необходимо копировать
// Возвращаемое значение:
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

// Выполнить копирование n символов буфера.
// Функциональность как у sstrcpy, но
// копируется не боле size символов
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

// Сравнить два буфера лексикографически.
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

// Выполнить слияние буферов.
// Параметры:
// dest - строка, оканчивающаяся нуль-символом в которую
// необходимо произвести копирование
// source - строка, которую необходимо скопировать
// Возвращаемое значение: 
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