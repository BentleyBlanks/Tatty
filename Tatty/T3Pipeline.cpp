#include"T3Pipeline.h"

//�ṩҪ�任������ ��Ⱦ�б��Լ��������ת��
void TransformRenderList(T3RenderListPtr rendList, T3Mat4X4Ptr mt, int32 coordinateSelect)
{
	switch(coordinateSelect)
	{
		//ֱ�Ӹ��ľֲ�����
	case TRANSFORM_LOCAL:
		for(int32 poly = 0; poly < rendList->numPolys; poly++)
		{
			//��õ�ǰ����ָ��Ķ����
			T3RenderPolyPtr currentPoly = rendList->polyPtr[poly];

			//���ڶ���ε�״̬�����ж�
			if((currentPoly == Null) ||
			   (!currentPoly->state & T3POLY_STATE_ACTIVE) ||
			   (currentPoly->state & T3POLY_STATE_CLIPPED) ||
			   (currentPoly->state & T3POLY_STATE_BACKFACE))
			   //����ת��ֱ�������˶����
			   continue;

			for(int32 vertex = 0; vertex < 3; vertex++)
			{
				T3Point4D result;

				//���о���任
				T3Mat_T3Vector4D_Mul_4X4(&currentPoly->vertexList[vertex], mt, &result);
				//��������浽�ֲ�������
				T3Vector4DCopy(&currentPoly->vertexList[vertex], &result);
			}
		}
		break;

		//����ת������������ת��
	case TRANSFORM_TRANS:
		for(int32 poly = 0; poly < rendList->numPolys; poly++)
		{
			//��õ�ǰ����ָ��Ķ����
			T3RenderPolyPtr currentPoly = rendList->polyPtr[poly];

			//���ڶ���ε�״̬�����ж�
			if((currentPoly == Null) ||
			   (!currentPoly->state & T3POLY_STATE_ACTIVE) ||
			   (currentPoly->state & T3POLY_STATE_CLIPPED) ||
			   (currentPoly->state & T3POLY_STATE_BACKFACE))
			   //����ת��ֱ�������˶����
			   continue;

			for(int32 vertex = 0; vertex < 3; vertex++)
			{
				T3Point4D result;

				//���о���任
				T3Mat_T3Vector4D_Mul_4X4(&currentPoly->transVertexList[vertex], mt, &result);
				//��������浽�ֲ�������
				T3Vector4DCopy(&currentPoly->transVertexList[vertex], &result);
			}
		}
		break;

		//��ģ��������б任�洢�ڱ任�б���
	case TRANSFORM_LOCAL_TO_TRANS:
		for(int32 poly = 0; poly < rendList->numPolys; poly++)
		{
			//��õ�ǰ����ָ��Ķ����
			T3RenderPolyPtr currentPoly = rendList->polyPtr[poly];

			//���ڶ���ε�״̬�����ж�
			if((currentPoly == Null) ||
			   (!currentPoly->state & T3POLY_STATE_ACTIVE) ||
			   (currentPoly->state & T3POLY_STATE_CLIPPED) ||
			   (currentPoly->state & T3POLY_STATE_BACKFACE))
			   //����ת��ֱ�������˶����
			   continue;

			for(int32 vertex = 0; vertex < 3; vertex++)
			{
				T3Point4D result;

				//���о���任
				T3Mat_T3Vector4D_Mul_4X4(&currentPoly->vertexList[vertex], mt, &result);
				//��������浽�ֲ�������
				T3Vector4DCopy(&currentPoly->transVertexList[vertex], &result);
			}
		}
		break;

	default:
		break;
	}
}

//��任��Ⱦ�б�ͬ�����Ƿ�Ҫת��ģ�͵ĳ���
void TransformT3Object(T3ObjectPtr obj, T3Mat4X4Ptr mt, int32 coordinateSelect, int32 transformOrientation)
{
	switch(coordinateSelect)
	{
	case TRANSFORM_LOCAL:
		for(int32 vertex = 0; vertex < obj->numVertices; vertex++)
		{
			T3Point4D result;

			//�Ծֲ�����ת��
			T3Mat_T3Vector4D_Mul_4X4(&obj->vertexList_Local[vertex], mt, &result);

			T3Vector4DCopy(&obj->vertexList_Local[vertex], &result);
		}
		break;

	case TRANSFORM_TRANS:
		for(int32 vertex = 0; vertex < obj->numVertices; vertex++)
		{
			T3Point4D result;

			//��ת�����������ж���ת��
			T3Mat_T3Vector4D_Mul_4X4(&obj->vertexList_Local[vertex], mt, &result);

			T3Vector4DCopy(&obj->vertexList_Trans[vertex], &result);
		}
		break;

	case TRANSFORM_LOCAL_TO_TRANS:
		for(int32 vertex = 0; vertex < obj->numVertices; vertex++)
		{
			T3Point4D result;

			//��ת�����������ж���ת��
			T3Mat_T3Vector4D_Mul_4X4(&obj->vertexList_Local[vertex], mt, &result);

			T3Vector4DCopy(&obj->vertexList_Trans[vertex], &result);
		}
		break;

	default:
		break;
	}

	//�ж��Ƿ���Ҫ��ת����
	if(transformOrientation)
	{
		//�������峯�������ļ�����
		T3Vector4D result;

		//uxת��
		T3Mat_T3Vector4D_Mul_4X4(&obj->ux, mt, &result);
		T3Vector4DCopy(&obj->ux, &result);

		//uyת��
		T3Mat_T3Vector4D_Mul_4X4(&obj->uy, mt, &result);
		T3Vector4DCopy(&obj->uy, &result);

		//uzת��
		T3Mat_T3Vector4D_Mul_4X4(&obj->uz, mt, &result);
		T3Vector4DCopy(&obj->uz, &result);
	}
}

//����ľֲ����굽���������ת��
void ModelToWorld_T3Object(T3ObjectPtr obj, int32 coodinateSelect)
{
	if(coodinateSelect == TRANSFORM_LOCAL_TO_TRANS)
	{
		for(int32 vertex = 0; vertex < obj->numVertices; vertex++)
		{
			//ֱ�Ӽ�������������б任
			T3Vector4DAdd(&obj->vertexList_Local[vertex], &obj->worldPosition, &obj->vertexList_Trans[vertex]);
		}
	}
	else
	{
		for(int32 vertex = 0; vertex < obj->numVertices; vertex++)
		{
			//ֱ�Ӽ�������������б任
			T3Vector4DAdd(&obj->vertexList_Local[vertex], &obj->worldPosition, &obj->vertexList_Trans[vertex]);
		}
	}
}

//��Ⱦ�б�ľֲ����굽���������ת��
void ModelToWorld_T3RenderList(T3RenderListPtr rendList, T3Point4DPtr worldPosition, int32 coodinateSelect)
{
	if(coodinateSelect == TRANSFORM_LOCAL_TO_TRANS)
	{
		for(int32 poly = 0; poly < rendList->numPolys; poly++)
		{
			//��õ�ǰ����ָ��Ķ����
			T3RenderPolyPtr currentPoly = rendList->polyPtr[poly];

			//���ڶ���ε�״̬�����ж�
			if((currentPoly == Null) ||
			   (!currentPoly->state & T3POLY_STATE_ACTIVE) ||
			   (currentPoly->state & T3POLY_STATE_CLIPPED) ||
			   (currentPoly->state & T3POLY_STATE_BACKFACE))
			   //����ת��ֱ�������˶����
			   continue;

			for(int32 vertex = 0; vertex < 3; vertex++)
			{
				T3Vector4DAdd(&currentPoly->vertexList[vertex], worldPosition, &currentPoly->transVertexList[vertex]);
			}
		}
	}
	else
	{
		for(int32 poly = 0; poly < rendList->numPolys; poly++)
		{
			//��õ�ǰ����ָ��Ķ����
			T3RenderPolyPtr currentPoly = rendList->polyPtr[poly];

			//���ڶ���ε�״̬�����ж�
			if((currentPoly == Null) ||
			   (!currentPoly->state & T3POLY_STATE_ACTIVE) ||
			   (currentPoly->state & T3POLY_STATE_CLIPPED) ||
			   (currentPoly->state & T3POLY_STATE_BACKFACE))
			   //����ת��ֱ�������˶����
			   continue;

			for(int32 vertex = 0; vertex < 3; vertex++)
			{
				T3Vector4DAdd(&currentPoly->transVertexList[vertex], worldPosition, &currentPoly->transVertexList[vertex]);
			}
		}
	}
}

//�������굽�������ı任
void WorldToCamera_T3Object(T3CameraPtr cam, T3ObjectPtr obj)
{
	//Ϊ�����ʼ���任����
	T3CameraBuildMat(cam, CAM_ROT_XYZ);

	for(int32 vertex = 0; vertex < obj->numVertices; vertex++)
	{
		T3Vector4D result;

		//������Ͼ���
		T3Mat_T3Vector4D_Mul_4X4(&obj->vertexList_Trans[vertex], &cam->mCam, &result);

		//������������ȥ
		T3Vector4DCopy(&obj->vertexList_Trans[vertex], &result);
	}
}

//��Ⱦ�б��������굽�������ı任
void WorldToCamera_T3RenderList(T3CameraPtr cam, T3RenderListPtr rendList)
{
	////Ϊ�����ʼ���任����
	//T3CameraBuildMat(cam, CAM_ROT_XYZ);

	for(int32 poly = 0; poly < rendList->numPolys; poly++)
	{
		T3RenderPolyPtr currentPoly = rendList->polyPtr[poly];

		//���ڶ���ε�״̬�����ж�
		if((currentPoly == Null) ||
		   !(currentPoly->state & T3POLY_STATE_ACTIVE) ||
		   (currentPoly->state & T3POLY_STATE_CLIPPED) ||
		   (currentPoly->state & T3POLY_STATE_BACKFACE))
		   //����ת��ֱ�������˶����
		   continue;

		for(int32 vertex = 0; vertex < 3; vertex++)
		{
			T3Point4D result;

			//����任
			T3Mat_T3Vector4D_Mul_4X4(&currentPoly->vertexList[vertex], &cam->mCam, &result);

			//�����������ȥ
			T3Vector4DCopy(&currentPoly->transVertexList[vertex], &result);
		}
	}
}

//�����������굽͸������
void CameraToPerspective_T3Object(T3ObjectPtr obj, T3CameraPtr cam)
{
	for(int32 vertex = 0; vertex < obj->numVertices; vertex++)
	{
		float32 z = obj->vertexList_Trans[vertex].z;

		//��ÿ�������͸�ӱ任
		obj->vertexList_Trans[vertex].x = cam->viewDistance * obj->vertexList_Trans[vertex].x / z;
		obj->vertexList_Trans[vertex].y = cam->viewDistance * obj->vertexList_Trans[vertex].y * cam->aspectRadio / z;

	}
}

//��Ⱦ�б��������굽͸������
void CameraToPerspective_T3RenderList(T3RenderListPtr rendList, T3CameraPtr cam)
{
	for(int32 poly = 0; poly < rendList->numPolys; poly++)
	{
		T3RenderPolyPtr currentPoly = rendList->polyPtr[poly];

		//���ڶ���ε�״̬�����ж�
		if((currentPoly == Null) ||
		   !(currentPoly->state & T3POLY_STATE_ACTIVE) ||
		   (currentPoly->state & T3POLY_STATE_CLIPPED) ||
		   (currentPoly->state & T3POLY_STATE_BACKFACE))
		   //����ת��ֱ�������˶����
		   continue;

		for(int32 vertex = 0; vertex < 3; vertex++)
		{
			float32 z = currentPoly->transVertexList[vertex].z;

			currentPoly->transVertexList[vertex].x = currentPoly->transVertexList[vertex].x * cam->viewDistance / z;
			currentPoly->transVertexList[vertex].y = currentPoly->transVertexList[vertex].y * cam->viewDistance * cam->aspectRadio / z;
		}
	}
}


//�����͸�����굽��Ļ�����
void PersepectiveToScreen_T3Object(T3ObjectPtr obj, T3CameraPtr cam)
{
	float32 alpha = (0.5 * cam->viewPortWidth - 0.5);
	float32 beta = (0.5 * cam->viewPortHeight - 0.5);
	for(int32 vertex = 0; vertex < obj->numVertices; vertex++)
	{
		//����ƽ�������������� ����תY��
		obj->vertexList_Trans[vertex].x = alpha + alpha * obj->vertexList_Trans[vertex].x;
		obj->vertexList_Trans[vertex].y = beta - beta * obj->vertexList_Trans[vertex].y;
	}
}


//��Ⱦ�б��͸�����굽��Ļ�����
void PersepectiveToScreen_T3RenderList(T3RenderListPtr rendList, T3CameraPtr cam)
{
	for(int32 poly = 0; poly < rendList->numPolys; poly++)
	{
		T3RenderPolyPtr currentPoly = rendList->polyPtr[poly];

		//���ڶ���ε�״̬�����ж�
		if((currentPoly == Null) ||
		   (!currentPoly->state & T3POLY_STATE_ACTIVE) ||
		   (currentPoly->state & T3POLY_STATE_CLIPPED) ||
		   (currentPoly->state & T3POLY_STATE_BACKFACE))
		   //����ת��ֱ�������˶����
		   continue;

		float32 alpha = (0.5 * cam->viewPortWidth - 0.5);
		float32 beta = (0.5 * cam->viewPortHeight - 0.5);

		for(int32 vertex = 0; vertex < 3; vertex++)
		{
			currentPoly->transVertexList[vertex].x = alpha + alpha * currentPoly->transVertexList[vertex].x;
			currentPoly->transVertexList[vertex].y = beta - beta * currentPoly->transVertexList[vertex].y;
		}
	}
}


//�����������Ⱦ�б�(���ö���β�����Ⱦ�б�)
void T3Object_InsertTo_T3RenderList(T3RenderListPtr rendList, T3ObjectPtr obj)
{
	//������״̬���ǿɼ�������
	if(!(obj->state & T3OBJECT_STATE_ACTIVE) ||
	   (obj->state & T3OBJECT_STATE_CULLED) ||
	   !(obj->state & T3OBJECT_STATE_VISIBLE))
	   return;

	for(int32 poly = 0; poly < obj->numPolygons; poly++)
	{
		//��ȡ��ǰ�����
		T3PolyPtr currentPoly = &obj->polyList[poly];

		//���ڶ���ε�״̬�����ж�
		if(!(currentPoly->state & T3POLY_STATE_ACTIVE) ||
		   (currentPoly->state & T3POLY_STATE_CLIPPED) ||
		   (currentPoly->state & T3POLY_STATE_BACKFACE))
		   //����ת��ֱ�������˶����
		   continue;

		//���涥���б�
		T3Point4DPtr vertexList_Old = currentPoly->vertexList;

		//����εĶ����б�������徭���任���Ķ�������
		currentPoly->vertexList = obj->vertexList_Trans;

		//���浱ǰ����εĻ���������ɫ
		//Color backupColor = currentPoly->color;

		//�������ε���Ⱦ�б�ʧ��
		if(!T3Poly_InsertTo_T3RenderList(rendList, currentPoly))
		{
			//���ܲ���ʧ�ܵ�����Ȼ��Ҫ��ԭ����ζ����б�
			currentPoly->vertexList = vertexList_Old;
			//currentPoly->color = backupColor;
			return;
		}

		//��ԭ����εĶ����б�	
		currentPoly->vertexList = vertexList_Old;
		//currentPoly->color = backupColor;
	}
}

//������β�����Ⱦ�б�
int32 T3Poly_InsertTo_T3RenderList(T3RenderListPtr rendList, T3PolyPtr poly)
{
	int32 num = rendList->numPolys;

	//����ǰ�б�Ķ���������������ֵ
	if(rendList->numPolys > T3RENDERLIST_MAX_POLY)
		return 0;

	//������numPolys��data��ptr����
	rendList->polyPtr[num] = &rendList->polyData[num];

	//������β��뵽��Ⱦ�б��numPolysλ��(��ǰ�ɲ���λ��)
	rendList->polyData[num].state = poly->state;

	//����������ɫ
	rendList->polyPtr[num]->color = poly->color;

	//�����������������ָ���Ķ��㿽������Ⱦ�б�Ķ����б��ת����Ķ����б���
	T3Vector4DCopy(&rendList->polyData[num].vertexList[0], &poly->vertexList[poly->vertex[0]]);
	T3Vector4DCopy(&rendList->polyData[num].vertexList[1], &poly->vertexList[poly->vertex[1]]);
	T3Vector4DCopy(&rendList->polyData[num].vertexList[2], &poly->vertexList[poly->vertex[2]]);

	T3Vector4DCopy(&rendList->polyData[num].transVertexList[0], &poly->vertexList[poly->vertex[0]]);
	T3Vector4DCopy(&rendList->polyData[num].transVertexList[1], &poly->vertexList[poly->vertex[1]]);
	T3Vector4DCopy(&rendList->polyData[num].transVertexList[2], &poly->vertexList[poly->vertex[2]]);

	//����ǰ�������������
	//�ж��Ƿ��ǵ�һ�������
	if(num == 0)
	{
		//˫���ÿ�
		rendList->polyData[num].previous = Null;
		rendList->polyData[num].next = Null;
	}
	else
	{
		//ָ����һ���洢�õĶ����
		rendList->polyData[num].previous = &rendList->polyData[num - 1];
		//ָ���¼���Ķ����
		rendList->polyData[num - 1].next = &rendList->polyData[num];
		//�ÿ�nextָ��
		rendList->polyData[num].next = Null;
	}

	//������+1
	rendList->numPolys++;

	return 1;
}


//��������
void RemoveBackfaces_T3RenderList(T3RenderListPtr rendList, T3CameraPtr cam)
{
	for(int32 poly = 0; poly < rendList->numPolys; poly++)
	{
		//��ȡ��ǰ�����
		T3RenderPolyPtr currentPoly = rendList->polyPtr[poly];

		//���ڶ���ε�״̬�����ж�
		if((&currentPoly == Null) ||
		   !(currentPoly->state & T3POLY_STATE_ACTIVE) ||
		   (currentPoly->state & T3POLY_STATE_CLIPPED) ||
		   (currentPoly->state & T3POLY_STATE_BACKFACE))
		   //����ת��ֱ�������˶����
		   continue;

		//u vΪ���������߰���˳ʱ�봴��������
		//nΪ������ƽ��ķ�����
		T3Vector4D u, v, n;
		//�������ڱ������ε�����
		T3Vector4DBuild(&currentPoly->transVertexList[0], &currentPoly->transVertexList[2], &u);
		T3Vector4DBuild(&currentPoly->transVertexList[0], &currentPoly->transVertexList[1], &v);

		//��ò���õ��淨��
		T3Vector4DCross(&u, &v, &n);

		//��������
		T3Vector4D view;
		T3Vector4DBuild(&cam->position, &currentPoly->transVertexList[0], &view);

		float32 dot;
		//�����������淨�ߵ��
		dot = T3Vector4DDot(&view, &n);

		if(dot <= 0.0)
		{
			SetBit(currentPoly->state, T3POLY_STATE_BACKFACE);
		}
	}
}


//������Ⱦ�б�
void T3RenderListReset(T3RenderListPtr rendList)
{
	rendList->numPolys = 0;
}


//����Ⱦ�б���й��ռ���
//light��ʾΪ��Դ�б� ��Ϊ����ͬʱ���ڵ��Դ�۹�Ƶȹ�Դ����
void Light_T3RenderList(T3RenderListPtr rendList, T3CameraPtr cam, light *light, int32 numLight)
{
	for(int32 poly = 0; poly < rendList->numPolys; poly++)
	{
		//��ȡ��ǰ�����
		T3RenderPolyPtr currentPoly = rendList->polyPtr[poly];

		//���ڶ���ε�״̬�����ж�
		if((&currentPoly == Null) ||
		   !(currentPoly->state & T3POLY_STATE_ACTIVE) ||
		   (currentPoly->state & T3POLY_STATE_CLIPPED) ||
		   (currentPoly->state & T3POLY_STATE_BACKFACE))
		   //����ת��ֱ�������˶����
		   continue;

		//�������ε�������������
		T3Point4D vertex0 = currentPoly->transVertexList[0],
			vertex1 = currentPoly->transVertexList[1],
			vertex2 = currentPoly->transVertexList[2];

		//��ȡ����λ���RGB������ɫ
		float32 baseR = currentPoly->color.getRed(),
			baseG = currentPoly->color.getGreen(),
			baseB = currentPoly->color.getBlue();

		//�ܵ����ֹ�Դ�����º�Ķ������ɫ
		float32 sumR = 0.0f, sumG = 0.0f, sumB = 0.0f;

		//������Դ
		for(int32 i = 0; i < numLight; i++)
		{
			if(light[i].state == LIGHTOFF)
				continue;

			//״̬��˼��
			if(light[i].attribute == LIGHT_AMBIENT)
			{
				sumR += ((light[i].ambient.getRed() * baseR) / 256);
				sumG += ((light[i].ambient.getGreen()  * baseG) / 256);
				sumB += ((light[i].ambient.getBlue()  * baseB) / 256);
			}
			if(light[i].attribute == LIGHT_POINT)
			{
				//���������ߵ������Լ��������淨��
				T3Vector4D u, v, n;
				T3Vector4DBuild(&vertex0, &vertex1, &u);
				T3Vector4DBuild(&vertex0, &vertex2, &v);

				//�����淨��
				T3Vector4DCross(&u, &v, &n);

				T3Vector4D sufaceToLight;
				//���浽��Դ������
				T3Vector4DBuild(&vertex0, &light[i].position, &sufaceToLight);

				//������浽��Դ����
				float32 distance = T3Vector4DLengthFast(&sufaceToLight);

				//���淨�߹�һ�����赥���оٳ���
				//ʹ�ÿ��ٿ����żӿ������ٶ�
				float32 nLength = T3Vector4DLengthFast(&n);

				//����λ������˵õ�cos(theta)
				float32 dotProduct = T3Vector4DDot(&sufaceToLight, &n);
				//���ǶȻ�0�����迼�ǹ���Ӱ��
				if(dotProduct > 0)
				{
					//I*C/(kc+kl*d+kq*d*d)
					float32 atten = light[i].kc + light[i].kl*distance + light[i].kq*distance*distance;

					//�൱�ڶ��淨��������������й�һ��
					float32 j = dotProduct / (atten*nLength*distance);
					sumR += light[i].diffuse.getRed()*baseR*j / 256.0;
					sumG += light[i].diffuse.getGreen()*baseG*j / 256.0;
					sumB += light[i].diffuse.getBlue()*baseB*j / 256.0;
				}
			}
			if(light[i].attribute == LIGHT_INFINITE)
			{
				//���������ߵ������Լ��������淨��
				T3Vector4D u, v, n;
				T3Vector4DBuild(&vertex0, &vertex1, &u);
				T3Vector4DBuild(&vertex0, &vertex2, &v);

				//�����淨��
				T3Vector4DCross(&u, &v, &n);

				//���淨�߹�һ�����赥���оٳ���
				//ʹ�ÿ��ٿ����żӿ������ٶ�
				float32 nLength = T3Vector4DLengthFast(&n);

				//����λ������˵õ�cos(theta)
				float32 dotProduct = T3Vector4DDot(&light->direction, &n);
				//���ǶȻ�0�����迼�ǹ���Ӱ��
				if(dotProduct > 0)
				{
					float32 j = dotProduct / nLength;
					sumR += light[i].diffuse.getRed()*baseR*j / 256.0;
					sumG += light[i].diffuse.getGreen()*baseG*j / 256.0;
					sumB += light[i].diffuse.getBlue()*baseB*j / 256.0;
				}
			}
			if(light[i].attribute == LIGHT_SPOTLIGHT)
			{
			}

		}

		//��ֹ��ɫ���
		if(sumR > 255.0)
			sumR = 255.0;
		if(sumG > 255.0)
			sumG = 255.0;
		if(sumB > 255.0)
			sumB = 255.0;

		Color col(sumR, sumG, sumB);
		//���ǵ�ǰ�������ɫ
		currentPoly->color = col;
	}
}

//���ڿ�������ıȽϺ���
int comparePolygonZ(const void* poly1, const void* poly2)
{
	//���ֵ
	float32 z1, z2;

	//����Ϊconst ֻ�����뵽��ʱ���������бȽ�
	//�Ƚ�voidת��ΪT3RenderPolyָ���ָ�� ��ͨ��ָ���ָ���ҵ�(ȡ����)���һ��T3RenderPoly��ָ��
	T3RenderPolyPtr tempPoly1 = *((T3RenderPolyPtr *) (poly1)), tempPoly2 = *((T3RenderPolyPtr *) (poly2));

	//�������ε�ƽ�����ֵ
	z1 = (float32)0.333333*(tempPoly1->transVertexList[0].z + tempPoly1->transVertexList[1].z + tempPoly1->transVertexList[2].z);
	z2 = (float32)0.333333*(tempPoly2->transVertexList[0].z + tempPoly2->transVertexList[1].z + tempPoly2->transVertexList[2].z);

	//zֵ�������� ����������ǰ��
	if(z1 > z2)
		return -1;
	else if(z1 < z2)
		return 1;
	else
		return 0;
}

//�Ը�������Ⱦ�б��������
void SortRenderList(T3RenderListPtr rendList)
{
	
	qsort((void *) rendList->polyPtr,  //�������Ŀ������
		  rendList->numPolys,	//Ŀ�����鳤��
		  sizeof(T3RenderPolyPtr),		//������Ԫ�س���
		  comparePolygonZ);		//�ȽϺ���
}