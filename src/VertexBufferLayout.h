#pragma once

#include <GL/glew.h>
#include <vector>
#include <iostream>

#include "Renderer.h"



struct VertexBufferElement {
	unsigned int count;
	unsigned int type;
	unsigned int normalized;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch(type) {
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
public:
	VertexBufferLayout() : m_Stride(0) {}
	//~VertexBufferLayout();
	
	void Push(unsigned int gl_type, unsigned int count)
	{
		//Because of templates not working the same in vs and in gcc
		//then i pass GL_type as parameter.
		//TO DO: refacotor to get a type instead of gl_type
		//ewentualnie przy kożystaniu z nietypowych typów
		//np. vec2 to zdrobić #define i indefined inta zrobić
		m_Elements.push_back({count, gl_type, GL_FALSE});
		m_Stride += VertexBufferElement::GetSizeOfType(gl_type)*count;
	}

	inline const std::vector<VertexBufferElement> GetElements() const
	{
		return m_Elements;
	};

	inline unsigned int GetStride() const
	{
		return m_Stride;
	};
	
};