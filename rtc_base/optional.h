/*
 *  Copyright 2004 The WebRTC Project Authors. All rights reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef _CPP_OPTIONAL_H_
#define _CPP_OPTIONAL_H_

//#if __cplusplus <= 201103L
#include <type_traits>
#include <exception>
namespace std {
    
template<typename T>
class optional
{
    using data_t = typename std::aligned_storage<sizeof(T), std::alignment_of<T>::value>::type;
public:
    optional() : m_hasInit(false) {}
    optional(const T& v)
    {
        Create(v);
    }
    
    optional(T&& v) : m_hasInit(false)
    {
        Create(std::move(v));
    }
    
    ~optional()
    {
        Destroy();
    }
    
    optional(const optional& other) : m_hasInit(false)
    {
        if (other.IsInit())
            Assign(other);
    }
    
    optional(optional&& other) : m_hasInit(false)
    {
        if (other.IsInit())
        {
            Assign(std::move(other));
            other.Destroy();
        }
    }
    
    optional& operator=(optional &&other)
    {
        Assign(std::move(other));
        return *this;
    }
    
    optional& operator=(const optional &other)
    {
        Assign(other);
        return *this;
    }
    
    template<class... Args>
    void emplace(Args&&... args)
    {
        Destroy();
        Create(std::forward<Args>(args)...);
    }
    
    bool IsInit() const { return m_hasInit; }
    
    explicit operator bool() const { return IsInit();
        
    }
    
    T& operator*()
    {
        return *((T*) (&m_data));
    }
    
    T const& operator*() const
    {
        if (IsInit())
        {
            return *((T*) (&m_data));
        }
        
        throw std::exception();
    }
    
    bool operator == (const optional<T>& rhs) const
    {
        return (!bool(*this)) != (!rhs) ? false : (!bool(*this) ? true : (*(*this)) == (*rhs));
    }
    
    bool operator < (const optional<T>& rhs) const
    {
        return !rhs ? false : (!bool(*this) ? true : (*(*this) < (*rhs)));
    }
    
    bool operator != (const optional<T>& rhs)
    {
        return !(*this == (rhs));
    }
private:
    template<class... Args>
    void Create(Args&&... args)
    {
        new (&m_data) T(std::forward<Args>
                        
                        (args)...);
        m_hasInit = true;
    }
    
    void Destroy()
    {
        if (m_hasInit)
        {
            m_hasInit = false;
            ((T*) (&m_data))->~T();
        }
    }
    
    void Assign(const optional& other)
    {
        if (other.IsInit())
        {
            Copy(other.m_data);
            m_hasInit = true;
        }
        else
        {
            Destroy();
        }
    }
    
    void Assign(optional&& other)
    {
        if (other.IsInit())
        {
            Move(std::move(other.m_data));
            m_hasInit = true;
            other.Destroy();
        }
        else
        {
            Destroy();
        }
    }
    
    void Move(data_t&& val)
    {
        Destroy();
        new (&m_data) T(std::move(*((T*)
                                    
                                    (&val))));
    }
    
    void Copy(const data_t& val)
    {
        Destroy();
        new (&m_data) T(*((T*) (&val)));
    }
    
private:
    bool m_hasInit;
    data_t m_data;
};
}
#endif /* optional_h */
