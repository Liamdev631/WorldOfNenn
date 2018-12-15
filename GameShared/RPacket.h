#pragma once
#include "Global.h"

using namespace std;

class RPacket
{
public:

private:
	const size_t m_size;
	const u8* m_data;
	u8* m_readOffset;

public:
	RPacket(const u8* data, size_t size)
		: m_size(size), m_data(data), m_readOffset((u8*)data + sizeof(u32))
	{

	}

	~RPacket()
	{
		// RPacket is not resposible for freeing the data
	}

	u8 const& operator[](std::size_t index) const
	{
		return m_data[index];
	};

	template <typename T>
	const T* read()
	{
		T* ret = reinterpret_cast<T*>(m_readOffset); 
		m_readOffset += sizeof(T);
		return ret;
	}

	template <typename T>
	const T& peek()
	{
		return *reinterpret_cast<T*>(m_readOffset);
	}

	const size_t getSize() const
	{
		return m_size;
	}

	const void* getData() const
	{
		return m_data;
	}

	const size_t getBytesRead() const
	{
		return m_readOffset - m_data;
	}

	const u32& getTimestamp() const
	{
		const u32& ts = *reinterpret_cast<const u32*>(m_data);
		return ts;
	}
};