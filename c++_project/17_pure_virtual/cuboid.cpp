#include "cuboid.h"

int Cuboid::area()
{
	return 2 * (m_len * m_width + m_len * m_hight + m_width * m_hight);
}

int Cuboid::volume()
{
	return m_len * m_width * m_hight;
}