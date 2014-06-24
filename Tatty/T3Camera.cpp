#include"T3Camera.h"

void T3CameraInit(T3CameraPtr cam, T3Point4DPtr pos, T3Vector4DPtr dir,
				  float32 FOV,
				  float32 farClipZ, float32 nearClipZ,
				  float32 viewPortWidth, float32 viewPortHeight)
{
	//������Ϣ
	//����λ�ú���ת�Ƕ���Ϣ
	T3Vector4DCopy(&cam->position, pos);
	T3Vector4DCopy(&cam->direction, dir);

    //��ʼ������ֲ�������
    T3Vector4D u = {1.0f, 0.0f, 0.0f, 1.0f}, v = {0.0, 1.0, 0.0, 1.0}, n = {0.0, 0.0, 1.0, 1.0};
    cam->u = u;
    cam->v = v;
    cam->n = n;

	//��ʼ����Ұ
	cam->FOV = FOV;

	//��ʼ���ӿ�
	cam->viewPortHeight = viewPortHeight;
	cam->viewPortWidth = viewPortWidth;

	//����������λ��
	T3MatIdentity4X4(&cam->mCam);
	T3MatIdentity4X4(&cam->mPer);
	T3MatIdentity4X4(&cam->mScr);

	//����Զ���ü���
	cam->farClipZ = farClipZ;
	cam->nearClipZ = nearClipZ;

	//�ӿڵ����� �����0��ʼ���� ��˼�һ
	cam->viewPortCenterX = (viewPortWidth - 1) / 2;
	cam->viewPortCenterY = (viewPortHeight - 1) / 2;

	//��ʼ����Ļ��߱�
	cam->aspectRadio = viewPortWidth / viewPortHeight;

	//��ʼ����ƽ��Ϊ2X2/as
	cam->viewPlaneHeight = 2.0 / cam->aspectRadio;
	cam->viewPlaneWidth = 2.0;

	//�õ�FOV/2��tan
	float32 TanFOVDiv2 = tan(AngleToRadian(FOV / 2));

	//�����Ӿ�
	cam->viewDistance = 0.5 * cam->viewPlaneWidth * TanFOVDiv2;
}

void T3CameraBuildMat(T3CameraPtr cam, int32 rotationOrder)
{
	//ƽ�� xyz��ת�������
	T3Mat4X4 mTranslateInv, mXInv, mYInv, mZInv;
	//���絽�������任�ľ���
	T3Mat4X4 mCamInv;
	//��ʱ����
	T3Mat4X4 mTemp;

	//��ʼ��ƽ�ƾ���������
	T3MatInit4X4(&mTranslateInv,
				 1, 0, 0, 0,
				 0, 1, 0, 0,
				 0, 0, 1, 0,
				 -cam->position.x, -cam->position.y, -cam->position.z, 1);

	//��ȡ��Ҫ��ת�ĽǶ�
	float32 thetaX = cam->direction.x;
	float32 thetaY = cam->direction.y;
	float32 thetaZ = cam->direction.z;

	//��������ֵ
	float32 cosTheta = FastCos(thetaX);
	float32 sinTheta = -FastSin(thetaX);

	//��ʼ��X��������
	T3MatInit4X4(&mXInv,
				 1, 0, 0, 0,
				 0, cosTheta, sinTheta, 0,
				 0, -sinTheta, cosTheta, 0,
				 0, 0, 0, 1);

	//��������ֵ
	cosTheta = FastCos(thetaY);
	sinTheta = -FastSin(thetaY);

	//��ʼ��Y��������
	T3MatInit4X4(&mYInv,
				 cosTheta, 0, -sinTheta, 0,
				 0, 1, 0, 0,
				 sinTheta, 0, cosTheta, 0,
				 0, 0, 0, 1);

	//��������ֵ
	cosTheta = FastCos(thetaZ);
	sinTheta = -FastSin(thetaZ);

	//��ʼ��Z��������
	T3MatInit4X4(&mZInv,
				 cosTheta, sinTheta, 0, 0,
				 -sinTheta, cosTheta, 0, 0,
				 0, 0, 1, 0,
				 0, 0, 0, 1);

	//������ת˳����в�ͬ����ת�任
	switch(rotationOrder)
	{
	case CAM_ROT_XYZ:
	{
						T3MatMul4X4(&mXInv, &mYInv, &mTemp);
						T3MatMul4X4(&mTemp, &mZInv, &mCamInv);
	} break;

	case CAM_ROT_YXZ:
	{
						T3MatMul4X4(&mYInv, &mXInv, &mTemp);
						T3MatMul4X4(&mTemp, &mZInv, &mCamInv);
	} break;

	case CAM_ROT_XZY:
	{
						T3MatMul4X4(&mXInv, &mZInv, &mTemp);
						T3MatMul4X4(&mTemp, &mYInv, &mCamInv);
	} break;

	case CAM_ROT_YZX:
	{
						T3MatMul4X4(&mYInv, &mZInv, &mTemp);
						T3MatMul4X4(&mTemp, &mXInv, &mCamInv);
	} break;

	case CAM_ROT_ZYX:
	{
						T3MatMul4X4(&mZInv, &mYInv, &mTemp);
						T3MatMul4X4(&mTemp, &mXInv, &mCamInv);
	} break;

	case CAM_ROT_ZXY:
	{
						T3MatMul4X4(&mZInv, &mXInv, &mTemp);
						T3MatMul4X4(&mTemp, &mYInv, &mCamInv);

	} break;

	}

	//����ת������ƽ�ƾ������
	T3MatMul4X4(&mTranslateInv, &mCamInv, &cam->mCam);
}

//�����޳�
int32 T3CameraCull_T3Object(T3ObjectPtr obj, T3CameraPtr cam, int32 cullFlag)
{
	//Ϊ�����ʼ���任����
	T3CameraBuildMat(cam, CAM_ROT_XYZ);

	//�洢����İ�Χ�����ı任�������
	T3Point4D spherePosition;

	//�԰�Χ����������(��������)���б任
	T3Mat_T3Vector4D_Mul_4X4(&obj->worldPosition, &cam->mCam, &spherePosition);

	//Զ���ü���
	if(cullFlag & CULL_T3OBJECT_Z)
	{
		if(((spherePosition.z - obj->maxRadius) > cam->farClipZ) || 
		   ((spherePosition.z + obj->maxRadius) < cam->nearClipZ))
		{
			//��������״̬
			SetBit(obj->state, T3OBJECT_STATE_CULLED);
			return 1;
		}
	}

	//�������Ҳü�������޳�
	if(cullFlag & CULL_T3OBJECT_X)
	{
		float32 xTest = spherePosition.z * cam->viewPlaneWidth * 0.5 / cam->viewDistance;

		if((spherePosition.x - obj->maxRadius)>xTest || 
		   (spherePosition.x + obj->maxRadius)<-xTest)
		{
			//��������״̬
			SetBit(obj->state, T3OBJECT_STATE_CULLED);
			return 1;
		}
	}

	//���²ü���
	if(cullFlag & CULL_T3OBJECT_Y)
	{
		float32 yTest = spherePosition.z * cam->viewPlaneHeight * 0.5 / cam->viewDistance;

		if((spherePosition.y - obj->maxRadius) > yTest || 
		   (spherePosition.y + obj->maxRadius) < -yTest)
		{
			//��������״̬
			SetBit(obj->state, T3OBJECT_STATE_CULLED);
			return 1;
		}
	}

	return 0;
}

//�������ע�ӷ���
void updateCameraAxis(T3CameraPtr cam)
{

	//// ������ʱ��UVN(δ��һ��)  
	//T3Vector4D u, v, n;
	//// ��N  
	//T3Vector4DSub(&cam->target, &cam->position, &n);
	//// ����V  
	//T3Vector4DCopy(&v, &cam->v);
	//// ӦΪN��V�������һ��ƽ�棬���Կ�������U  
	//T3Vector4DCross(&v, &n, &u);
	//// ��ΪV��N���ܲ���ֱ�����Է���V��ʹ��V��U��N����ֱ  
	//T3Vector4DCross(&n, &u, &v);
	//// UVN��һ  
	//T3Vector4DNormalize(&u, &cam->u);
	//T3Vector4DNormalize(&v, &cam->v);
	//T3Vector4DNormalize(&n, &cam->n);

    //��ǰ��ȡcam�ֲ���������Ϣ
    //������ֵ�ᷢ���仯 ��˲���ָ����ʽ
    float32 *vx = &cam->v.x, *vy = &cam->v.y, *vz = &cam->v.z;
    float32 *ux = &cam->u.x, *uy = &cam->u.y, *uz = &cam->u.z;
    float32 thetaY = cam->direction.y, thetaX = cam->direction.x;
    T3Vector4D temp;
    float32 negaCosThetaY = 1 - FastCos(thetaY), sinThetaY = FastSin(thetaY), cosThetaY = FastCos(thetaY),
        negaCosThetaX = 1 - FastCos(thetaX), sinThetaX = FastSin(thetaX), cosThetaX = FastCos(thetaX);

    if(thetaY != 0.0)
    {
        //����v����и���-->direction.y
        //������v�����ת����
        T3Mat4X4 mV = {
            (*vx)*(*vx)*negaCosThetaY, (*vx)*(*vy)*negaCosThetaY + (*vz)*sinThetaY, (*vx)*(*vz)*negaCosThetaY - (*vy)*sinThetaY, 0.0,
            (*vx)*(*vy)*negaCosThetaY - (*vz)*sinThetaY, (*vy)*(*vy)*negaCosThetaY + cosThetaY, (*vy)*(*vz)*negaCosThetaY + (*vx)*sinThetaY, 0.0,
            (*vx)*(*vz)*negaCosThetaY + (*vy)*sinThetaY, (*vy)*(*vz)*negaCosThetaY - (*vx)*sinThetaY, (*vz)*(*vz)*negaCosThetaY + cosThetaY, 0.0,
            0.0, 0.0, 0.0, 1.0};

        //n
        T3Mat_T3Vector3D_Mul_4X4(&cam->n, &mV, &temp);
        T3Vector4DCopy(&cam->n, &temp);

        //u
        T3Mat_T3Vector3D_Mul_4X4(&cam->u, &mV, &temp);
        T3Vector4DCopy(&cam->u, &temp);
        
        //���������й�һ��
        T3Vector4DNormalize(&cam->n);
        T3Vector4DNormalize(&cam->u);
    }

    if(thetaX != 0.0)
    {
        //��u����и���-->direction.x
        //������u�����ת����
        T3Mat4X4 mU = {
            (*ux)*(*ux)*negaCosThetaX, (*ux)*(*uy)*negaCosThetaX + (*uz)*sinThetaX, (*ux)*(*uz)*negaCosThetaX - (*uy)*sinThetaX, 0.0,
            (*ux)*(*uy)*negaCosThetaX - (*uz)*sinThetaX, (*uy)*(*uy)*negaCosThetaX + cosThetaX, (*uy)*(*uz)*negaCosThetaX + (*ux)*sinThetaX, 0.0,
            (*ux)*(*uz)*negaCosThetaX + (*uy)*sinThetaX, (*uy)*(*uz)*negaCosThetaX - (*ux)*sinThetaX, (*uz)*(*uz)*negaCosThetaX + cosThetaX, 0.0,
            0.0, 0.0, 0.0, 1.0};

        //v
        T3Mat_T3Vector3D_Mul_4X4(&cam->v, &mU, &temp);
        T3Vector4DCopy(&cam->v, &temp);
        //n
        T3Mat_T3Vector3D_Mul_4X4(&cam->n, &mU, &temp);
        T3Vector4DCopy(&cam->n, &temp);

        //���������й�һ��
        T3Vector4DNormalize(&cam->n);
        T3Vector4DNormalize(&cam->v);
    }
}

//���ݷ���������λ��
void updateCameraPosition(T3CameraPtr cam, int32 direction)
{

}