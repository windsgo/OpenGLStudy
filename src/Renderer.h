#pragma once

#include "GlobalFunctions.h"

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

class Renderer
{
private:
    /* data */

public:
    Renderer(/* args */);
    ~Renderer();

    void Clear() const;
    void Draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader) const;
};
