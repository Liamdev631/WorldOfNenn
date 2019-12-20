#pragma once
#include "Global.h"

using namespace std;

class WPacket
{
public:

private:
	size_t m_size;
	u8* m_data;
	u8* m_writeOffset;

public:
	WPacket(size_t size)
		: m_size(size)
	{
		m_data = new u8[m_size];
		m_writeOffset = m_data + sizeof(u32);
	}

	~WPacket()
	{

	}

	//template <typename T>
	//void write(const T* value)
	//{
	//	memcpy(m_writeOffset, value, sizeof(T));
	//	m_writeOffset += sizeof(T);
	//}

	u8 const& operator[](std::size_t index) const
	{
#ifdef _DEBUG
		if (index >= m_size) {
			printf("WPacket operator[] overflow %p\n", this);
			throw std::exception("WPacket operator[] overflow.");
		}
#endif
		return m_data[index];
	};

	template <typename T>
	void write(const T& value)
	{
		memcpy(m_writeOffset, &value, sizeof(T));
		m_writeOffset += sizeof(T);
	}

	template <typename T>
	T* writePacket()
	{
		//T* addr = reinterpret_cast<T*>(m_writeOffset);
		//T* return = new(m_writeOffset)T();
		//m_writeOffset += sizeof(T);
		//return addr;

		T* ret = new(m_writeOffset) T();
		m_writeOffset += sizeof(T);
		return ret;
	}

	size_t getSize() const
	{
		return m_size;
	}

	u8* getData() const
	{
		return m_data;
	}

	size_t getBytesWritten() const
	{
		return m_writeOffset - m_data;
	}

	void flush()
	{
		m_writeOffset = m_data + sizeof(u32);
	}

	u32& getTimestamp()
	{
		return *reinterpret_cast<u32*>(m_data);
	}

};