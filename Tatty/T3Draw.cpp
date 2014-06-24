#include"T3Draw.h"

//渲染物体
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
//		//只有当物体是活动的才渲染
//		if(obj->state == T3OBJECT_STATE_VISIBLE ||
//		   obj->state == T3OBJECT_STATE_CULLED)
//		   continue;
//
//		//读取当前多边形的三个顶点索引
//		int32 index1 = obj->polyList[poly].vertex[0];
//		int32 index2 = obj->polyList[poly].vertex[1];
//		int32 index3 = obj->polyList[poly].vertex[2];
//
//		//是否开抗锯齿
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


//渲染渲染列表
void Draw_T3RenderList_DirectX(T3RenderListPtr rendList, DirectXDriver *driver)
{
	int32 antiaAliasing = 0;
	for(int32 poly = 0; poly<rendList->numPolys; poly++)
	{
		//获得当前索引指向的多边形
		T3RenderPolyPtr currentPoly = rendList->polyPtr[poly];

		//对于多边形的状态进行判断
		if((currentPoly == Null) ||
		   !(currentPoly->state & T3POLY_STATE_ACTIVE) ||
		   (currentPoly->state & T3POLY_STATE_CLIPPED) ||
		   (currentPoly->state & T3POLY_STATE_BACKFACE))
		   //无需转换直接跳过此多边形
		   continue;

		//一次调用提高效率
		float32 x0 = rendList->polyPtr[poly]->transVertexList[0].x;
		float32 y0 = rendList->polyPtr[poly]->transVertexList[0].y;

		float32 x1 = rendList->polyPtr[poly]->transVertexList[1].x;
		float32 y1 = rendList->polyPtr[poly]->transVertexList[1].y;

		float32 x2 = rendList->polyPtr[poly]->transVertexList[2].x;
		float32 y2 = rendList->polyPtr[poly]->transVertexList[2].y;
		//是否开抗锯齿
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

//使用GDI来渲染渲染列表
//void Draw_T3RenderList_GDI(T3RenderListPtr rendList, HDC hdc)
//{
//	int32 antiaAliasing = 0;
//	for(int32 poly = 0; poly<rendList->numPolys; poly++)
//	{
//		//获得当前索引指向的多边形
//		T3RenderPolyPtr currentPoly = rendList->polyPtr[poly];
//
//		//对于多边形的状态进行判断
//		if((currentPoly == Null) ||
//		   !(currentPoly->state & T3POLY_STATE_ACTIVE) ||
//		   (currentPoly->state & T3POLY_STATE_CLIPPED) ||
//		   (currentPoly->state & T3POLY_STATE_BACKFACE))
//		   //无需转换直接跳过此多边形
//		   continue;
//
//		//一次调用提高效率
//		float32 x0 = rendList->polyPtr[poly]->transVertexList[0].x;
//		float32 y0 = rendList->polyPtr[poly]->transVertexList[0].y;
//
//		float32 x1 = rendList->polyPtr[poly]->transVertexList[1].x;
//		float32 y1 = rendList->polyPtr[poly]->transVertexList[1].y;
//
//		float32 x2 = rendList->polyPtr[poly]->transVertexList[2].x;
//		float32 y2 = rendList->polyPtr[poly]->transVertexList[2].y;
//		//是否开抗锯齿
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
		//获得当前索引指向的多边形
		T3RenderPolyPtr currentPoly = rendList->polyPtr[poly];

		//对于多边形的状态进行判断
		if((currentPoly == Null) ||
		   !(currentPoly->state & T3POLY_STATE_ACTIVE) ||
		   (currentPoly->state & T3POLY_STATE_CLIPPED) ||
		   (currentPoly->state & T3POLY_STATE_BACKFACE))
		   //无需转换直接跳过此多边形
		   continue;

		//一次调用提高效率
		point2f p0(rendList->polyPtr[poly]->transVertexList[0].x, rendList->polyPtr[poly]->transVertexList[0].y);

		point2f p1(rendList->polyPtr[poly]->transVertexList[1].x, rendList->polyPtr[poly]->transVertexList[1].y);

		point2f p2(rendList->polyPtr[poly]->transVertexList[2].x, rendList->polyPtr[poly]->transVertexList[2].y);
		
		drawTriangle(p0, p1, p2, driver, currentPoly->color);
	}
}


//通用的绘制三角形函数
void drawTriangle(point2f p0, point2f p1, point2f p2, DirectXDriver *driver, Color color)
{
	//访问一次点的成员变量
	float32 x0 = p0.x, y0 = p0.y,
		x1 = p1.x, y1 = p1.y,
		x2 = p2.x, y2 = p2.y;

	//对三点的位置进行判断 查看是否为一条直线
	if(x0 == x2&&x2 == x1 || y1 == y2&&y2 == y0)
		return;

	float32 tempX, tempY;
	//处理三点的位置关系
	if(y0 > y1)
	{
		//交换p1 p2
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

	//边界测试--提供给三角形裁剪使用

	point2f _p0(x0, y0), _p1(x1, y1), _p2(x2, y2);
	//平顶三角形渲染
	if(y0 == y2)
	{
		//p0在p2左边
		//if(x0 > x2)
		drawTopTriangle(_p1, _p0, _p2, driver, color);
		//else
		//	drawTopTriangle(p1, p2, p0, driver);
	}
	else if(y2 == y1)
	{
		//p1在p2左边
		//if(x2 > x1)
		drawBottomTriangle(_p0, _p2, _p1, driver, color);
		//else
			//drawBottomTriangle(p0, p1, p2, driver);
	}
	else
	{
		//多种计算分割点的方法
		//x' = (y2-y1)*1/k + x1
		//x' = (y0-y1)*1/k + x0
		float32 x = (y2 - y1)*((x1 - x0) / (y1 - y0)) + x1;
		//找到通用三角形分割点
		point2f separate(x, y2);

		drawBottomTriangle(_p0, separate, _p2, driver, color);
		drawTopTriangle(_p1, separate, _p2, driver, color);
	}
}

//两种不同的三角形图元绘制函数
//  p1______p2
//    \	   /
//	   \  /
//	    \/
//      p0
//平顶
void drawTopTriangle(point2f p0, point2f p1, point2f p2, DirectXDriver *driver, Color color)
{
	//保证p1在p2的左边
	float32 temp;
	//只是交换了p0 p1的临时变量中的成员内容 并不改变p1 p2本身
	if(p1.x > p2.x)
		T3Swap(p1.x, p2.x, temp);

	float32 height = p0.y - p1.y;
	
	float32 left = p0.x - p1.x, right = p0.x - p2.x;

	//计算梯度 斜率的倒数
	float32 dxdyLeft = left / height, 
		dxdyRight = right / height;

	float32 xLeft = p1.x, xRight = p2.x;
	//光栅化循环
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
//平底
void drawBottomTriangle(point2f p0, point2f p1, point2f p2, DirectXDriver *driver, Color color)
{
	//保证p2在p1的左边
	float32 temp;
	//只是交换了p2 p1的临时变量中的成员内容 并不改变p1 p2本身
	if(p1.x < p2.x)
		T3Swap(p1.x, p2.x, temp);

	float32 height = p2.y - p0.y;

	float32 left = p2.x - p0.x, right = p1.x - p0.x;

	//计算梯度 斜率的倒数
	float32 dxdyLeft = left / height,
		dxdyRight = right / height;

	float32 xLeft = p0.x, xRight = p0.x;
	//光栅化循环
	for(float32 y = p0.y; y < p1.y; y += 1.0)
	{
		driver->draw2DLine(point2f(xLeft, y), point2f(xRight, y), color);

		xLeft += dxdyLeft;
		xRight += dxdyRight;
	}
}

//绘制天空和地面
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

//绘制平滑Gouraud着色三角形
void drawGouraudTriangle(point2f p0, point2f p1, point2f p2, DirectXDriver *driver, Color color)
{


}