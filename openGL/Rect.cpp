#include "Rect.h"

#include "glut.h"

Rect::Rect(vec2 const& _position, vec2 const& _size)
	:m_position(_position)
	, m_size(_size) {}

void Rect::draw() {
	glRectf(
		m_position.x, 
		m_position.y,
		m_position.x + m_size.x, 
		m_position.y + m_size.y);

}

bool Rect::intersect(vec2 const& _point) {
	if (
		(_point.x >= m_position.x)
		&& (_point.x < m_position.x + m_size.x)
		&& (_point.y >= m_position.y)
		&& (_point.y < m_position.y + m_size.y)
		)
		return true;
	return false;
}

bool Rect::intersect(Rect const& _rect) {
	if (
		(m_position.x + m_size.x >= _rect.m_position.x)
		&& (m_position.x < _rect.m_position.x + _rect.m_size.x)
		&& (m_position.y + m_size.y >= _rect.m_position.y)
		&& (m_position.y < _rect.m_position.y + _rect.m_size.y)
		)
		return true;
	return false;
}