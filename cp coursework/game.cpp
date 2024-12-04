﻿#include "windowheader.h"
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
	StaticMesh pine;
	FPCamera camera;
	FPcamManager camman;
	Checkkey check;
	textureManager texman;
	Matrix44 defaultMatrix;
	Checkmouse checkMouse(win.hwnd);
	GamesEngineeringBase::Timer tim;

	win.Init(1024, 768, "MyWindow");
	dx.Init(1024, 768, win.hwnd, false);
	
	//shader.init("Shaderv.txt", "Shaderp.txt", &dx);
	shader.initStatic("Shaderv.txt", "Shaderp2.txt", &dx);

	plane.init(&dx);
	banana.loadMesh(&dx, "Resources/bananaclump.gem", &texman);
	pine.loadMesh(&dx, "Resources/pine.gem",&texman);

	float t = 0.f;
	
	Vec4 eye = Vec4(500.f, 80.f,500.f,1.f);
	Vec4 center = Vec4(0.f, 80.f, 0.f, 1.f);
	Vec4 up = Vec4(0.f, 1.f, 0.f, 1.f);

	/*camman.setAll( eye, center, up, 45.f, 1024.f / 768.f, 0.1f, 5000.f);*/
	camera.setPosition(eye);
	camera.setTarget(center);
	camera.setViewMatrix(eye, center, up);
	camera.setProjectionMatrix(45, 1024.f / 768.f, 0.1, 5000);

	while (true)
	{
		win.processMessages();
		float dt = tim.dt();

		/*camman.cameraMovement(checkMouse,400.f,dt);*/
		float mouseX, mouseY;
		checkMouse.getMouseMovement(mouseX, mouseY);
		camera.processMouseInput(mouseX, mouseY,400.f, dt);
		camera.updateCameraDirection();
		camera.moveForward(700.f, dt);
		camera.moveBackward(500.f, dt);
		camera.moveRight(500.f, dt);
		camera.moveLeft(500.f, dt);

		Matrix44 viewmatrix = camera.getViewMatrix();
		Matrix44 projectionmatrix = camera.getProjectionMatrix();
	    Matrix44 together = viewmatrix* projectionmatrix ;

		dx.clear(); 
	
		shader.apply(&dx);
		plane.draw(&dx, &shader, defaultMatrix, together);
		banana.draw(&dx, &shader,texman, defaultMatrix, together);
		pine.draw(&dx, &shader,texman, defaultMatrix, together);

		if (check.keyPressed(VK_ESCAPE))  break;  
		dx.present();
	}
}