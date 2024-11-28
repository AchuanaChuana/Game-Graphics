#include "windowheader.h"
#include "adaptor.h"
#include "dxCore.h"
#include "Shader.h"
#include "Mesh.h"
#include "camera.h"

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
	Window win;
	DxCore dx;
	Shaders shader;
	plane plane;
	FPCamera camera;
	
	win.Init(1024, 768, "MyWindow");
	dx.Init(1024, 768, win.hwnd, false);
	shader.init("Shaderv.txt", "Shaderp.txt", &dx);
	plane.init(&dx);

	Vec4 eye = Vec4(10.f,10.f,10.f,1.f);
	Vec4 center = Vec4(0.f, 1.f, 0.f, 1.f);
	Vec4 up = Vec4(0.f, 0.707f, 0.707f, 1.f);
	camera.setViewMatrix(eye, center, up);
	camera.setProjectionMatrix(75, 1024.f/768.f, 0.1,100);

	Matrix44 defaultMatrix;
	//Matrix44 scaledmatrix = defaultMatrix.scaling(Vec3(0.001, 0.001, 0.001));
	Matrix44 viewmatrix = camera.getViewMatrix();
	Matrix44 projectionmatrix = camera.getProjectionMatrix();
	Matrix44 together = projectionmatrix * viewmatrix ;

	while (true)
	{
		//t += tim.dt();
		camera.updateViewMatrix();
		dx.clear(); 
	
		shader.updateConstantVS("staticMeshBuffer", "W", &defaultMatrix);
		shader.updateConstantPS("staticMeshBuffer", "VP", &together);
		shader.apply(&dx);
		plane.draw(&dx);
		win.processMessages();
		dx.present();
	}
}