//
//  ReferenceCounter.h
//

#pragma once

class ReferenceCounter
{
public:
    ReferenceCounter () : m_sharedCounter(0), m_weakCounter(1) { }
    
    inline void          IncrementShared ()       { ++m_sharedCounter;        }
    inline void          IncrementWeak   ()       { ++m_weakCounter;          }
    inline unsigned long DecrementShared ()       { return --m_sharedCounter; if(m_sharedCounter == 0) --m_weakCounter;}
    inline unsigned long DecrementWeak   ()       { return --m_weakCounter;   }
    inline unsigned long GetShared       () const { return m_sharedCounter;   }
    inline unsigned long GetWeak         () const { return m_weakCounter;     }
    
private:
    unsigned long m_sharedCounter;
    unsigned long m_weakCounter;
};
