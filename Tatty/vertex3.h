#ifndef VERTEX3_H
#define VERTEX3_H

#include"types.h"
#include"vector4.h"
#include"color.h"

class vertex3
{
public:
	vertex3()
	{
	};

	vertex3(float32 &x, float32 &y, float32 &z, float32 &w, Color &color);

	vertex3(point4f &pos, Color &color) : position(pos), color(color)
	{
	}


	point4f position;

	//һ���������һ����ɫ��Ϣ��һ��λ����Ϣ
	Color color;
};


#endif