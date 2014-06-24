#include"DirectXDriver.h"


#define D3DFVF_VERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

DirectXDriver::DirectXDriver(HWND hwnd)
{
	this->hwnd = hwnd;

	g_D3D = NULL;
	g_D3DDevice = NULL;
	g_VertexBuffer = NULL;

	numVertex = 0;
    numPoints = 0;
}

//驱动初始化
bool DirectXDriver::init()
{
	//初始化顶点数目
	memset(vertex, 0, numVertex*sizeof(vertex3));

	D3DDISPLAYMODE displayMode;

	//Create the D3D object
	g_D3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(g_D3D == NULL)
		return false;

	//Get the desktop display mode
	if(FAILED(g_D3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &displayMode)))
		return false;

	//set up the structure used to create the D3DDevice
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

    d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = displayMode.Format;

    //Check if 4x AA is supported, if so use it.
    //检查是否支持4倍速率采样
    if(g_D3D->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL, displayMode.Format, false,
        D3DMULTISAMPLE_8_SAMPLES,
        NULL) == D3D_OK)
    {
        // 保存多采样类型
        d3dpp.MultiSampleType = D3DMULTISAMPLE_8_SAMPLES;
	}

	//Create the D3DDevice
	if(FAILED(g_D3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &g_D3DDevice)))
		return false;

	return true;
}


//开始绘制
void DirectXDriver::begin()
{
	//Clear the backbuffer
    g_D3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(55, 55, 55), 1.0f, 0);

	//Begin the scene , Start rendering
	g_D3DDevice->BeginScene();

	//清空顶点数目
	memset(vertex, 0, numVertex*sizeof(vertex3));
	//还原顶点数 像素数
	numVertex = 0;
    numPoints = 0;
}

//结束绘制
void DirectXDriver::end()
{
	//渲染所有端点
	//创建顶点缓存
	if(FAILED(g_D3DDevice->CreateVertexBuffer(sizeof(vertex), 0, D3DFVF_VERTEX, D3DPOOL_DEFAULT, &g_VertexBuffer, NULL)))
		return;

	g_D3DDevice->SetFVF(D3DFVF_VERTEX);
	g_D3DDevice->SetStreamSource(0, g_VertexBuffer, 0, sizeof(vertex3));

    //线列
    if(numVertex >= 0 || numVertex < FLT_MAX)
        g_D3DDevice->DrawPrimitiveUP(D3DPT_LINELIST, numVertex, vertex, sizeof(vertex3));

    //点阵
    if(numPoints >= 0 || numPoints < FLT_MAX)
        g_D3DDevice->DrawPrimitiveUP(D3DPT_POINTLIST, numPoints, points, sizeof(vertex3));


	if(g_VertexBuffer != NULL)
	{
		g_VertexBuffer->Release();
		//防止野指针出现
		g_VertexBuffer = nullptr;
	}

	//End the scene , Stop rendering
	g_D3DDevice->EndScene();

	//Display the scene
	g_D3DDevice->Present(NULL, NULL, NULL, NULL);
}

//绘制一条2D直线
void DirectXDriver::draw2DLine(point2f &a, point2f &b, Color &col)
{
	point4f start(a.x, a.y, 0.0, 1.0), end(b.x, b.y, 0.0, 1.0);

	vertex[numVertex].color = col;
	vertex[numVertex + 1].color = col;
	vertex[numVertex].position = start;
	vertex[numVertex + 1].position = end;

	//更新当前顶点数
	numVertex += 2;
}

//绘制一条3D/4D直线
void DirectXDriver::draw3DLine(point4f &a, point4f &b, Color &col)
{
	vertex[numVertex].color = col;
	vertex[numVertex + 1].color = col;
	vertex[numVertex].position = a;
	vertex[numVertex + 1].position = b;

	//更新当前顶点数
	numVertex += 2;
}

//绘制一个矩形
void DirectXDriver::draw2DRectangle(point2f &top, point2f &down, Color &col)
{
	vertex3 v[4];
	//v[0] = vertex3(top.x, top.y, 0.0f, 0.0f, col);
	//v[1] = vertex3();
	//v[2] = vertex3();
	//v[3] = vertex3();

	int indices[6] = {0, 1, 2, 0, 2, 3};


	g_D3DDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 4, 2, &indices[0],
										D3DFMT_INDEX16, &v[0], sizeof(vertex3));
}

//绘制单一像素
void DirectXDriver::drawPixel(point2f &point, Color &col)
{
    //2D->4D
    point4f temp(point.x, point.y, 0.0, 1.0);

    points[numPoints].color = col;
    points[numPoints].position = temp;

    numPoints++;
}

//关闭DirectX
void DirectXDriver::shutDown()
{
	if(g_D3DDevice != NULL)
		g_D3DDevice->Release();
	if(g_D3D != NULL)
		//g_D3D->Release();
	if(g_VertexBuffer != NULL)
		g_VertexBuffer->Release();

	g_D3DDevice = NULL;
	g_D3D = NULL;
	g_VertexBuffer = NULL;
}