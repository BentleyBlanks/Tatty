#ifndef LIGHT_H
#define LIGHT_H

#include"color.h"
#include"T3Math.h"

#define LIGHTOFF 0
#define LIGHTON 1

//环境光 无穷远光源 聚光灯 点光源
#define LIGHT_AMBIENT 0
#define LIGHT_INFINITE 1
#define LIGHT_SPOTLIGHT 2
#define LIGHT_POINT 3

class light
{
public:	
	light()
	{}

	light(int32 state, int32 attribute,
		  Color ambient, Color diffuse, Color specular,
		  float32 kc, float32 kl, float32 kq,
		  T3Point4D pos, T3Vector4D dir)
		  : state(state), attribute(attribute), 
		  ambient(ambient), diffuse(diffuse), specular(specular), 
		  kc(kc), kl(kl), kq(kq), position(pos), direction(dir)
	{}

	void init(int32 state, int32 attribute,
		  Color ambient, Color diffuse, Color specular,
		  float32 kc, float32 kl, float32 kq,
		  T3Point4D pos, T3Vector4D dir)
	{
		this->state = state; this->attribute = attribute;
		this->ambient = ambient; this->diffuse = diffuse; this->specular = specular;
		this->kc = kc; this->kl = kl; this->kq = kq; 
		this->position = pos; this->direction = dir;
	}

	//光源是否开启状态
	int32 state;
	//光源属性
	int32 attribute;

	//环境光强度
	Color ambient;
	//散射光强度
	Color diffuse;
	//镜面反射光强度
	Color specular;

	//线性衰减因子
	float32 kc, kl, kq;
	//光源位置
	T3Point4D position;
	//光源的照射方向
	T3Vector4D direction;
};

#endif