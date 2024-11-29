#include "windowheader.h"
#include "adaptor.h"
#include "dxCore.h"
#include "Shader.h"
#include "Mesh.h"
#include "camera.h"
#include "GamesEngineeringBase.h"


int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
	Window win;
	DxCore dx;
	Shaders shader;
	plane plane;
	StaticMesh banana;
	FPCamera camera;
	Checkkey check;
	GamesEngineeringBase::Timer tim;
	
	win.Init(1024, 768, "MyWindow");
	dx.Init(1024, 768, win.hwnd, false);
	shader.init("Shaderv.txt", "Shaderp.txt", &dx);
	plane.init(&dx);
	banana.init(&dx, "Resources/bananaclump.gem");

	float t = 0.f;
	

	Vec4 eye = Vec4(1100.f,500.f,1100.f,1.f);
	Vec4 center = Vec4(0.f, 0.f, 0.f, 1.f);
	Vec4 up = Vec4(0.f, 1.f, 0.f, 1.f);

	camera.setPosition(eye);
	camera.setTarget(center);
	camera.setViewMatrix(eye, center, up);
	camera.setProjectionMatrix(45, 1024.f/768.f, 0.1,5000);

	Matrix44 defaultMatrix;
	Matrix44 scaledmatrix = defaultMatrix.scaling(Vec3(0.1, 0.1, 0.1));

	while (true)
	{
		win.processMessages();
		//t += tim.dt();
		//Vec4 from = Vec4(1100 * cosf(t), 500, 1100 * sinf(t),1);
		float dt = tim.dt();
		
		camera.moveForward(700.f, dt);
		camera.moveBackward(500.f, dt);
		camera.moveRight(500.f, dt);
		camera.moveLeft(500.f, dt);

		Matrix44 viewmatrix = camera.getViewMatrix();
		Matrix44 projectionmatrix = camera.getProjectionMatrix();
	    Matrix44 together = viewmatrix* projectionmatrix ;

		dx.clear(); 
	
		shader.updateConstantVS("staticMeshBuffer", "W", &defaultMatrix);
		shader.updateConstantVS("staticMeshBuffer", "VP", &together);
		shader.apply(&dx);
		plane.draw(&dx);
		banana.draw(&dx);

		dx.present();
	}
}