#include"T3Primitive.h"
//重置物体的状态
void T3ObjectReset(T3ObjectPtr obj, int32 numOfObj)
{
	for(int32 numObj=0; numObj<numOfObj; numObj++)
	{
		//重置物体状态
		ResetBit(obj[numObj].state, T3OBJECT_STATE_CULLED);

		for(int32 poly=0; poly<obj[numObj].numPolygons; poly++)
		{
			//获取当前多边形
			T3PolyPtr currentPoly = &obj[numObj].polyList[poly];

			if(!(currentPoly->state & T3POLY_STATE_ACTIVE))
				continue;

			ResetBit(currentPoly->state, T3POLY_STATE_CLIPPED);
			ResetBit(currentPoly->state, T3POLY_STATE_BACKFACE);
		}
	}

}


