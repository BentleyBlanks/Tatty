#include"T3Camera.h"

void T3CameraInit(T3CameraPtr cam, T3Point4DPtr pos, T3Vector4DPtr dir,
				  float32 FOV,
				  float32 farClipZ, float32 nearClipZ,
				  float32 viewPortWidth, float32 viewPortHeight)
{
	//拷贝信息
	//拷贝位置和旋转角度信息
	T3Vector4DCopy(&cam->position, pos);
	T3Vector4DCopy(&cam->direction, dir);

    //初始化相机局部坐标轴
    T3Vector4D u = {1.0f, 0.0f, 0.0f, 1.0f}, v = {0.0, 1.0, 0.0, 1.0}, n = {0.0, 0.0, 1.0, 1.0};
    cam->u = u;
    cam->v = v;
    cam->n = n;

	//初始化视野
	cam->FOV = FOV;

	//初始化视口
	cam->viewPortHeight = viewPortHeight;
	cam->viewPortWidth = viewPortWidth;

	//将三个矩阵单位化
	T3MatIdentity4X4(&cam->mCam);
	T3MatIdentity4X4(&cam->mPer);
	T3MatIdentity4X4(&cam->mScr);

	//定义远近裁剪面
	cam->farClipZ = farClipZ;
	cam->nearClipZ = nearClipZ;

	//视口的中心 坐标从0开始计数 因此减一
	cam->viewPortCenterX = (viewPortWidth - 1) / 2;
	cam->viewPortCenterY = (viewPortHeight - 1) / 2;

	//初始化屏幕宽高比
	cam->aspectRadio = viewPortWidth / viewPortHeight;

	//初始化视平面为2X2/as
	cam->viewPlaneHeight = 2.0 / cam->aspectRadio;
	cam->viewPlaneWidth = 2.0;

	//得到FOV/2的tan
	float32 TanFOVDiv2 = tan(AngleToRadian(FOV / 2));

	//计算视距
	cam->viewDistance = 0.5 * cam->viewPlaneWidth * TanFOVDiv2;
}

void T3CameraBuildMat(T3CameraPtr cam, int32 rotationOrder)
{
	//平移 xyz旋转的逆矩阵
	T3Mat4X4 mTranslateInv, mXInv, mYInv, mZInv;
	//世界到相机坐标变换的矩阵
	T3Mat4X4 mCamInv;
	//临时矩阵
	T3Mat4X4 mTemp;

	//初始化平移矩阵的逆矩阵
	T3MatInit4X4(&mTranslateInv,
				 1, 0, 0, 0,
				 0, 1, 0, 0,
				 0, 0, 1, 0,
				 -cam->position.x, -cam->position.y, -cam->position.z, 1);

	//提取需要旋转的角度
	float32 thetaX = cam->direction.x;
	float32 thetaY = cam->direction.y;
	float32 thetaZ = cam->direction.z;

	//正弦余弦值
	float32 cosTheta = FastCos(thetaX);
	float32 sinTheta = -FastSin(thetaX);

	//初始化X轴的逆矩阵
	T3MatInit4X4(&mXInv,
				 1, 0, 0, 0,
				 0, cosTheta, sinTheta, 0,
				 0, -sinTheta, cosTheta, 0,
				 0, 0, 0, 1);

	//正弦余弦值
	cosTheta = FastCos(thetaY);
	sinTheta = -FastSin(thetaY);

	//初始化Y轴的逆矩阵
	T3MatInit4X4(&mYInv,
				 cosTheta, 0, -sinTheta, 0,
				 0, 1, 0, 0,
				 sinTheta, 0, cosTheta, 0,
				 0, 0, 0, 1);

	//正弦余弦值
	cosTheta = FastCos(thetaZ);
	sinTheta = -FastSin(thetaZ);

	//初始化Z轴的逆矩阵
	T3MatInit4X4(&mZInv,
				 cosTheta, sinTheta, 0, 0,
				 -sinTheta, cosTheta, 0, 0,
				 0, 0, 1, 0,
				 0, 0, 0, 1);

	//根据旋转顺序进行不同的旋转变换
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

	//将旋转矩阵与平移矩阵相乘
	T3MatMul4X4(&mTranslateInv, &mCamInv, &cam->mCam);
}

//物体剔除
int32 T3CameraCull_T3Object(T3ObjectPtr obj, T3CameraPtr cam, int32 cullFlag)
{
	//为相机初始化变换矩阵
	T3CameraBuildMat(cam, CAM_ROT_XYZ);

	//存储物体的包围球中心变换后的坐标
	T3Point4D spherePosition;

	//对包围球中心坐标(世界坐标)进行变换
	T3Mat_T3Vector4D_Mul_4X4(&obj->worldPosition, &cam->mCam, &spherePosition);

	//远近裁剪面
	if(cullFlag & CULL_T3OBJECT_Z)
	{
		if(((spherePosition.z - obj->maxRadius) > cam->farClipZ) || 
		   ((spherePosition.z + obj->maxRadius) < cam->nearClipZ))
		{
			//更新物体状态
			SetBit(obj->state, T3OBJECT_STATE_CULLED);
			return 1;
		}
	}

	//根据左右裁剪面进行剔除
	if(cullFlag & CULL_T3OBJECT_X)
	{
		float32 xTest = spherePosition.z * cam->viewPlaneWidth * 0.5 / cam->viewDistance;

		if((spherePosition.x - obj->maxRadius)>xTest || 
		   (spherePosition.x + obj->maxRadius)<-xTest)
		{
			//更新物体状态
			SetBit(obj->state, T3OBJECT_STATE_CULLED);
			return 1;
		}
	}

	//上下裁剪面
	if(cullFlag & CULL_T3OBJECT_Y)
	{
		float32 yTest = spherePosition.z * cam->viewPlaneHeight * 0.5 / cam->viewDistance;

		if((spherePosition.y - obj->maxRadius) > yTest || 
		   (spherePosition.y + obj->maxRadius) < -yTest)
		{
			//更新物体状态
			SetBit(obj->state, T3OBJECT_STATE_CULLED);
			return 1;
		}
	}

	return 0;
}

//更新相机注视方向
void updateCameraAxis(T3CameraPtr cam)
{

	//// 定义临时的UVN(未归一化)  
	//T3Vector4D u, v, n;
	//// 求N  
	//T3Vector4DSub(&cam->target, &cam->position, &n);
	//// 设置V  
	//T3Vector4DCopy(&v, &cam->v);
	//// 应为N和V可以组成一个平面，所以可求法向量U  
	//T3Vector4DCross(&v, &n, &u);
	//// 因为V和N可能不垂直，所以反求V，使得V和U、N都垂直  
	//T3Vector4DCross(&n, &u, &v);
	//// UVN归一  
	//T3Vector4DNormalize(&u, &cam->u);
	//T3Vector4DNormalize(&v, &cam->v);
	//T3Vector4DNormalize(&n, &cam->n);

    //提前获取cam局部坐标轴信息
    //因坐标值会发生变化 因此采用指针形式
    float32 *vx = &cam->v.x, *vy = &cam->v.y, *vz = &cam->v.z;
    float32 *ux = &cam->u.x, *uy = &cam->u.y, *uz = &cam->u.z;
    float32 thetaY = cam->direction.y, thetaX = cam->direction.x;
    T3Vector4D temp;
    float32 negaCosThetaY = 1 - FastCos(thetaY), sinThetaY = FastSin(thetaY), cosThetaY = FastCos(thetaY),
        negaCosThetaX = 1 - FastCos(thetaX), sinThetaX = FastSin(thetaX), cosThetaX = FastCos(thetaX);

    if(thetaY != 0.0)
    {
        //先绕v轴进行更新-->direction.y
        //创建绕v轴的旋转矩阵
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
        
        //对向量进行归一化
        T3Vector4DNormalize(&cam->n);
        T3Vector4DNormalize(&cam->u);
    }

    if(thetaX != 0.0)
    {
        //绕u轴进行更新-->direction.x
        //创建绕u轴的旋转矩阵
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

        //对向量进行归一化
        T3Vector4DNormalize(&cam->n);
        T3Vector4DNormalize(&cam->v);
    }
}

//根据方向更新相机位置
void updateCameraPosition(T3CameraPtr cam, int32 direction)
{

}