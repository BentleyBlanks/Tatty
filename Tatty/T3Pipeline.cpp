#include"T3Pipeline.h"

//提供要变换的坐标 渲染列表以及矩阵进行转换
void TransformRenderList(T3RenderListPtr rendList, T3Mat4X4Ptr mt, int32 coordinateSelect)
{
	switch(coordinateSelect)
	{
		//直接更改局部坐标
	case TRANSFORM_LOCAL:
		for(int32 poly = 0; poly < rendList->numPolys; poly++)
		{
			//获得当前索引指向的多边形
			T3RenderPolyPtr currentPoly = rendList->polyPtr[poly];

			//对于多边形的状态进行判断
			if((currentPoly == Null) ||
			   (!currentPoly->state & T3POLY_STATE_ACTIVE) ||
			   (currentPoly->state & T3POLY_STATE_CLIPPED) ||
			   (currentPoly->state & T3POLY_STATE_BACKFACE))
			   //无需转换直接跳过此多边形
			   continue;

			for(int32 vertex = 0; vertex < 3; vertex++)
			{
				T3Point4D result;

				//进行矩阵变换
				T3Mat_T3Vector4D_Mul_4X4(&currentPoly->vertexList[vertex], mt, &result);
				//将结果保存到局部坐标中
				T3Vector4DCopy(&currentPoly->vertexList[vertex], &result);
			}
		}
		break;

		//对于转换后的坐标进行转换
	case TRANSFORM_TRANS:
		for(int32 poly = 0; poly < rendList->numPolys; poly++)
		{
			//获得当前索引指向的多边形
			T3RenderPolyPtr currentPoly = rendList->polyPtr[poly];

			//对于多边形的状态进行判断
			if((currentPoly == Null) ||
			   (!currentPoly->state & T3POLY_STATE_ACTIVE) ||
			   (currentPoly->state & T3POLY_STATE_CLIPPED) ||
			   (currentPoly->state & T3POLY_STATE_BACKFACE))
			   //无需转换直接跳过此多边形
			   continue;

			for(int32 vertex = 0; vertex < 3; vertex++)
			{
				T3Point4D result;

				//进行矩阵变换
				T3Mat_T3Vector4D_Mul_4X4(&currentPoly->transVertexList[vertex], mt, &result);
				//将结果保存到局部坐标中
				T3Vector4DCopy(&currentPoly->transVertexList[vertex], &result);
			}
		}
		break;

		//对模型坐标进行变换存储在变换列表内
	case TRANSFORM_LOCAL_TO_TRANS:
		for(int32 poly = 0; poly < rendList->numPolys; poly++)
		{
			//获得当前索引指向的多边形
			T3RenderPolyPtr currentPoly = rendList->polyPtr[poly];

			//对于多边形的状态进行判断
			if((currentPoly == Null) ||
			   (!currentPoly->state & T3POLY_STATE_ACTIVE) ||
			   (currentPoly->state & T3POLY_STATE_CLIPPED) ||
			   (currentPoly->state & T3POLY_STATE_BACKFACE))
			   //无需转换直接跳过此多边形
			   continue;

			for(int32 vertex = 0; vertex < 3; vertex++)
			{
				T3Point4D result;

				//进行矩阵变换
				T3Mat_T3Vector4D_Mul_4X4(&currentPoly->vertexList[vertex], mt, &result);
				//将结果保存到局部坐标中
				T3Vector4DCopy(&currentPoly->transVertexList[vertex], &result);
			}
		}
		break;

	default:
		break;
	}
}

//与变换渲染列表不同在于是否要转换模型的朝向
void TransformT3Object(T3ObjectPtr obj, T3Mat4X4Ptr mt, int32 coordinateSelect, int32 transformOrientation)
{
	switch(coordinateSelect)
	{
	case TRANSFORM_LOCAL:
		for(int32 vertex = 0; vertex < obj->numVertices; vertex++)
		{
			T3Point4D result;

			//对局部坐标转换
			T3Mat_T3Vector4D_Mul_4X4(&obj->vertexList_Local[vertex], mt, &result);

			T3Vector4DCopy(&obj->vertexList_Local[vertex], &result);
		}
		break;

	case TRANSFORM_TRANS:
		for(int32 vertex = 0; vertex < obj->numVertices; vertex++)
		{
			T3Point4D result;

			//对转换后的坐标进行二次转换
			T3Mat_T3Vector4D_Mul_4X4(&obj->vertexList_Local[vertex], mt, &result);

			T3Vector4DCopy(&obj->vertexList_Trans[vertex], &result);
		}
		break;

	case TRANSFORM_LOCAL_TO_TRANS:
		for(int32 vertex = 0; vertex < obj->numVertices; vertex++)
		{
			T3Point4D result;

			//对转换后的坐标进行二次转换
			T3Mat_T3Vector4D_Mul_4X4(&obj->vertexList_Local[vertex], mt, &result);

			T3Vector4DCopy(&obj->vertexList_Trans[vertex], &result);
		}
		break;

	default:
		break;
	}

	//判断是否需要旋转朝向
	if(transformOrientation)
	{
		//保存物体朝向向量的计算结果
		T3Vector4D result;

		//ux转换
		T3Mat_T3Vector4D_Mul_4X4(&obj->ux, mt, &result);
		T3Vector4DCopy(&obj->ux, &result);

		//uy转换
		T3Mat_T3Vector4D_Mul_4X4(&obj->uy, mt, &result);
		T3Vector4DCopy(&obj->uy, &result);

		//uz转换
		T3Mat_T3Vector4D_Mul_4X4(&obj->uz, mt, &result);
		T3Vector4DCopy(&obj->uz, &result);
	}
}

//物体的局部坐标到世界坐标的转换
void ModelToWorld_T3Object(T3ObjectPtr obj, int32 coodinateSelect)
{
	if(coodinateSelect == TRANSFORM_LOCAL_TO_TRANS)
	{
		for(int32 vertex = 0; vertex < obj->numVertices; vertex++)
		{
			//直接加上世界坐标进行变换
			T3Vector4DAdd(&obj->vertexList_Local[vertex], &obj->worldPosition, &obj->vertexList_Trans[vertex]);
		}
	}
	else
	{
		for(int32 vertex = 0; vertex < obj->numVertices; vertex++)
		{
			//直接加上世界坐标进行变换
			T3Vector4DAdd(&obj->vertexList_Local[vertex], &obj->worldPosition, &obj->vertexList_Trans[vertex]);
		}
	}
}

//渲染列表的局部坐标到世界坐标的转换
void ModelToWorld_T3RenderList(T3RenderListPtr rendList, T3Point4DPtr worldPosition, int32 coodinateSelect)
{
	if(coodinateSelect == TRANSFORM_LOCAL_TO_TRANS)
	{
		for(int32 poly = 0; poly < rendList->numPolys; poly++)
		{
			//获得当前索引指向的多边形
			T3RenderPolyPtr currentPoly = rendList->polyPtr[poly];

			//对于多边形的状态进行判断
			if((currentPoly == Null) ||
			   (!currentPoly->state & T3POLY_STATE_ACTIVE) ||
			   (currentPoly->state & T3POLY_STATE_CLIPPED) ||
			   (currentPoly->state & T3POLY_STATE_BACKFACE))
			   //无需转换直接跳过此多边形
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
			//获得当前索引指向的多边形
			T3RenderPolyPtr currentPoly = rendList->polyPtr[poly];

			//对于多边形的状态进行判断
			if((currentPoly == Null) ||
			   (!currentPoly->state & T3POLY_STATE_ACTIVE) ||
			   (currentPoly->state & T3POLY_STATE_CLIPPED) ||
			   (currentPoly->state & T3POLY_STATE_BACKFACE))
			   //无需转换直接跳过此多边形
			   continue;

			for(int32 vertex = 0; vertex < 3; vertex++)
			{
				T3Vector4DAdd(&currentPoly->transVertexList[vertex], worldPosition, &currentPoly->transVertexList[vertex]);
			}
		}
	}
}

//世界坐标到相机坐标的变换
void WorldToCamera_T3Object(T3CameraPtr cam, T3ObjectPtr obj)
{
	//为相机初始化变换矩阵
	T3CameraBuildMat(cam, CAM_ROT_XYZ);

	for(int32 vertex = 0; vertex < obj->numVertices; vertex++)
	{
		T3Vector4D result;

		//计算组合矩阵
		T3Mat_T3Vector4D_Mul_4X4(&obj->vertexList_Trans[vertex], &cam->mCam, &result);

		//计算结果拷贝回去
		T3Vector4DCopy(&obj->vertexList_Trans[vertex], &result);
	}
}

//渲染列表世界坐标到相机坐标的变换
void WorldToCamera_T3RenderList(T3CameraPtr cam, T3RenderListPtr rendList)
{
	////为相机初始化变换矩阵
	//T3CameraBuildMat(cam, CAM_ROT_XYZ);

	for(int32 poly = 0; poly < rendList->numPolys; poly++)
	{
		T3RenderPolyPtr currentPoly = rendList->polyPtr[poly];

		//对于多边形的状态进行判断
		if((currentPoly == Null) ||
		   !(currentPoly->state & T3POLY_STATE_ACTIVE) ||
		   (currentPoly->state & T3POLY_STATE_CLIPPED) ||
		   (currentPoly->state & T3POLY_STATE_BACKFACE))
		   //无需转换直接跳过此多边形
		   continue;

		for(int32 vertex = 0; vertex < 3; vertex++)
		{
			T3Point4D result;

			//坐标变换
			T3Mat_T3Vector4D_Mul_4X4(&currentPoly->vertexList[vertex], &cam->mCam, &result);

			//将结果拷贝回去
			T3Vector4DCopy(&currentPoly->transVertexList[vertex], &result);
		}
	}
}

//物体的相机坐标到透视坐标
void CameraToPerspective_T3Object(T3ObjectPtr obj, T3CameraPtr cam)
{
	for(int32 vertex = 0; vertex < obj->numVertices; vertex++)
	{
		float32 z = obj->vertexList_Trans[vertex].z;

		//对每个点进行透视变换
		obj->vertexList_Trans[vertex].x = cam->viewDistance * obj->vertexList_Trans[vertex].x / z;
		obj->vertexList_Trans[vertex].y = cam->viewDistance * obj->vertexList_Trans[vertex].y * cam->aspectRadio / z;

	}
}

//渲染列表的相机坐标到透视坐标
void CameraToPerspective_T3RenderList(T3RenderListPtr rendList, T3CameraPtr cam)
{
	for(int32 poly = 0; poly < rendList->numPolys; poly++)
	{
		T3RenderPolyPtr currentPoly = rendList->polyPtr[poly];

		//对于多边形的状态进行判断
		if((currentPoly == Null) ||
		   !(currentPoly->state & T3POLY_STATE_ACTIVE) ||
		   (currentPoly->state & T3POLY_STATE_CLIPPED) ||
		   (currentPoly->state & T3POLY_STATE_BACKFACE))
		   //无需转换直接跳过此多边形
		   continue;

		for(int32 vertex = 0; vertex < 3; vertex++)
		{
			float32 z = currentPoly->transVertexList[vertex].z;

			currentPoly->transVertexList[vertex].x = currentPoly->transVertexList[vertex].x * cam->viewDistance / z;
			currentPoly->transVertexList[vertex].y = currentPoly->transVertexList[vertex].y * cam->viewDistance * cam->aspectRadio / z;
		}
	}
}


//物体的透视坐标到屏幕坐标的
void PersepectiveToScreen_T3Object(T3ObjectPtr obj, T3CameraPtr cam)
{
	float32 alpha = (0.5 * cam->viewPortWidth - 0.5);
	float32 beta = (0.5 * cam->viewPortHeight - 0.5);
	for(int32 vertex = 0; vertex < obj->numVertices; vertex++)
	{
		//对视平面的坐标进行缩放 并反转Y轴
		obj->vertexList_Trans[vertex].x = alpha + alpha * obj->vertexList_Trans[vertex].x;
		obj->vertexList_Trans[vertex].y = beta - beta * obj->vertexList_Trans[vertex].y;
	}
}


//渲染列表的透视坐标到屏幕坐标的
void PersepectiveToScreen_T3RenderList(T3RenderListPtr rendList, T3CameraPtr cam)
{
	for(int32 poly = 0; poly < rendList->numPolys; poly++)
	{
		T3RenderPolyPtr currentPoly = rendList->polyPtr[poly];

		//对于多边形的状态进行判断
		if((currentPoly == Null) ||
		   (!currentPoly->state & T3POLY_STATE_ACTIVE) ||
		   (currentPoly->state & T3POLY_STATE_CLIPPED) ||
		   (currentPoly->state & T3POLY_STATE_BACKFACE))
		   //无需转换直接跳过此多边形
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


//将物体插入渲染列表(调用多边形插入渲染列表)
void T3Object_InsertTo_T3RenderList(T3RenderListPtr rendList, T3ObjectPtr obj)
{
	//若物体状态并非可见则跳过
	if(!(obj->state & T3OBJECT_STATE_ACTIVE) ||
	   (obj->state & T3OBJECT_STATE_CULLED) ||
	   !(obj->state & T3OBJECT_STATE_VISIBLE))
	   return;

	for(int32 poly = 0; poly < obj->numPolygons; poly++)
	{
		//获取当前多边形
		T3PolyPtr currentPoly = &obj->polyList[poly];

		//对于多边形的状态进行判断
		if(!(currentPoly->state & T3POLY_STATE_ACTIVE) ||
		   (currentPoly->state & T3POLY_STATE_CLIPPED) ||
		   (currentPoly->state & T3POLY_STATE_BACKFACE))
		   //无需转换直接跳过此多边形
		   continue;

		//保存顶点列表
		T3Point4DPtr vertexList_Old = currentPoly->vertexList;

		//多边形的顶点列表就是物体经过变换过的顶点数组
		currentPoly->vertexList = obj->vertexList_Trans;

		//保存当前多边形的基础表面颜色
		//Color backupColor = currentPoly->color;

		//插入多边形到渲染列表失败
		if(!T3Poly_InsertTo_T3RenderList(rendList, currentPoly))
		{
			//尽管插入失败但是仍然需要还原多边形顶点列表
			currentPoly->vertexList = vertexList_Old;
			//currentPoly->color = backupColor;
			return;
		}

		//还原多边形的顶点列表	
		currentPoly->vertexList = vertexList_Old;
		//currentPoly->color = backupColor;
	}
}

//将多边形插入渲染列表
int32 T3Poly_InsertTo_T3RenderList(T3RenderListPtr rendList, T3PolyPtr poly)
{
	int32 num = rendList->numPolys;

	//若当前列表的多边形数量超出最大值
	if(rendList->numPolys > T3RENDERLIST_MAX_POLY)
		return 0;

	//连接在numPolys上data和ptr部分
	rendList->polyPtr[num] = &rendList->polyData[num];

	//将多边形插入到渲染列表的numPolys位置(当前可插入位置)
	rendList->polyData[num].state = poly->state;

	//保存多边形颜色
	rendList->polyPtr[num]->color = poly->color;

	//将多边形三角形索引指定的顶点拷贝到渲染列表的顶点列表和转换后的顶点列表中
	T3Vector4DCopy(&rendList->polyData[num].vertexList[0], &poly->vertexList[poly->vertex[0]]);
	T3Vector4DCopy(&rendList->polyData[num].vertexList[1], &poly->vertexList[poly->vertex[1]]);
	T3Vector4DCopy(&rendList->polyData[num].vertexList[2], &poly->vertexList[poly->vertex[2]]);

	T3Vector4DCopy(&rendList->polyData[num].transVertexList[0], &poly->vertexList[poly->vertex[0]]);
	T3Vector4DCopy(&rendList->polyData[num].transVertexList[1], &poly->vertexList[poly->vertex[1]]);
	T3Vector4DCopy(&rendList->polyData[num].transVertexList[2], &poly->vertexList[poly->vertex[2]]);

	//将当前多边形连接链表
	//判断是否是第一个多边形
	if(num == 0)
	{
		//双向置空
		rendList->polyData[num].previous = Null;
		rendList->polyData[num].next = Null;
	}
	else
	{
		//指向上一个存储好的多边形
		rendList->polyData[num].previous = &rendList->polyData[num - 1];
		//指向新加入的多边形
		rendList->polyData[num - 1].next = &rendList->polyData[num];
		//置空next指针
		rendList->polyData[num].next = Null;
	}

	//链表长度+1
	rendList->numPolys++;

	return 1;
}


//背面消除
void RemoveBackfaces_T3RenderList(T3RenderListPtr rendList, T3CameraPtr cam)
{
	for(int32 poly = 0; poly < rendList->numPolys; poly++)
	{
		//获取当前多边形
		T3RenderPolyPtr currentPoly = rendList->polyPtr[poly];

		//对于多边形的状态进行判断
		if((&currentPoly == Null) ||
		   !(currentPoly->state & T3POLY_STATE_ACTIVE) ||
		   (currentPoly->state & T3POLY_STATE_CLIPPED) ||
		   (currentPoly->state & T3POLY_STATE_BACKFACE))
		   //无需转换直接跳过此多边形
		   continue;

		//u v为三角形两边按照顺时针创建的向量
		//n为三角形平面的法向量
		T3Vector4D u, v, n;
		//创建基于本三角形的向量
		T3Vector4DBuild(&currentPoly->transVertexList[0], &currentPoly->transVertexList[2], &u);
		T3Vector4DBuild(&currentPoly->transVertexList[0], &currentPoly->transVertexList[1], &v);

		//获得叉积得到面法线
		T3Vector4DCross(&u, &v, &n);

		//视线向量
		T3Vector4D view;
		T3Vector4DBuild(&cam->position, &currentPoly->transVertexList[0], &view);

		float32 dot;
		//视线向量与面法线点积
		dot = T3Vector4DDot(&view, &n);

		if(dot <= 0.0)
		{
			SetBit(currentPoly->state, T3POLY_STATE_BACKFACE);
		}
	}
}


//重置渲染列表
void T3RenderListReset(T3RenderListPtr rendList)
{
	rendList->numPolys = 0;
}


//对渲染列表进行光照计算
//light表示为光源列表 因为可能同时存在点光源聚光灯等光源类型
void Light_T3RenderList(T3RenderListPtr rendList, T3CameraPtr cam, light *light, int32 numLight)
{
	for(int32 poly = 0; poly < rendList->numPolys; poly++)
	{
		//获取当前多边形
		T3RenderPolyPtr currentPoly = rendList->polyPtr[poly];

		//对于多边形的状态进行判断
		if((&currentPoly == Null) ||
		   !(currentPoly->state & T3POLY_STATE_ACTIVE) ||
		   (currentPoly->state & T3POLY_STATE_CLIPPED) ||
		   (currentPoly->state & T3POLY_STATE_BACKFACE))
		   //无需转换直接跳过此多边形
		   continue;

		//保存多边形的三个顶点坐标
		T3Point4D vertex0 = currentPoly->transVertexList[0],
			vertex1 = currentPoly->transVertexList[1],
			vertex2 = currentPoly->transVertexList[2];

		//获取多边形基底RGB分量颜色
		float32 baseR = currentPoly->color.getRed(),
			baseG = currentPoly->color.getGreen(),
			baseB = currentPoly->color.getBlue();

		//受到各种光源照射下后的多边形颜色
		float32 sumR = 0.0f, sumG = 0.0f, sumB = 0.0f;

		//遍历光源
		for(int32 i = 0; i < numLight; i++)
		{
			if(light[i].state == LIGHTOFF)
				continue;

			//状态机思想
			if(light[i].attribute == LIGHT_AMBIENT)
			{
				sumR += ((light[i].ambient.getRed() * baseR) / 256);
				sumG += ((light[i].ambient.getGreen()  * baseG) / 256);
				sumB += ((light[i].ambient.getBlue()  * baseB) / 256);
			}
			if(light[i].attribute == LIGHT_POINT)
			{
				//三角形两边的向量以及三角形面法线
				T3Vector4D u, v, n;
				T3Vector4DBuild(&vertex0, &vertex1, &u);
				T3Vector4DBuild(&vertex0, &vertex2, &v);

				//计算面法线
				T3Vector4DCross(&u, &v, &n);

				T3Vector4D sufaceToLight;
				//表面到光源的向量
				T3Vector4DBuild(&vertex0, &light[i].position, &sufaceToLight);

				//计算表面到光源距离
				float32 distance = T3Vector4DLengthFast(&sufaceToLight);

				//将面法线归一化步骤单独列举出来
				//使用快速开根号加快运算速度
				float32 nLength = T3Vector4DLengthFast(&n);

				//两单位向量相乘得到cos(theta)
				float32 dotProduct = T3Vector4DDot(&sufaceToLight, &n);
				//负角度或0度无需考虑光照影响
				if(dotProduct > 0)
				{
					//I*C/(kc+kl*d+kq*d*d)
					float32 atten = light[i].kc + light[i].kl*distance + light[i].kq*distance*distance;

					//相当于对面法线与距离向量进行归一化
					float32 j = dotProduct / (atten*nLength*distance);
					sumR += light[i].diffuse.getRed()*baseR*j / 256.0;
					sumG += light[i].diffuse.getGreen()*baseG*j / 256.0;
					sumB += light[i].diffuse.getBlue()*baseB*j / 256.0;
				}
			}
			if(light[i].attribute == LIGHT_INFINITE)
			{
				//三角形两边的向量以及三角形面法线
				T3Vector4D u, v, n;
				T3Vector4DBuild(&vertex0, &vertex1, &u);
				T3Vector4DBuild(&vertex0, &vertex2, &v);

				//计算面法线
				T3Vector4DCross(&u, &v, &n);

				//将面法线归一化步骤单独列举出来
				//使用快速开根号加快运算速度
				float32 nLength = T3Vector4DLengthFast(&n);

				//两单位向量相乘得到cos(theta)
				float32 dotProduct = T3Vector4DDot(&light->direction, &n);
				//负角度或0度无需考虑光照影响
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

		//防止颜色溢出
		if(sumR > 255.0)
			sumR = 255.0;
		if(sumG > 255.0)
			sumG = 255.0;
		if(sumB > 255.0)
			sumB = 255.0;

		Color col(sumR, sumG, sumB);
		//覆盖当前多边形颜色
		currentPoly->color = col;
	}
}

//用于快速排序的比较函数
int comparePolygonZ(const void* poly1, const void* poly2)
{
	//深度值
	float32 z1, z2;

	//参数为const 只能申请到临时变量来进行比较
	//先讲void转换为T3RenderPoly指针的指针 在通过指针的指针找到(取内容)获得一个T3RenderPoly的指针
	T3RenderPolyPtr tempPoly1 = *((T3RenderPolyPtr *) (poly1)), tempPoly2 = *((T3RenderPolyPtr *) (poly2));

	//计算多边形的平均深度值
	z1 = (float32)0.333333*(tempPoly1->transVertexList[0].z + tempPoly1->transVertexList[1].z + tempPoly1->transVertexList[2].z);
	z2 = (float32)0.333333*(tempPoly2->transVertexList[0].z + tempPoly2->transVertexList[1].z + tempPoly2->transVertexList[2].z);

	//z值降序排列 最大的排在最前面
	if(z1 > z2)
		return -1;
	else if(z1 < z2)
		return 1;
	else
		return 0;
}

//对给定的渲染列表进行排序
void SortRenderList(T3RenderListPtr rendList)
{
	
	qsort((void *) rendList->polyPtr,  //待排序的目标数组
		  rendList->numPolys,	//目标数组长度
		  sizeof(T3RenderPolyPtr),		//待排序元素长度
		  comparePolygonZ);		//比较函数
}