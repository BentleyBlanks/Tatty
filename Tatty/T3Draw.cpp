#include"T3Draw.h"

//��Ⱦ����
//void Draw_T3Object(T3ObjectPtr obj, HDC hdc)
//{
//	int32 antiaAliasing = 0;
//	for(int32 poly = 0; poly<obj->numPolygons; poly++)
//	{
//		//if(obj->polyList[poly].state != T3POLY_STATE_ACTIVE ||
//		//	obj->polyList[poly].state == T3POLY_STATE_CLIPED ||
//		//	obj->polyList[poly].state == T3POLY_STATE_BACKFACE)
//		//	continue;
//
//		//ֻ�е������ǻ�Ĳ���Ⱦ
//		if(obj->state == T3OBJECT_STATE_VISIBLE ||
//		   obj->state == T3OBJECT_STATE_CULLED)
//		   continue;
//
//		//��ȡ��ǰ����ε�������������
//		int32 index1 = obj->polyList[poly].vertex[0];
//		int32 index2 = obj->polyList[poly].vertex[1];
//		int32 index3 = obj->polyList[poly].vertex[2];
//
//		//�Ƿ񿪿����
//		if(!antiaAliasing)
//		{
//			MoveToEx(hdc, obj->vertexList_Trans[index1].x,
//					 obj->vertexList_Trans[index1].y, NULL);
//
//			LineTo(hdc, obj->vertexList_Trans[index2].x,
//				   obj->vertexList_Trans[index2].y);
//
//			MoveToEx(hdc, obj->vertexList_Trans[index2].x,
//					 obj->vertexList_Trans[index2].y, NULL);
//
//			LineTo(hdc, obj->vertexList_Trans[index3].x,
//				   obj->vertexList_Trans[index3].y);
//
//			MoveToEx(hdc, obj->vertexList_Trans[index3].x,
//					 obj->vertexList_Trans[index3].y, NULL);
//
//			LineTo(hdc, obj->vertexList_Trans[index1].x,
//				   obj->vertexList_Trans[index1].y);
//		}
//		else
//		{
//			CTGraphics::DrawLine(hdc, obj->vertexList_Trans[index1].x,
//								 obj->vertexList_Trans[index1].y,
//								 obj->vertexList_Trans[index2].x,
//								 obj->vertexList_Trans[index2].y,
//								 RGB(1.0, 1.0, 0.0));
//
//
//			CTGraphics::DrawLine(hdc, obj->vertexList_Trans[index2].x,
//								 obj->vertexList_Trans[index2].y,
//								 obj->vertexList_Trans[index3].x,
//								 obj->vertexList_Trans[index3].y,
//								 RGB(1.0, 1.0, 0.0));
//
//			CTGraphics::DrawLine(hdc, obj->vertexList_Trans[index3].x,
//								 obj->vertexList_Trans[index3].y,
//								 obj->vertexList_Trans[index1].x,
//								 obj->vertexList_Trans[index1].y,
//								 RGB(1.0, 1.0, 0.0));
//		}
//	}
//
//}


//��Ⱦ��Ⱦ�б�
void Draw_T3RenderList_DirectX(T3RenderListPtr rendList, DirectXDriver *driver)
{
	int32 antiaAliasing = 0;
	for(int32 poly = 0; poly<rendList->numPolys; poly++)
	{
		//��õ�ǰ����ָ��Ķ����
		T3RenderPolyPtr currentPoly = rendList->polyPtr[poly];

		//���ڶ���ε�״̬�����ж�
		if((currentPoly == Null) ||
		   !(currentPoly->state & T3POLY_STATE_ACTIVE) ||
		   (currentPoly->state & T3POLY_STATE_CLIPPED) ||
		   (currentPoly->state & T3POLY_STATE_BACKFACE))
		   //����ת��ֱ�������˶����
		   continue;

		//һ�ε������Ч��
		float32 x0 = rendList->polyPtr[poly]->transVertexList[0].x;
		float32 y0 = rendList->polyPtr[poly]->transVertexList[0].y;

		float32 x1 = rendList->polyPtr[poly]->transVertexList[1].x;
		float32 y1 = rendList->polyPtr[poly]->transVertexList[1].y;

		float32 x2 = rendList->polyPtr[poly]->transVertexList[2].x;
		float32 y2 = rendList->polyPtr[poly]->transVertexList[2].y;
		//�Ƿ񿪿����
		if(!antiaAliasing)
		{
			Color col(50, 50, 50);
			driver->draw2DLine(point2f(x0, y0), point2f(x1, y1), col);
			driver->draw2DLine(point2f(x1, y1), point2f(x2, y2), col);
			driver->draw2DLine(point2f(x2, y2), point2f(x0, y0), col);

			//driver->draw3DLine(point4f(x0, y0, 0), point4f(x1, y1, 0));
			//driver->draw3DLine(point4f(x1, y1, 0), point4f(x2, y2, 0));
			//driver->draw3DLine(point4f(x2, y2, 0), point4f(x0, y0, 0));
		}
		else
		{

		}
	}
}

//ʹ��GDI����Ⱦ��Ⱦ�б�
//void Draw_T3RenderList_GDI(T3RenderListPtr rendList, HDC hdc)
//{
//	int32 antiaAliasing = 0;
//	for(int32 poly = 0; poly<rendList->numPolys; poly++)
//	{
//		//��õ�ǰ����ָ��Ķ����
//		T3RenderPolyPtr currentPoly = rendList->polyPtr[poly];
//
//		//���ڶ���ε�״̬�����ж�
//		if((currentPoly == Null) ||
//		   !(currentPoly->state & T3POLY_STATE_ACTIVE) ||
//		   (currentPoly->state & T3POLY_STATE_CLIPPED) ||
//		   (currentPoly->state & T3POLY_STATE_BACKFACE))
//		   //����ת��ֱ�������˶����
//		   continue;
//
//		//һ�ε������Ч��
//		float32 x0 = rendList->polyPtr[poly]->transVertexList[0].x;
//		float32 y0 = rendList->polyPtr[poly]->transVertexList[0].y;
//
//		float32 x1 = rendList->polyPtr[poly]->transVertexList[1].x;
//		float32 y1 = rendList->polyPtr[poly]->transVertexList[1].y;
//
//		float32 x2 = rendList->polyPtr[poly]->transVertexList[2].x;
//		float32 y2 = rendList->polyPtr[poly]->transVertexList[2].y;
//		//�Ƿ񿪿����
//		if(!antiaAliasing)
//		{
//			MoveToEx(hdc, x0, y0, NULL);
//
//			LineTo(hdc, x1, y1);
//
//			MoveToEx(hdc, x1, y1, NULL);
//
//			LineTo(hdc, x2, y2);
//
//			MoveToEx(hdc, x2, y2, NULL);
//
//			LineTo(hdc, x0, y0);
//		}
//		else
//		{
//			CTGraphics::DrawLine(hdc,
//								 x0, y0,
//								 x1, y1,
//								 RGB(1.0, 1.0, 0.0));
//
//
//			CTGraphics::DrawLine(hdc,
//								 x1, y1,
//								 x2, y2,
//								 RGB(1.0, 1.0, 0.0));
//
//			CTGraphics::DrawLine(hdc,
//								 x2, y2,
//								 x0, y0,
//								 RGB(1.0, 1.0, 0.0));
//		}
//	}
//}

void Draw_T3RenderList_DirectX_Solid(T3RenderListPtr rendList, DirectXDriver *driver)
{
	for(int32 poly = 0; poly<rendList->numPolys; poly++)
	{
		//��õ�ǰ����ָ��Ķ����
		T3RenderPolyPtr currentPoly = rendList->polyPtr[poly];

		//���ڶ���ε�״̬�����ж�
		if((currentPoly == Null) ||
		   !(currentPoly->state & T3POLY_STATE_ACTIVE) ||
		   (currentPoly->state & T3POLY_STATE_CLIPPED) ||
		   (currentPoly->state & T3POLY_STATE_BACKFACE))
		   //����ת��ֱ�������˶����
		   continue;

		//һ�ε������Ч��
		point2f p0(rendList->polyPtr[poly]->transVertexList[0].x, rendList->polyPtr[poly]->transVertexList[0].y);

		point2f p1(rendList->polyPtr[poly]->transVertexList[1].x, rendList->polyPtr[poly]->transVertexList[1].y);

		point2f p2(rendList->polyPtr[poly]->transVertexList[2].x, rendList->polyPtr[poly]->transVertexList[2].y);
		
		drawTriangle(p0, p1, p2, driver, currentPoly->color);
	}
}


//ͨ�õĻ��������κ���
void drawTriangle(point2f p0, point2f p1, point2f p2, DirectXDriver *driver, Color color)
{
	//����һ�ε�ĳ�Ա����
	float32 x0 = p0.x, y0 = p0.y,
		x1 = p1.x, y1 = p1.y,
		x2 = p2.x, y2 = p2.y;

	//�������λ�ý����ж� �鿴�Ƿ�Ϊһ��ֱ��
	if(x0 == x2&&x2 == x1 || y1 == y2&&y2 == y0)
		return;

	float32 tempX, tempY;
	//���������λ�ù�ϵ
	if(y0 > y1)
	{
		//����p1 p2
		T3Swap(y0, y1, tempY);
		T3Swap(x0, x1, tempX);
	}

	if(y2 > y1)
	{
		T3Swap(y1, y2, tempY);
		T3Swap(x1, x2, tempX);
	}

	if(y0 > y2)
	{
		T3Swap(y0, y2, tempY);
		T3Swap(x0, x2, tempX);
	}

	//�߽����--�ṩ�������βü�ʹ��

	point2f _p0(x0, y0), _p1(x1, y1), _p2(x2, y2);
	//ƽ����������Ⱦ
	if(y0 == y2)
	{
		//p0��p2���
		//if(x0 > x2)
		drawTopTriangle(_p1, _p0, _p2, driver, color);
		//else
		//	drawTopTriangle(p1, p2, p0, driver);
	}
	else if(y2 == y1)
	{
		//p1��p2���
		//if(x2 > x1)
		drawBottomTriangle(_p0, _p2, _p1, driver, color);
		//else
			//drawBottomTriangle(p0, p1, p2, driver);
	}
	else
	{
		//���ּ���ָ��ķ���
		//x' = (y2-y1)*1/k + x1
		//x' = (y0-y1)*1/k + x0
		float32 x = (y2 - y1)*((x1 - x0) / (y1 - y0)) + x1;
		//�ҵ�ͨ�������ηָ��
		point2f separate(x, y2);

		drawBottomTriangle(_p0, separate, _p2, driver, color);
		drawTopTriangle(_p1, separate, _p2, driver, color);
	}
}

//���ֲ�ͬ��������ͼԪ���ƺ���
//  p1______p2
//    \	   /
//	   \  /
//	    \/
//      p0
//ƽ��
void drawTopTriangle(point2f p0, point2f p1, point2f p2, DirectXDriver *driver, Color color)
{
	//��֤p1��p2�����
	float32 temp;
	//ֻ�ǽ�����p0 p1����ʱ�����еĳ�Ա���� �����ı�p1 p2����
	if(p1.x > p2.x)
		T3Swap(p1.x, p2.x, temp);

	float32 height = p0.y - p1.y;
	
	float32 left = p0.x - p1.x, right = p0.x - p2.x;

	//�����ݶ� б�ʵĵ���
	float32 dxdyLeft = left / height, 
		dxdyRight = right / height;

	float32 xLeft = p1.x, xRight = p2.x;
	//��դ��ѭ��
	for(float32 y = p1.y; y < p0.y; y += 1.0)
	{
		driver->draw2DLine(point2f(xLeft, y), point2f(xRight, y), color);

		xLeft += dxdyLeft;
		xRight += dxdyRight;
	}
}
//   p0
//   /\
//  /  \
// /____\
//p2	 p1
//ƽ��
void drawBottomTriangle(point2f p0, point2f p1, point2f p2, DirectXDriver *driver, Color color)
{
	//��֤p2��p1�����
	float32 temp;
	//ֻ�ǽ�����p2 p1����ʱ�����еĳ�Ա���� �����ı�p1 p2����
	if(p1.x < p2.x)
		T3Swap(p1.x, p2.x, temp);

	float32 height = p2.y - p0.y;

	float32 left = p2.x - p0.x, right = p1.x - p0.x;

	//�����ݶ� б�ʵĵ���
	float32 dxdyLeft = left / height,
		dxdyRight = right / height;

	float32 xLeft = p0.x, xRight = p0.x;
	//��դ��ѭ��
	for(float32 y = p0.y; y < p1.y; y += 1.0)
	{
		driver->draw2DLine(point2f(xLeft, y), point2f(xRight, y), color);

		xLeft += dxdyLeft;
		xRight += dxdyRight;
	}
}

//������պ͵���
void drawSkyBox(DirectXDriver *driver)
{
	for(float32 i = 0.0; i < screenHeight / 2; i += 1.0)
	{
		driver->draw2DLine(point2f(0.0, i), point2f(screenWidth, i), Color(181, 220, 255));
	}

	for(float32 i = screenHeight / 2; i < screenHeight; i += 1.0)
	{
		driver->draw2DLine(point2f(0.0, i), point2f(screenWidth, i), Color(34, 140, 76));
	}
}

//����ƽ��Gouraud��ɫ������
void drawGouraudTriangle(point2f p0, point2f p1, point2f p2, DirectXDriver *driver, Color color)
{


}