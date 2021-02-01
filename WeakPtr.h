//
//  WeakPtr.hpp
//

#pragma once

#include "ReferenceCounter.h"

class ReferenceCounter;
template<typename T> class SharedPtr;
template<typename T>
class WeakPtr
{
public:
    WeakPtr();
    WeakPtr(const WeakPtr<T>& p);
    WeakPtr(const SharedPtr<T>& p);
    WeakPtr(T* pointer, ReferenceCounter* rc); // for casting
    ~WeakPtr();
    T& operator*() const;
    T* operator->() const;
    WeakPtr<T>& operator=(const WeakPtr<T>& p);
    bool operator==(const WeakPtr<T>& p) const;
    bool operator!=(const WeakPtr<T>& p) const;
    bool operator==(const T* const p) const;
    bool operator!=(const T* const p) const;
    bool Expired() const;
    bool IsValid() const { return m_object != nullptr; }
    T* Get() const;
    
    explicit operator bool() const { return m_object; }
    
    template<typename Y>
    WeakPtr<Y> As() const;
    
private:
    T* m_object;
    ReferenceCounter* m_rc;

    friend class SharedPtr<T>;
};

template<typename T>
WeakPtr<T>::WeakPtr() : m_object(nullptr), m_rc(nullptr)
{
    
}

template<typename T>
WeakPtr<T>::WeakPtr(const WeakPtr<T>& pointer) : m_object(pointer.m_object), m_rc(pointer.m_rc)
{
    if(m_rc)
    {
        m_rc->IncrementWeak();
    }
}

template<typename T>
WeakPtr<T>::WeakPtr(const SharedPtr<T>& pointer) : m_object(pointer.m_object), m_rc(pointer.m_rc)
{
    if(m_rc)
    {
        m_rc->IncrementWeak();
    }
}

template<typename T>
WeakPtr<T>::WeakPtr(T* pointer, ReferenceCounter* rc)  : m_object(pointer), m_rc(rc)
{
    if(m_rc)
    {
        m_rc->IncrementWeak();
    }
}

template <typename T>
WeakPtr<T>::~WeakPtr()
{
    if(m_rc && m_rc->DecrementWeak() == 0)
    {
        delete m_rc;
        m_rc = nullptr;
    }
}

template<typename T>
T& WeakPtr<T>::operator*() const
{
    return *m_object;
}

template<typename T>
T* WeakPtr<T>::operator->() const
{
    return m_object;
}

template<typename T>
WeakPtr<T>& WeakPtr<T>::operator=(const WeakPtr<T>& other)
{
    if(this != &other)
    {
        if(m_rc && m_rc->DecrementWeak() == 0)
        {
            delete m_rc;
            m_rc = nullptr;
        }
    
        m_object = other.m_object;
        m_rc     = other.m_rc;
        
        if(m_rc)
        {
            m_rc->IncrementWeak();
        }
    }
    
    return *this;
}

template<typename T>
bool WeakPtr<T>::operator==(const WeakPtr<T>& other) const
{
    return m_object == other.m_object;
}

template<typename T>
bool WeakPtr<T>::operator!=(const WeakPtr<T>& other) const
{
    return m_object != other.m_object;
}

template<typename T>
bool WeakPtr<T>::operator==(const T* const other) const
{
    return m_object == other;
}

template<typename T>
bool WeakPtr<T>::operator!=(const T* const other) const
{
    return m_object != other;
}

template<typename T>
bool WeakPtr<T>::Expired() const
{
    if(m_rc)
    {
        return m_rc->GetShared() == 0;
    }
    
    return true;
}

template<typename T>
T* WeakPtr<T>::Get() const
{
    if(Expired())
    {
        return nullptr;
    }
    else
    {
        return m_object;
    }
}

template<typename T>
template<typename Y>
WeakPtr<Y> WeakPtr<T>::As() const
{
    if(std::is_base_of<T, Y>::value)
    {
        // Down cast - dynamic_cast required.
        Y* object = dynamic_cast<Y*>(m_object);
        return (object) ? SharedPtr<Y>(object, m_rc) : SharedPtr<Y>();
    }
    else
    {
        // Try static_cast if not.
        return WeakPtr<Y>(static_cast<Y*>(m_object), m_rc);
    }
}
