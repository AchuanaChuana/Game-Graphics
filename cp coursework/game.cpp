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
	DxCore dxdef;

	Shaders shaderS;
	Shaders shaderA;
	Shaders shaderAnor;
	Shaders shadernoTex;
	Shaders shaderop;
	Shaders shaderopnor;
	Shaders shaderstyle;
	Shaders shaderwater;
	Shaders shadertile;
	Shaders shaderswing;
	//Shaders shaderdefswing;         

	//LightingPass lighting;

	planewithTex plane;
	planewithTex pondground;
	staticMesh bamboo;
	DrawBamboo bambooo;
	staticMesh flower;
	Box box;
	Box box2;
	Sphere dome;
	drawDinosaur dina(3000.f,3000.f);
	Snow snow;
	WaterPlane water;

	FPCamera camera;
	Checkkey check;
	textureManager texman;
	textureManager texmandef;
	Matrix44 defaultMatrix;
	Matrix44 biggerDefault = defaultMatrix.scaling(Vec3(100, 100, 100));
	Checkmouse checkMouse(win.hwnd);
	GamesEngineeringBase::Timer tim;
	srand(time(NULL));

	win.Init(1024, 768, "MyWindow");
	dx.Init(1024, 768, win.hwnd, false);
	//dxdef.InitDeferred(1024, 768, win.hwnd, false);              (Initialize DefferedShading)
	
	shaderop.initStatic("Shaderv.txt", "Shaderpop.txt", &dx);
	shaderopnor.initStatic("Shaderv.txt", "Shaderpopnormal.txt", &dx);
	shaderS.initStatic("Shaderv.txt", "Shaderp2.txt", &dx);
	shaderA.initAnimated("Shadervmove.txt", "Shaderp2.txt", &dx);
	shaderAnor.initAnimated("Shadervmove.txt", "Shaderpopnormal.txt", &dx);
	shadernoTex.initStatic("Shaderv.txt", "Shaderp.txt", &dx);
	shaderwater.initStatic("Shadervwater.txt", "Shaderpwater.txt", &dx);
	shadertile.initStatic("Shaderv.txt", "Shaderptile.txt", &dx);
	shaderswing.initStatic("ShadervSwing.txt", "Shaderpopnormal.txt", &dx);
	//shaderdefswing.initDeferred("ShadervSwing.txt", "Shaderpdefopnormal.txt", &dxdef);          (read the vertex and pixel shader)

	//lighting.initLighting(&dxdef, "Lightingvs.txt", "Lightingps.txt");                 (Initialize lightingpass)

	plane.init(&dx,&texman, std::string("Textures/snowbc.png"),10000.f);
	pondground.init(&dx, &texman, std::string("Textures/pondbc.png"),2000.f);
	flower.loadMesh(&dx, "Resources/flower1.gem", &texman);
	bamboo.loadMesh(&dx, "Resources/bamboo.gem",&texman);
	bambooo.loadMesh(&dx, "Resources/bamboo.gem", &texman);
	//bambooo.loadMesh(&dxdef, "Resources/bamboo.gem", &texmandef);       (Load Bambooo with the deffered shading mode)
	dina.loadMesh(&dx, "Resources/TRex.gem", &texman);
	dome.init(&dx, &texman, std::string("Textures/HDRI/cloudyblue.png"), 30, 30, 30000);
	box.init(&dx, &texman, std::string("Textures/wood.png"), 2030.f, 120.f, 30.f);
	box2.init(&dx, &texman, std::string("Textures/wood.png"), 30.f, 120.f, 2030.f);
	snow.init(&dx, &texman, 4, 4, 20, 200);
	water.init(&dx, &texman, std::string("Textures/water.png"),80);

	float t = 0.f;
	
	Vec4 eye = Vec4(500.f, 120.f,500.f,1.f);
	Vec4 center = Vec4(0.f, 120.f, 0.f, 1.f);
	Vec4 up = Vec4(0.f, 1.f, 0.f, 1.f);

	camera.setPosition(eye);
	camera.setTarget(center);
	camera.setViewMatrix(eye, center, up);
	camera.setProjectionMatrix(45, 1024.f / 768.f, 0.1, 50000);

	vector<Matrix44> bambooPos = bamboo.generateRandomPositions(30, 3000, 1, 3000, Vec3(3,3,3));
	vector<Matrix44> flowerPos = flower.generateRandomPositions(100, 3000, 1, 3000, Vec3(1, 1, 1));
	vector<Matrix44> bamboooPos = bambooo.generateRandomPositions(30, 3000, 1, 3000, Vec3(3, 3, 3));

	//Vec3 LightDirection = Vec3(0.0f, -1.0f, 0.0f);
	//float LightIntensity = 1.0f;
	//Vec3 LightColor = Vec3(1.0f, 1.0f, 1.0f);
	//float padding = 0.0f;                                        (data for LightingPass Cbuffer)


	while (true)
	{
		win.processMessages();
		float dt = tim.dt();

		float mouseX, mouseY;
		checkMouse.getMouseMovement(mouseX, mouseY);
		camera.processMouseInput(mouseX, mouseY,20.f, dt);
		camera.updateCameraDirection();
		camera.moveForward(700.f, dt);
		camera.moveBackward(500.f, dt);
		camera.moveRight(500.f, dt);
		camera.moveLeft(500.f, dt);

		Matrix44 viewmatrix = camera.getViewMatrix();
		Matrix44 projectionmatrix = camera.getProjectionMatrix();
	    Matrix44 together = viewmatrix* projectionmatrix ;

		dx.clear(); 
		//dxdef.setGBufferRenderTargets();      (set Gbuffer RenderTarget)
		//dxdef.clearGBuffer();
	
		snow.update(dt);
		water.update(dt);
		bambooo.update(dt);

		/*lighting.lightingShaders.updateLightingPS("LightingParams", "LightDirection", &LightDirection.x);               (Upload data to cBuffer)
		lighting.lightingShaders.updateLightingPS("LightingParams", "LightIntensity", &LightIntensity);
		lighting.lightingShaders.updateLightingPS("LightingParams", "LightColor", &LightColor.x);*/
	
		plane.draw(&dx, &shadertile,texman,20.f, "Textures/snowbc.png", defaultMatrix, together);
		bambooo.drawb(&dx, &shaderswing, texman, defaultMatrix, together);
		//bambooo.drawb(&dxdef, &shaderdefswing, texmandef, defaultMatrix, together);                               (Draw bamboo with deffered shading)
		bambooo.drawManyRandb(&dx, &shaderswing, texman, together, bamboooPos);
		flower.drawManyRand(&dx, &shaderopnor, texman, together, flowerPos);
		dina.draw(&shaderA, &dx, dt, texman, together);
		box.draw(&dx, &shaderS, texman, "Textures/wood.png", defaultMatrix.translation(Vec3(0, 0, 1000)), together);
		box.draw(&dx, &shaderS, texman, "Textures/wood.png", defaultMatrix.translation(Vec3(0, 0, -1000)), together);
		box2.draw(&dx, &shaderS, texman, "Textures/wood.png", defaultMatrix.translation(Vec3(1000, 0, 0)), together);
		box2.draw(&dx, &shaderS, texman, "Textures/wood.png", defaultMatrix.translation(Vec3(-1000, 0, 0)), together);
		snow.drawManyRand(&dx, &shadernoTex,  together);
		dome.draw(&dx, &shaderS, texman,"Textures/HDRI/cloudyblue.png", defaultMatrix, together);
		water.draw(&dx, &shaderwater,texman,"Textures/water.png", 1.0f,defaultMatrix.translation(Vec3(0,25,0)), together);

		if (check.keyPressed(VK_ESCAPE))  break;  

		//dxdef.devicecontext->OMSetRenderTargets(1, &dxdef.backbufferRenderTargetView, dxdef.depthStencilView);
		//dxdef.clear();
		//lighting.renderLightingPass(&dxdef, dxdef.gBufferSRVs);                                 (apply the lightingpass)

		dx.present();
		//dxdef.present();             (present with deffered shading)
	}
}