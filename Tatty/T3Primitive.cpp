#include"T3Primitive.h"
//���������״̬
void T3ObjectReset(T3ObjectPtr obj, int32 numOfObj)
{
	for(int32 numObj=0; numObj<numOfObj; numObj++)
	{
		//��������״̬
		ResetBit(obj[numObj].state, T3OBJECT_STATE_CULLED);

		for(int32 poly=0; poly<obj[numObj].numPolygons; poly++)
		{
			//��ȡ��ǰ�����
			T3PolyPtr currentPoly = &obj[numObj].polyList[poly];

			if(!(currentPoly->state & T3POLY_STATE_ACTIVE))
				continue;

			ResetBit(currentPoly->state, T3POLY_STATE_CLIPPED);
			ResetBit(currentPoly->state, T3POLY_STATE_BACKFACE);
		}
	}

}


