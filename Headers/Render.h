#pragma once
#include "Avatar.h"
#include "ModelGenerator.h"
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
bool isAvatarLoaded = false;
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

void displayFrontMesh() {

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
				glColor3f(1.0, 0.0, 0.0);
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
				glColor3f(1.0, 0.0, 0.0);
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
				glColor3f(1.0, 0.0, 0.0);
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
				glColor3f(1.0, 0.0, 0.0);
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
				glColor3f(1.0, 0.0, 0.0);
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
				glColor3f(1.0, 0.0, 0.0);
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
	if (isDisplayFrontMesh) {
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