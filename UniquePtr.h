//
//  UniquePtr.h
//

#pragma once

template<class T>
class UniquePtr
{
public:
    explicit UniquePtr (T* object) : m_object(object) {}
    ~UniquePtr() { delete m_object; }
    
    // Auto generated methods by compiler. Removed.
    UniquePtr(const UniquePtr&)             = delete;
    UniquePtr& operator= (const UniquePtr&) = delete;
    
    T* operator-> () const { return m_object;            }
    T& operator*  () const { return *m_object;           }
    operator bool () const { return m_object != nullptr; }
    T* Get        () const { return m_object;            }
    bool IsValid  () const { return m_object != nullptr; }
    
    T* Release ()
    {
        T* result = nullptr;
        std::swap(result, m_object);
        return result;
    }
    
private:
    T* m_object;
};
