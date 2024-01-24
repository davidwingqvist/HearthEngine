#pragma once

/*

	Helper class for the ID3D11 Structures used for DirectX
	Don't use any other type of pointer for this class.
*/
template<typename T>
class DXPointer
{
private:

	T* m_data = nullptr;

public:

	DXPointer(){m_data = nullptr};
	~DXPointer();

	T* operator=(T* ptr)
	{
		m_data = ptr;
		return m_data;
	};

	T* Get();
	T** GetAddressOf();
	void Reset();
};

template<typename T>
inline DXPointer<T>::~DXPointer()
{
	if (m_data)
		m_data->Release();
}

template<typename T>
inline T* DXPointer<T>::Get()
{
	return m_data;
}

template<typename T>
inline T** DXPointer<T>::GetAddressOf()
{
	return &m_data;
}

template<typename T>
inline void DXPointer<T>::Reset()
{
	if (m_data)
		m_data->Release();
	m_data = nullptr;
}
