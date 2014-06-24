#include"vertex3.h"


vertex3::vertex3(float32 &x, float32 &y, float32 &z, float32 &w, Color &color)
{
	point4f *point = new point4f(x, y, z, w);

	position = *point;

	this->color = color;
}
