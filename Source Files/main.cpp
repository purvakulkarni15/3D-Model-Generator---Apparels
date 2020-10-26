#include "ModelGenerator.h"
#include "Avatar.h"
#include <GL/glew.h>
#include <stdio.h>

#define ID_SIMULATE 100
#define ID_LOAD_AVATAR_MALE_A 101
#define ID_LOAD_AVATAR_MALE_T 102
#define ID_LOAD_AVATAR_FEMALE_A 103
#define ID_LOAD_AVATAR_FEMALE_T 104
#define ID_LOAD_FRONT_MESH 105
#define ID_LOAD_BACK_MESH 106
#define ID_LOAD_IMAGE_F 107
#define ID_LOAD_IMAGE_B 108
#define ID_GENERATE_2D_MESH 109
#define ID_DISPLAY_AVATAR 110


//Global Variables
HDC g_HDC;
glm::vec2 center;
int windowWidth, windowHeight;
ModelGenerator modelGenerator;
ImageProcessor imageProcessor;
bool done = false;

float xScale = 1.0;
float yScale = 1.0;
bool scaleMode = false;
bool isFrontImageLoaded = false;
bool isBackImageLoaded = false;
bool isRender = true;
bool isDisplayAvatar = false;
bool isDisplayBackMesh = false;
bool isDisplayFrontMesh = false;
bool isLoadedAvatar = false;
bool isMeshGenerated = false;
glm::vec2 mouseClickPos; 

const char* front = NULL;
const char* back = NULL;


void initialize();
void resize(int width, int height);
void render();
void display();
void grid();
void displayAvatar();
void displayFrontMesh();
void displayBackMesh();
void setupPixelFormat(HDC hDC);

/*
	CALLBACK: calling convention
*/
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmdlineArgs, int nCmdshow)
{
	WNDCLASS wndclass;
	HWND hwnd;
	MSG msg;
	DWORD   dwStyle;
	RECT    windowRect;

	/*
		WndClass is a structure used to set the properties of the window to be created.
	*/

	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpszClassName = "Rapid Digitization Of Clothing Articles";
	wndclass.lpszMenuName = NULL;
	wndclass.lpfnWndProc = (WNDPROC)WndProc;
	wndclass.hInstance = GetModuleHandle(NULL);
	wndclass.hbrBackground = NULL;
	wndclass.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_HAND);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("TEXT INSIDE MESSAGE BOX"), TEXT("TITLE FOR THE MESSAGE BOX"), MB_OK);
		return -1;
	}

	AdjustWindowRectEx(&windowRect, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);

	int width = 1000, height = 1000;

	hwnd = CreateWindowEx(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
		"Rapid Digitization Of Clothing Articles",
		TEXT("Rapid Digitization Of Clothing Articles"),
		WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		0,
		0,
		width,
		height,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hwnd, SW_SHOW);

	
	modelGenerator = ModelGenerator();
	
	while (!done)
	{
		PeekMessage(&msg, hwnd, NULL, NULL, PM_REMOVE);

		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			if (isRender) {
				render();
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return 0;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	static HGLRC hRC;
	static HDC hDC;
	static HMENU hMenu = CreateMenu();
	static HMENU hMenuImages = CreateMenu();
	static HMENU hMenuAvatar = CreateMenu();
	int width, height;

	static vector<int> pts1;
	static vector<int> pts2;

	switch (msg)
	{
	case WM_CREATE:
	{
		hDC = GetDC(hwnd);  //get current windows device context
		g_HDC = hDC;
		setupPixelFormat(hDC); //call our pixel format setup function
		hRC = wglCreateContext(hDC);
		wglMakeCurrent(hDC, hRC);

		AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hMenuImages, "Load Images");
		AppendMenu(hMenuImages, MF_STRING, ID_LOAD_IMAGE_F, "Front image");
		AppendMenu(hMenuImages, MF_STRING, ID_LOAD_IMAGE_B, "Back Image");
		AppendMenu(hMenu, MF_STRING, ID_GENERATE_2D_MESH, "Generate 2D Mesh");
		AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hMenuAvatar, "Load Avatar");
		AppendMenu(hMenuAvatar, MF_STRING, ID_LOAD_AVATAR_FEMALE_A, "Female - A");
		AppendMenu(hMenuAvatar, MF_STRING, ID_LOAD_AVATAR_FEMALE_T, "Female - T");
		AppendMenu(hMenuAvatar, MF_STRING, ID_LOAD_AVATAR_MALE_A, "Male - A");
		AppendMenu(hMenuAvatar, MF_STRING, ID_LOAD_AVATAR_MALE_T, "Male - T");
		AppendMenu(hMenu, MF_STRING, ID_LOAD_FRONT_MESH, "Display Front Mesh");
		AppendMenu(hMenu, MF_STRING, ID_LOAD_BACK_MESH, "Display Back Mesh");
		AppendMenu(hMenu, MF_STRING, ID_DISPLAY_AVATAR, "Display Avatar");
		AppendMenu(hMenu, MF_STRING, ID_SIMULATE, "Generate 3D Model");

		SetMenu(hwnd, hMenu);

		EnableMenuItem(hMenu, ID_GENERATE_2D_MESH, MF_BYCOMMAND | MF_DISABLED);
		EnableMenuItem(hMenu, ID_LOAD_FRONT_MESH, MF_BYCOMMAND | MF_DISABLED);
		EnableMenuItem(hMenu, ID_LOAD_BACK_MESH, MF_BYCOMMAND | MF_DISABLED);
		EnableMenuItem(hMenu, ID_DISPLAY_AVATAR, MF_BYCOMMAND | MF_DISABLED);
		EnableMenuItem(hMenu, ID_SIMULATE, MF_BYCOMMAND | MF_DISABLED);


		initialize();
		break;
	}
	case WM_SIZE:
	{
		height = HIWORD(lparam);
		width = LOWORD(lparam);
		windowHeight = height;
		windowWidth = width;
		resize(width, height);
		break;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wparam))
		{
			case ID_SIMULATE:
			{
				modelGenerator.apparel.modifyObject(glm::vec3(xScale, yScale, 1.0), glm::vec3(center.x / 2.0, center.y / 2.0, 0));
				center = glm::vec2(0, 0);
				xScale = 1.0;
				yScale = 1.0;
				modelGenerator.clothSimulator.addConstraints(&modelGenerator.apparel);
				modelGenerator.intersectApparelWithAvatar();
				modelGenerator.clothSimulator.satisfyConstraints(1);
				modelGenerator.clothSimulator.satisfyConstraints(2);
				modelGenerator.stitchFrontAndBack();
				modelGenerator.storeModel(front, back);
				MessageBox(NULL, "3D Model Created", "Notification", MB_OK);
				isRender = false;
				done = true;
				break;
			}
			case ID_GENERATE_2D_MESH: 
			{
				if (!isMeshGenerated) {
					modelGenerator.generateModel(imageProcessor);
					center = glm::vec2(0, 0);
					modelGenerator.apparel.modifyObject(glm::vec3(xScale, yScale*-1, 1.0), glm::vec3(center.x / 2.0, center.y / 2.0, 0));

					EnableMenuItem(hMenu, ID_LOAD_FRONT_MESH, MF_BYCOMMAND | MF_ENABLED);
					EnableMenuItem(hMenu, ID_LOAD_BACK_MESH, MF_BYCOMMAND | MF_ENABLED);
					EnableMenuItem(hMenu, ID_SIMULATE, MF_BYCOMMAND | MF_ENABLED);

					MessageBox(NULL, "2D Mesh Created", "Notification", MB_OK);

					isMeshGenerated = !isMeshGenerated;
					EnableMenuItem(hMenu, ID_GENERATE_2D_MESH, MF_BYCOMMAND | MF_DISABLED);
				}
				break;
			}
			case ID_LOAD_IMAGE_F: {

				if (front == NULL) {

					OPENFILENAME ofn;
					TCHAR szFile[260] = { 0 };

					// Initialize OPENFILENAME
					ZeroMemory(&ofn, sizeof(ofn));
					ofn.lStructSize = sizeof(ofn);
					ofn.hwndOwner = hwnd;
					ofn.lpstrFile = szFile;
					ofn.nMaxFile = sizeof(szFile);
					ofn.lpstrFilter = "Image files (*.jpg;*.png;*.bmp)\0*.jpg;*.png;*.bmp\0";
					ofn.nFilterIndex = 1;
					ofn.lpstrFileTitle = NULL;
					ofn.nMaxFileTitle = 0;
					ofn.lpstrInitialDir = NULL;
					ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

					if (GetOpenFileName(&ofn) == TRUE)
					{
						front = ofn.lpstrFile;

						if (front != NULL) {
							imageProcessor.loadFrontImage(front);
							isFrontImageLoaded = true;
							EnableMenuItem(hMenu, ID_LOAD_IMAGE_F, MF_BYCOMMAND | MF_DISABLED);
							if (isBackImageLoaded && isLoadedAvatar) {
								EnableMenuItem(hMenu, ID_GENERATE_2D_MESH, MF_BYCOMMAND | MF_ENABLED);
							}

							MessageBox(NULL, "Front Image loaded", "Notification", MB_OK);
						}
					}
				}

				break;
			}
			case ID_LOAD_IMAGE_B: {

				if (back == NULL) {

					OPENFILENAME ofn;
					TCHAR szFile[260] = { 0 };

					// Initialize OPENFILENAME
					ZeroMemory(&ofn, sizeof(ofn));
					ofn.lStructSize = sizeof(ofn);
					ofn.hwndOwner = hwnd;
					ofn.lpstrFile = szFile;
					ofn.nMaxFile = sizeof(szFile);
					ofn.lpstrFilter = "Image files (*.jpg;*.png;*.bmp)\0*.jpg;*.png;*.bmp\0";
					ofn.nFilterIndex = 1;
					ofn.lpstrFileTitle = NULL;
					ofn.nMaxFileTitle = 0;
					ofn.lpstrInitialDir = NULL;
					ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

					if (GetOpenFileName(&ofn) == TRUE)
					{
						back = ofn.lpstrFile;
						if (back != NULL)
						{
							imageProcessor.loadBackImage(back);
							isBackImageLoaded = true;
							EnableMenuItem(hMenu, ID_LOAD_IMAGE_B, MF_BYCOMMAND | MF_DISABLED);
							if (isFrontImageLoaded && isLoadedAvatar) {
								EnableMenuItem(hMenu, ID_GENERATE_2D_MESH, MF_BYCOMMAND | MF_ENABLED);
							}

							MessageBox(NULL, "Back Image loaded", "Notification", MB_OK);
						}
					}
				}
				break;
			}
			case ID_LOAD_AVATAR_MALE_A:
			{
				modelGenerator.avatar.loadModel("C:\\Users\\DELL\\source\\repos\\RapidDigitizationOfApparels\\RapidDigitizationOfApparels\\Avatar\\FemaleT.obj");
				isLoadedAvatar = !isLoadedAvatar;
				EnableMenuItem(hMenu, ID_DISPLAY_AVATAR, MF_BYCOMMAND | MF_ENABLED);
				if (isBackImageLoaded && isFrontImageLoaded && isLoadedAvatar) {
					EnableMenuItem(hMenu, ID_GENERATE_2D_MESH, MF_BYCOMMAND | MF_ENABLED);
				}
				MessageBox(NULL, "Avatar loaded", "Notification", MB_OK);
				render();
				break;
			}
			case ID_LOAD_AVATAR_MALE_T:
			{
				modelGenerator.avatar.loadModel("C:\\Users\\DELL\\source\\repos\\RapidDigitizationOfApparels\\RapidDigitizationOfApparels\\Avatar\\MaleT.obj");
				isLoadedAvatar = !isLoadedAvatar;
				EnableMenuItem(hMenu, ID_DISPLAY_AVATAR, MF_BYCOMMAND | MF_ENABLED);
				if (isBackImageLoaded && isFrontImageLoaded && isLoadedAvatar) {
					EnableMenuItem(hMenu, ID_GENERATE_2D_MESH, MF_BYCOMMAND | MF_ENABLED);
				}
				MessageBox(NULL, "Avatar loaded", "Notification", MB_OK);
				render();
				break;
			}
			case ID_LOAD_AVATAR_FEMALE_A:
			{
				modelGenerator.avatar.loadModel("C:\\Users\\DELL\\source\\repos\\RapidDigitizationOfApparels\\RapidDigitizationOfApparels\\Avatar\\FemaleT.obj");
				isLoadedAvatar = !isLoadedAvatar;
				EnableMenuItem(hMenu, ID_DISPLAY_AVATAR, MF_BYCOMMAND | MF_ENABLED);
				if (isBackImageLoaded && isFrontImageLoaded && isLoadedAvatar) {
					EnableMenuItem(hMenu, ID_GENERATE_2D_MESH, MF_BYCOMMAND | MF_ENABLED);
				}
				MessageBox(NULL, "Avatar loaded", "Notification", MB_OK);
				render();
				break;
			}
			case ID_LOAD_AVATAR_FEMALE_T:
			{
				modelGenerator.avatar.loadModel("C:\\Users\\DELL\\source\\repos\\RapidDigitizationOfApparels\\RapidDigitizationOfApparels\\Avatar\\Female.obj");
				isLoadedAvatar = !isLoadedAvatar;
				EnableMenuItem(hMenu, ID_DISPLAY_AVATAR, MF_BYCOMMAND | MF_ENABLED);
				if (isBackImageLoaded && isFrontImageLoaded && isLoadedAvatar) {
					EnableMenuItem(hMenu, ID_GENERATE_2D_MESH, MF_BYCOMMAND | MF_ENABLED);
				}
				MessageBox(NULL, "Avatar loaded", "Notification", MB_OK);
				render();
				break;
			}
			case ID_LOAD_FRONT_MESH:
			{
				isDisplayFrontMesh = !isDisplayFrontMesh;
				render();
				break;
			}
			case ID_LOAD_BACK_MESH:
			{
				isDisplayBackMesh = !isDisplayBackMesh;
				render();
				break;
			}
			case ID_DISPLAY_AVATAR:
			{
				isDisplayAvatar = !isDisplayAvatar;
				render();
				break;
			}

		}
		break;
	}
	case WM_LBUTTONDOWN:
	{
		POINT p;
		GetCursorPos(&p);

		if (!(mouseClickPos.x == p.x && mouseClickPos.y == p.y)) {

			if (ScreenToClient(hwnd, &p))
			{
				mouseClickPos.x = p.x; // Set the x-coord variable
				mouseClickPos.y = p.y; // Set the y-coord variable

				mouseClickPos.x = mouseClickPos.x - windowWidth / 2.0;
				mouseClickPos.y = windowHeight / 2.0 - mouseClickPos.y;
			}

			modelGenerator.apparel.modifyObject(glm::vec3(xScale, yScale, 1.0), glm::vec3(center.x / 2.0, center.y / 2.0, 0));

			if (isDisplayFrontMesh) {

				int index = modelGenerator.getParticleIndex(modelGenerator.apparel.boundaryPointsF, mouseClickPos);
				
				if (index > -1) {

					if (pts1.size() != 0 && pts1[pts1.size() - 1] != index) {
						pts1.push_back(index);
						if (pts1.size() % 2 == 0 && pts1.size() > 1) {
							modelGenerator.setStitchParticles(modelGenerator.apparel.boundaryPointsF, modelGenerator.apparel.stitchingPointsF, pts1[pts1.size() - 2], pts1[pts1.size() - 1]);
						}
					}
					else if (pts1.size() == 0) {
						pts1.push_back(index);
					}
				}
			}
			if (isDisplayBackMesh) {
				int index = modelGenerator.getParticleIndex(modelGenerator.apparel.boundaryPointsB, mouseClickPos);

				if (index > -1) {

					if (pts2.size() != 0 && pts2[pts2.size() - 1] != index) {
						pts2.push_back(index);
						if (pts2.size() % 2 == 0 && pts2.size() > 1) {
							modelGenerator.setStitchParticles(modelGenerator.apparel.boundaryPointsB, modelGenerator.apparel.stitchingPointsB, pts2[pts2.size() - 2], pts2[pts2.size() - 1]);
						}
					}
					else if (pts2.size() == 0) {
						pts2.push_back(index);
					}
				}
			}

			center = glm::vec2(0, 0);
			xScale = 1.0;
			yScale = 1.0;

			render();
		}

		break;
	}
	case WM_KEYDOWN:
	{
		switch (wparam)
		{
			case VK_LEFT: {
				center = center + glm::vec2(-1.0, 0);
				if (scaleMode) {
					xScale = xScale / 1.005;
				}
				break;
			}
			case VK_RIGHT: {
				center = center + glm::vec2(1.0, 0);
				if (scaleMode) {
					xScale = xScale * 1.005;
				}
				break;
			}
			case VK_UP: {
				center = center + glm::vec2(0.0, 1.0);
				if (scaleMode) {
					yScale = yScale * 1.005;
				}
				break;
			}
			case VK_DOWN: {
				center = center + glm::vec2(0.0, -1.0);
				if (scaleMode) {
					yScale = yScale / 1.005;
				}
				break;
			}
			case 'S': {
				scaleMode = !scaleMode;
				break;
			}

		}
		break;
	}
	case WM_CLOSE:
	{
		done = true;
		wglMakeCurrent(hDC, NULL);
		wglDeleteContext(hRC);
		break;
	}
	default:
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
}

void initialize()
{
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

}

void resize(int width, int height)
{
	if (height == 0)
		height = 1;

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-width / 2, width / 2, -height / 2, height / 2, -1000.0, 1000.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glLoadIdentity();
	glPushMatrix();
	grid();
	display();
	glPopMatrix();
	glFlush();
}


void displayAvatar() {
	for (int i = 0; i < modelGenerator.avatar.triangles.size(); i++) {

		glColor3f(0.5, 0.5, 0.5);
		glBegin(GL_TRIANGLES);
		glVertex3f(modelGenerator.avatar.triangles[i].p1->vertex.x, modelGenerator.avatar.triangles[i].p1->vertex.y, modelGenerator.avatar.triangles[i].p1->vertex.z);
		glVertex3f(modelGenerator.avatar.triangles[i].p2->vertex.x, modelGenerator.avatar.triangles[i].p2->vertex.y, modelGenerator.avatar.triangles[i].p2->vertex.z);
		glVertex3f(modelGenerator.avatar.triangles[i].p3->vertex.x, modelGenerator.avatar.triangles[i].p3->vertex.y, modelGenerator.avatar.triangles[i].p3->vertex.z);
		glEnd();
	}

}

void displayFrontMesh(){

	glPointSize(5.0);

	
	for (int i = 0; i < modelGenerator.apparel.trianglesF.size(); i++) {
		glColor4f(0.2, 1.0, 0.1, 0.5);
		glBegin(GL_TRIANGLES);
		glVertex3f(modelGenerator.apparel.trianglesF[i].p1->vertex.x*xScale + center.x / 2.0, modelGenerator.apparel.trianglesF[i].p1->vertex.y*yScale + center.y / 2.0, modelGenerator.apparel.trianglesF[i].p1->vertex.z);
		glVertex3f(modelGenerator.apparel.trianglesF[i].p2->vertex.x*xScale + center.x / 2.0, modelGenerator.apparel.trianglesF[i].p2->vertex.y*yScale + center.y / 2.0, modelGenerator.apparel.trianglesF[i].p1->vertex.z);
		glVertex3f(modelGenerator.apparel.trianglesF[i].p3->vertex.x*xScale + center.x / 2.0, modelGenerator.apparel.trianglesF[i].p3->vertex.y*yScale + center.y / 2.0, modelGenerator.apparel.trianglesF[i].p1->vertex.z);
		glEnd();
	}
	
	if (!isDisplayAvatar) {

		for (int i = 0; i < modelGenerator.apparel.trianglesF.size(); i++) {

			if (modelGenerator.apparel.trianglesF[i].p1->isStitched) {
				glColor3f(1.0, 1.0, 1.0);
				glBegin(GL_POINTS);
				glVertex3f(modelGenerator.apparel.trianglesF[i].p1->vertex.x*xScale + center.x / 2.0, modelGenerator.apparel.trianglesF[i].p1->vertex.y*yScale + center.y / 2.0, modelGenerator.apparel.trianglesF[i].p1->vertex.z);
				glEnd();
			}
			else {

				glColor3f(1.0, 1.0, 0.0);
				glBegin(GL_POINTS);
				glVertex3f(modelGenerator.apparel.trianglesF[i].p1->vertex.x*xScale + center.x / 2.0, modelGenerator.apparel.trianglesF[i].p1->vertex.y*yScale + center.y / 2.0, modelGenerator.apparel.trianglesF[i].p1->vertex.z);
				glEnd();
			}

			if (modelGenerator.apparel.trianglesF[i].p2->isStitched) {
				glColor3f(1.0, 1.0, 1.0);
				glBegin(GL_POINTS);
				glVertex3f(modelGenerator.apparel.trianglesF[i].p2->vertex.x*xScale + center.x / 2.0, modelGenerator.apparel.trianglesF[i].p2->vertex.y*yScale + center.y / 2.0, modelGenerator.apparel.trianglesF[i].p1->vertex.z);
				glEnd();
			}
			else {
				glColor3f(1.0, 1.0, 0.0);
				glBegin(GL_POINTS);
				glVertex3f(modelGenerator.apparel.trianglesF[i].p2->vertex.x*xScale + center.x / 2.0, modelGenerator.apparel.trianglesF[i].p2->vertex.y*yScale + center.y / 2.0, modelGenerator.apparel.trianglesF[i].p1->vertex.z);
				glEnd();
			}

			if (modelGenerator.apparel.trianglesF[i].p3->isStitched) {
				glColor3f(1.0, 1.0, 1.0);
				glBegin(GL_POINTS);
				glVertex3f(modelGenerator.apparel.trianglesF[i].p3->vertex.x*xScale + center.x / 2.0, modelGenerator.apparel.trianglesF[i].p3->vertex.y*yScale + center.y / 2.0, modelGenerator.apparel.trianglesF[i].p1->vertex.z);
				glEnd();
			}
			else {
				glColor3f(1.0, 1.0, 0.0);
				glBegin(GL_POINTS);
				glVertex3f(modelGenerator.apparel.trianglesF[i].p3->vertex.x*xScale + center.x / 2.0, modelGenerator.apparel.trianglesF[i].p3->vertex.y*yScale + center.y / 2.0, modelGenerator.apparel.trianglesF[i].p1->vertex.z);
				glEnd();
			}
		}
	}

}

void displayBackMesh() {


	glPointSize(5.0);

	for (int i = 0; i < modelGenerator.apparel.trianglesB.size(); i++) {
		glColor4f(0.2, 1.0, 0.1, 0.5);
		glBegin(GL_TRIANGLES);
		glVertex3f(modelGenerator.apparel.trianglesB[i].p1->vertex.x*xScale + center.x / 2.0, modelGenerator.apparel.trianglesB[i].p1->vertex.y*yScale + center.y / 2.0, modelGenerator.apparel.trianglesB[i].p1->vertex.z);
		glVertex3f(modelGenerator.apparel.trianglesB[i].p2->vertex.x*xScale + center.x / 2.0, modelGenerator.apparel.trianglesB[i].p2->vertex.y*yScale + center.y / 2.0, modelGenerator.apparel.trianglesB[i].p2->vertex.z);
		glVertex3f(modelGenerator.apparel.trianglesB[i].p3->vertex.x*xScale + center.x / 2.0, modelGenerator.apparel.trianglesB[i].p3->vertex.y*yScale + center.y / 2.0, modelGenerator.apparel.trianglesB[i].p3->vertex.z);
		glEnd();
	}

	if (!isDisplayAvatar) {

		for (int i = 0; i < modelGenerator.apparel.trianglesB.size(); i++) {

			if (modelGenerator.apparel.trianglesB[i].p1->isStitched) {
				glColor3f(1.0, 1.0, 1.0);
				glBegin(GL_POINTS);
				glVertex3f(modelGenerator.apparel.trianglesB[i].p1->vertex.x*xScale + center.x / 2.0, modelGenerator.apparel.trianglesB[i].p1->vertex.y*yScale + center.y / 2.0, modelGenerator.apparel.trianglesB[i].p1->vertex.z);
				glEnd();
			}
			else {

				glColor3f(1.0, 1.0, 0.0);
				glBegin(GL_POINTS);
				glVertex3f(modelGenerator.apparel.trianglesB[i].p1->vertex.x*xScale + center.x / 2.0, modelGenerator.apparel.trianglesB[i].p1->vertex.y*yScale + center.y / 2.0, modelGenerator.apparel.trianglesB[i].p1->vertex.z);
				glEnd();
			}

			if (modelGenerator.apparel.trianglesB[i].p2->isStitched) {
				glColor3f(1.0, 1.0, 1.0);
				glBegin(GL_POINTS);
				glVertex3f(modelGenerator.apparel.trianglesB[i].p2->vertex.x*xScale + center.x / 2.0, modelGenerator.apparel.trianglesB[i].p2->vertex.y*yScale + center.y / 2.0, modelGenerator.apparel.trianglesB[i].p2->vertex.z);
				glEnd();
			}
			else {
				glColor3f(1.0, 1.0, 0.0);
				glBegin(GL_POINTS);
				glVertex3f(modelGenerator.apparel.trianglesB[i].p2->vertex.x*xScale + center.x / 2.0, modelGenerator.apparel.trianglesB[i].p2->vertex.y*yScale + center.y / 2.0, modelGenerator.apparel.trianglesB[i].p2->vertex.z);
				glEnd();
			}

			if (modelGenerator.apparel.trianglesB[i].p3->isStitched) {
				glColor3f(1.0, 1.0, 1.0);
				glBegin(GL_POINTS);
				glVertex3f(modelGenerator.apparel.trianglesB[i].p3->vertex.x*xScale + center.x / 2.0, modelGenerator.apparel.trianglesB[i].p3->vertex.y*yScale + center.y / 2.0, modelGenerator.apparel.trianglesB[i].p3->vertex.z);
				glEnd();
			}
			else {
				glColor3f(1.0, 1.0, 0.0);
				glBegin(GL_POINTS);
				glVertex3f(modelGenerator.apparel.trianglesB[i].p3->vertex.x*xScale + center.x / 2.0, modelGenerator.apparel.trianglesB[i].p3->vertex.y*yScale + center.y / 2.0, modelGenerator.apparel.trianglesB[i].p3->vertex.z);
				glEnd();
			}
		}
	}

}
void display(void)
{
	if (isDisplayAvatar) {
		displayAvatar();
	}
	if(isDisplayFrontMesh) {
		displayFrontMesh();
	}
	if (isDisplayBackMesh) {
		displayBackMesh();
	}
	SwapBuffers(g_HDC);
}

void grid()
{
	for (int i = -windowWidth / 2; i < windowWidth / 2; i += 10)
	{
		glColor3f(0.2, 0.2, 0.2);

		glBegin(GL_LINES);

		glVertex3f(i, -windowHeight / 2, -20.f);
		glVertex3f(i, windowHeight / 2, -20.f);

		glEnd();
	}

	for (int i = -windowHeight / 2; i < windowHeight / 2; i += 10)
	{
		glColor3f(0.2, 0.2, 0.2);
		glBegin(GL_LINES);

		glVertex3f(-windowWidth / 2, i, -20.f);
		glVertex3f(windowWidth / 2, i, -20.f);

		glEnd();
	}
}

void setupPixelFormat(HDC hDC)
{
	int nPixelFormat;

	static PIXELFORMATDESCRIPTOR pfd = {
			sizeof(PIXELFORMATDESCRIPTOR),
			1,
			PFD_DRAW_TO_WINDOW |
			PFD_SUPPORT_OPENGL |
			PFD_DOUBLEBUFFER,
			PFD_TYPE_RGBA,
			32,
			0, 0, 0, 0, 0, 0,
			0,
			0,
			0,
			0, 0, 0, 0,
			16,
			0,
			0,
			PFD_MAIN_PLANE,
			0,
			0, 0, 0 };

	nPixelFormat = ChoosePixelFormat(hDC, &pfd);
	SetPixelFormat(hDC, nPixelFormat, &pfd);
}
