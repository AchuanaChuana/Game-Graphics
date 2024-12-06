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
	Shaders shaderstyle;
	planewithTex plane;
	staticMesh bamboo;
	staticMesh flower;
	Sphere dome;
	animatedMesh gun;
	animatedMesh dinasour;
	FPCamera camera;
	Checkkey check;
	textureManager texman;
	Matrix44 defaultMatrix;
	Matrix44 biggerDefault = defaultMatrix.scaling(Vec3(100, 100, 100));
	Checkmouse checkMouse(win.hwnd);
	GamesEngineeringBase::Timer tim;
	srand(time(NULL));

	win.Init(1024, 768, "MyWindow");
	dx.Init(1024, 768, win.hwnd, false);
	
	shaderop.initStatic("Shaderv.txt", "Shaderpop.txt", &dx);
	shaderS.initStatic("Shaderv.txt", "Shaderp2.txt", &dx);
	shaderA.initAnimated("Shadervmove.txt", "Shaderp2.txt", &dx);
	shadernoTex.initStatic("Shaderv.txt", "Shaderp.txt", &dx);
	shaderstyle.initStatic("Shaderv.txt", "Shaderpline.txt", &dx);

	plane.init(&dx,&texman, std::string("Textures/snowbc.png"));
	flower.loadMesh(&dx, "Resources/flower1.gem", &texman);
	bamboo.loadMesh(&dx, "Resources/bamboo.gem",&texman);
	dinasour.loadMesh(&dx, "Resources/TRex.gem", &texman);
	dome.init(&dx, &texman, std::string("Textures/HDRI/cloudyblue.png"), 30, 30, 10000);

	float t = 0.f;
	
	Vec4 eye = Vec4(500.f, 120.f,500.f,1.f);
	Vec4 center = Vec4(0.f, 120.f, 0.f, 1.f);
	Vec4 up = Vec4(0.f, 1.f, 0.f, 1.f);

	/*camman.setAll( eye, center, up, 45.f, 1024.f / 768.f, 0.1f, 5000.f);*/
	camera.setPosition(eye);
	camera.setTarget(center);
	camera.setViewMatrix(eye, center, up);
	camera.setProjectionMatrix(45, 1024.f / 768.f, 0.1, 20000);

	vector<Matrix44> bambooPos = bamboo.generateRandomPositions(20, 3000, 1, 3000, Vec3(3,3,3));
	vector<Matrix44> flowerPos = flower.generateRandomPositions(30, 2000, 1, 2000, Vec3(1, 1, 1));
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
	
		plane.draw(&dx, &shaderS,texman, "Textures/snowbc.png", defaultMatrix, together);
		//pine.draw(&dx, &shaderop,texman, defaultMatrix, together);
		bamboo.drawManyRand(&dx, &shaderop, texman, together, bambooPos);
		flower.drawManyRand(&dx, &shaderop, texman, together, flowerPos);
		dinasour.draw(&shaderA, &dx, dt, texman, biggerDefault, together);
		dinasour.t += dt;
		dome.draw(&dx, &shaderS, texman,"Textures/HDRI/cloudyblue.png", defaultMatrix, together);

		if (check.keyPressed(VK_ESCAPE))  break;  
		dx.present();
	}
}