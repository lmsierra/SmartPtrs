//
//  SmartPointers.h
//

#pragma once

#include "ReferenceCounter.h"

template<typename T> class WeakPtr;
template<typename T>
class SharedPtr
{
public:
    SharedPtr();
    SharedPtr(T* pointer);
    SharedPtr(T* pointer, ReferenceCounter* rc);
    SharedPtr(const SharedPtr<T>& p);
    SharedPtr(const WeakPtr<T>& p);
    ~SharedPtr();
    
    T& operator*()                                const;
    T* operator->()                               const;
    SharedPtr<T>& operator=(const SharedPtr<T>& p);
    bool operator==(const SharedPtr<T>& p)         const;
    bool operator!=(const SharedPtr<T>& p)         const;
    bool operator==(const T* const p)             const;
    bool operator!=(const T* const p)             const;
    bool operator!()                              const;
    
    T* Get()                 const { return m_object; }
    explicit operator bool() const { return m_object; }

    bool IsValid()           const { return m_object != nullptr; }
    void Reset();
    
    template<typename Y>
    SharedPtr<Y> As() const;

private:
    T* m_object;
    ReferenceCounter* m_rc;
    
    friend class WeakPtr<T>;
};

template<typename T>
SharedPtr<T>::SharedPtr() : m_object(nullptr), m_rc(nullptr)
{

}


template<typename T>
SharedPtr<T>::SharedPtr(T* pointer) : m_object(pointer), m_rc(nullptr)
{
    m_rc = new ReferenceCounter();
    m_rc->IncrementShared();
}


template<typename T>
SharedPtr<T>::SharedPtr(T* pointer, ReferenceCounter* rc) : m_object(pointer), m_rc(rc)
{
    if(m_rc)
    {
        m_rc->IncrementShared();
    }
}

template<typename T>
SharedPtr<T>::SharedPtr(const SharedPtr<T>& pointer) : m_object(pointer.m_object), m_rc(pointer.m_rc)
{
    if(m_rc)
    {
        m_rc->IncrementShared();
    }
}

template<typename T>
SharedPtr<T>::SharedPtr(const WeakPtr<T>& pointer) : m_object(pointer.m_object), m_rc(pointer.m_rc)
{
    if(!m_rc)
    {
        m_rc = new ReferenceCounter();
    }
    m_rc->IncrementShared();
}

template<typename T>
SharedPtr<T>::~SharedPtr()
{
    if(m_rc && m_rc->DecrementShared() == 0)
    {
        delete m_object;
        m_object = nullptr;
    }
    
    if(m_rc && m_rc->GetWeak() == 0)
    {
        delete m_rc;
        m_rc = nullptr;
    }
}


template<typename T>
T& SharedPtr<T>::operator*() const
{
    return *m_object;
}

template<typename T>
T* SharedPtr<T>::operator->() const
{
    return m_object;
}

template<typename T>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<T>& other)
{
    if(*this != other)
    {
        if (m_rc && m_rc->DecrementShared() == 0)
        {
            delete m_object;
            m_object = nullptr;
        }
        
        if(m_rc && m_rc->GetWeak() == 0)
        {
            delete m_rc;
            m_rc = nullptr;
        }
        
        m_object = other.m_object;
        m_rc     = other.m_rc;
        
        m_rc->IncrementShared();
    }
    
    return *this;
}

template<typename T>
bool SharedPtr<T>::operator==(const SharedPtr<T>& other) const
{
    return m_object == other.m_object;
}

template<typename T>
bool SharedPtr<T>::operator!=(const SharedPtr<T>& other) const
{
    return m_object != other.m_object;
}

template<typename T>
bool SharedPtr<T>::operator==(const T* const other) const
{
    return m_object == other;
}

template<typename T>
bool SharedPtr<T>::operator!=(const T* const other) const
{
    return m_object != other;
}

template<typename T>
bool SharedPtr<T>::operator!() const
{
    return !m_object;
}

template<typename T>
void SharedPtr<T>::Reset()
{
    if (m_rc->DecrementShared() == 0)
    {
        delete m_object;
    }
    
    m_object = nullptr;
    m_rc     = nullptr;
}

template<typename T>
template<typename Y>
SharedPtr<Y> SharedPtr<T>::As() const
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
        return SharedPtr<Y>(static_cast<Y*>(m_object), m_rc);
    }
}
