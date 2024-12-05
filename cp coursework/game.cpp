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
	Shaders shaderS;
	Shaders shaderA;
	Shaders shadernoTex;
	Shaders shaderop;
	//Shaders shadertile;
	planewithTex plane;
	staticMesh banana;
	staticMesh pine;
	animatedMesh gun;
	animatedMesh dinasour;
	FPCamera camera;
	FPcamManager camman;
	Checkkey check;
	textureManager texman;
	Matrix44 defaultMatrix;
	Matrix44 biggerDefault = defaultMatrix.scaling(Vec3(4, 4, 4));
	Checkmouse checkMouse(win.hwnd);
	GamesEngineeringBase::Timer tim;

	win.Init(1024, 768, "MyWindow");
	dx.Init(1024, 768, win.hwnd, false);
	
	//shader.init("Shaderv.txt", "Shaderp.txt", &dx);
	shaderop.initStatic("Shaderv.txt", "Shaderpop.txt", &dx);
	shaderS.initStatic("Shaderv.txt", "Shaderp2.txt", &dx);
	shaderA.initAnimated("Shaderv.txt", "Shaderp2.txt", &dx);
	shadernoTex.initStatic("Shaderv.txt", "Shaderp.txt", &dx);
	//shadertile.initStatic("Shaderv.txt", "Shaderptile.txt", &dx);

	plane.init(&dx,&texman, std::string("Textures/grass.png"));
	banana.loadMesh(&dx, "Resources/bananaclump.gem", &texman);
	pine.loadMesh(&dx, "Resources/pine.gem",&texman);
	dinasour.loadMesh(&dx, "Resources/TRex.gem", &texman);
	gun.loadMesh(&dx, "Resources/Automatic_Carbine_5.gem", &texman);

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
	
		plane.draw(&dx, &shaderS,texman, defaultMatrix, together);
		banana.draw(&dx, &shaderS,texman, defaultMatrix, together);
		pine.draw(&dx, &shaderop,texman, defaultMatrix, together);
		//gun.draw(&shaderA, &dx, dt, texman, defaultMatrix, together);
		gun.t += dt;
		dinasour.draw(&shaderA, &dx, dt, texman, defaultMatrix, together);
		dinasour.t += dt;

		if (check.keyPressed(VK_ESCAPE))  break;  
		dx.present();
	}
}