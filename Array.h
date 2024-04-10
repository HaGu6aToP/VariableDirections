//---------------------------------------------------------------------------

#ifndef ArrayH
#define ArrayH

#include <iostream>

namespace MyClasses {
	template<typename T>
	class Array
	{
	private:
		T* m_array;
		int m_len;
	public:
		Array() : m_len{ 0 }, m_array{ nullptr }
		{
		}

		Array(int len) : m_len{ len }, m_array{ new T[len] }
		{
			for (int i{ 0 }; i < m_len; i++)
				m_array[i] = T{};
		}

		Array(const Array<T>& arr)
		{
			m_len = arr.len();
			m_array = new T[m_len];
			for (int i{ 0 }; i < m_len; i++)
				m_array[i] = arr[i];
		}

		Array(int len, T* arr) : Array(len)
		{
			m_array = new T[len];
			for (int i{ 0 }; i < len; i++)
				m_array[i] = arr[i];
		}

		~Array()
		{
			delete[] m_array;
		}

		int len() const
		{
			return m_len;
		}

		T& operator [](int i)
		{
			return m_array[i];
		}

		const T& operator [](int i) const
		{
			return m_array[i];
		}

		friend std::ostream& operator << (std::ostream& out, const Array<T>& array)
		{
			out << ' ';
			for (int i{ 0 }; i < array.len(); i++)
				out << array[i] << ' ';
			return (out << '\n');
		}

		friend bool operator == (const Array<T>& a1, const Array<T>& a2)
		{
			if (a1.len() != a2.len()) return false;

			bool flag = true;

			for (int i{ 0 }; i < a1.len(); i++)
			{
				flag = (a1[i] == a2[i]);
				if (!flag) break;
			}

			return flag;
		}


		Array& operator =(const Array& array)
		{
			m_len = array.len();
			delete[] m_array;
			m_array = new T[m_len];

			for (int i{ 0 }; i < m_len; i++)
				m_array[i] = array[i];

			return *this;
		}

		T append(T el)
		{
			Array<T> temp{ *this };
			m_len++;

			if (!(m_array == nullptr))
				delete[] m_array;
			m_array = new T[m_len];

			for (int i{ 0 }; i < m_len - 1; i++)
				m_array[i] = temp[i];

			m_array[m_len - 1] = el;

			return el;
		}

		int pop()
		{
			setLength(m_len - 1);
		}

		int in(const T& el) const
		{
			for (int i{ 0 }; i < m_len; i++)
				if (m_array[i] == el) return i;

			return -1;
		}

		void setLength(int len)
		{
			if (len <= 0)
			{
				m_len = 0;
				delete[] m_array;
				m_array = nullptr;
				return;
			}
			T* arr = new T[len];

			for (int i = 0; i < m_len; i++)
				arr[i] = m_array[i];

			delete[] m_array;
			m_array = arr;
			m_len = len;
		}

		void clear()
		{
			if (m_array == nullptr) return;
			delete[] m_array;
			m_array = nullptr;
			m_len = 0;
		}

		void remove(int index)
		{
			for (int i = index; i < m_len - 1; i++)
				m_array[i] = m_array[i + 1];

			m_len--;
			setLength(m_len);
		}

		void write(FILE* f)
		{
			fwrite(&m_len, sizeof(int), 1, f);
			fwrite(m_array, sizeof(T) * m_len, 1, f);
		}

		void read(FILE* f)
		{
			fread(&m_len, sizeof(int), 1, f);
			fread(m_array, sizeof(T) * m_len, 1, f);
		}

	};
}
#endif
