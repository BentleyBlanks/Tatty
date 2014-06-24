#ifndef LIGHT_H
#define LIGHT_H

#include"color.h"
#include"T3Math.h"

#define LIGHTOFF 0
#define LIGHTON 1

//������ ����Զ��Դ �۹�� ���Դ
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

	//��Դ�Ƿ���״̬
	int32 state;
	//��Դ����
	int32 attribute;

	//������ǿ��
	Color ambient;
	//ɢ���ǿ��
	Color diffuse;
	//���淴���ǿ��
	Color specular;

	//����˥������
	float32 kc, kl, kq;
	//��Դλ��
	T3Point4D position;
	//��Դ�����䷽��
	T3Vector4D direction;
};

#endif