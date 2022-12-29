#pragma once
#include <vector>
#include "Renderer.h"
#include <type_traits>

struct VertexBufferElement
{
    VertexBufferElement(unsigned int _type, unsigned int _count, unsigned char _n)
        : type(_type), count(_count), normalized(_n) {}
    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    static unsigned int GetSizeOfType(unsigned int type)
    {
        switch (type)
        {
        case GL_FLOAT:
            return 4;
        case GL_UNSIGNED_INT:
            return 4;
        case GL_UNSIGNED_BYTE:
            return 1;
        }
        ASSERT(false);
        return 0;
    }
};

class VertexBufferLayout
{
private:
    std::vector<VertexBufferElement> m_Elements;
    unsigned int m_Stride;
#if defined(__GNUC__)
    // linux下类成员函数模板特化会被gcc阻挡，绕一下
    template <typename T>
    struct identity
    {
        using type = T;
    };
#endif

public:
    VertexBufferLayout()
        : m_Stride(0) {}

    template <typename T>
    void Push(unsigned int count)
    {
#if defined(__GNUC__)
        _push(identity<T>(), count);
#elif defined(_MSC_VER)
        ASSERT(false)
#endif
    }

#if defined(__GNUC__)
private:
    template <typename T>
    void _push(identity<T>, unsigned int count)
    {
        ASSERT(false);
    }

    void _push(identity<float>, unsigned int count)
    {
        m_Elements.emplace_back(GL_FLOAT, count, GL_FALSE);
        m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
    }

    void _push(identity<unsigned int>, unsigned int count)
    {
        m_Elements.emplace_back(GL_UNSIGNED_INT, count, GL_FALSE);
        m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
    }

    void _push(identity<unsigned char>, unsigned int count)
    {
        m_Elements.emplace_back(GL_UNSIGNED_BYTE, count, GL_TRUE);
        m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
    }
#elif defined(_MSC_VER)
public:
    // windows下可以用以下的特例方法
    template <>
    void Push<float>(unsigned int count)
    {
        m_Elements.emplace_back(GL_FLOAT, count, GL_FALSE);
        m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
    }

    template <>
    void Push<unsigned int>(unsigned int count)
    {
        m_Elements.emplace_back(GL_UNSIGNED_INT, count, GL_FALSE);
        m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
    }

    template <>
    void Push<unsigned char>(unsigned int count)
    {
        m_Elements.emplace_back(GL_UNSIGNED_BYTE, count, GL_TRUE);
        m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
    }
#endif

public:
    inline const std::vector<VertexBufferElement> &GetElements() const
    {
        return m_Elements;
    }
    inline unsigned int GetStride() const { return m_Stride; }
};