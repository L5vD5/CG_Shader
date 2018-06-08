#include <fstream>
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>
#include <Windows.h>
#include <iostream>
#include <GL/glew.h>
#include <gl.h>
#include <glu.h>
#include <glut.h>
#include "Matrix.h"
#include "WaveFrontOBJ.h"
#include <Shader.h>


using namespace std;

//윈도우 사이즈
int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;

//카메라
float	g_Zoom;
float	g_Rotate[2];
float	g_Translate[2];

//마우스
int				g_Mouse_Coord[2];
unsigned char	g_Mouse_Event[3];

int width=800;
int height=600;

Shader lectureShader;

GLfloat vertices[] = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f, 0.5f, 0.0f
};

GLuint VBO, VAO;
void InitCamera(){

	// initialize camera frame transforms.
	for (int i = 0; i < cameraCount; i++)
	{
		float* c = cameras[i];													// 'c' points the coordinate of i-th camera.
		wld2cam.push_back(Matrix());											// Insert {0} matrix to wld2cam vector.
		glPushMatrix();															// Push the current matrix of GL into stack.

		glLoadIdentity();														// Set the GL matrix Identity matrix.
		gluLookAt(c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7], c[8]);		// Setting the coordinate of camera.
		glGetFloatv(GL_MODELVIEW_MATRIX, wld2cam[i].matrix());					// Read the world-to-camera matrix computed by gluLookAt.
		cam2wld.push_back(wld2cam[i].inverse());								// Get the camera-to-world matrix.

		glPopMatrix();															// Transfer the matrix that was pushed the stack to GL.
	}
	cameraIndex = 0;
}





void drawCamera()
{
	int i;
	// set viewing transformation.
	glLoadMatrixf(wld2cam[cameraIndex].matrix());

	// draw cameras.
	for (i = 0; i < (int)wld2cam.size(); i++)
	{
		if (i != cameraIndex)
		{
			glPushMatrix();													// Push the current matrix on GL to stack. The matrix is wld2cam[cameraIndex].matrix().
			glMultMatrixf(cam2wld[i].matrix());								// Multiply the matrix to draw i-th camera.
			if (selectMode == 0)											// selectMode == 1 means backbuffer mode.
			{
				drawAxis(5);												// Draw x, y, and z axis.
			}
			else
			{
				float r, g, b;
				glDisable(GL_LIGHTING);										// Disable lighting in backbuffer mode.
				munge(i + 1, r, g, b);										// Match the corresponding (i+1)th color to r, g, b. You can change the color of camera on backbuffer.
				glColor3f(r, g, b);											// Set r, g, b the color of camera.
			}

			glutSolidSphere(1.0, 10, 10);

			glPopMatrix();													// Call the matrix on stack. wld2cam[cameraIndex].matrix() in here.
		}
	}
}





/*********************************************************************************
* Draw 'cow' object.
**********************************************************************************/
void InitCow(){
	cow = new WaveFrontOBJ("cow_pa4.obj");

	glPushMatrix();											// Push the current matrix of GL into stack.
	glLoadIdentity();										// Set the GL matrix Identity matrix.
	glTranslatef(0, -cow->bbmin.pos.y, -8);					// Set the location of cow.
	glRotatef(-90, 0, 1, 0);								// Set the direction of cow. These information are stored in the matrix of GL.
	glGetFloatv(GL_MODELVIEW_MATRIX, cow2wld.matrix());		// Read the modelview matrix about location and direction set above, and store it in cow2wld matrix.
	glPopMatrix();											// Pop the matrix on stack to GL.

}





void drawCow()
{
	glPushMatrix();											// Push the current matrix of GL into stack. This is because the matrix of GL will be change while drawing cow.

	// The information about location of cow to be drawn is stored in cow2wld matrix.
	// If you change the value of the cow2wld matrix or the current matrix, cow would rotate or move.
	glMultMatrixf(cow2wld.matrix());

	if (selectMode == 0)									// selectMode == 1 means backbuffer mode.
	{
		drawAxis(5);										// Draw x, y, and z axis.
	}
	else
	{
		float r, g, b;
		glDisable(GL_LIGHTING);								// Disable lighting in backbuffer mode.
		munge(32, r, g, b);									// Match the corresponding constant color to r, g, b. You can change the color of camera on backbuffer
		glColor3f(r, g, b);
	}
	

	cow->Draw();											
	/*******************************************************************/
	//(PA #4) : cow object의 normal을 그리는 함수를 추가하십시오.
	/*******************************************************************/
	if (FN == true)
		cow->Draw_FN();
	if (VN == true)
		cow->Draw_VN();

	glPopMatrix();											// Pop the matrix in stack to GL. Change it the matrix before drawing cow.
}





void Initbunny(){
	bunny = new WaveFrontOBJ("bunny_pa4.obj");

	glPushMatrix();											// Push the current matrix of GL into stack.
	glLoadIdentity();										// Set the GL matrix Identity matrix.
	glTranslatef(0, -bunny->bbmin.pos.y, 8);					// Set the location of bunny.
	glRotatef(180, 0, 1, 0);								// Set the direction of bunny. These information are stored in the matrix of GL.
	glGetFloatv(GL_MODELVIEW_MATRIX, bunny2wld.matrix());		// Read the modelview matrix about location and direction set above, and store it in bunny2wld matrix.
	glPopMatrix();											// Pop the matrix on stack to GL.

}





void drawbunny()
{
	glPushMatrix();		// Push the current matrix of GL into stack. This is because the matrix of GL will be change while drawing bunny.

	// The information about location of bunny to be drawn is stored in bunny2wld matrix.
	// If you change the value of the bunny2wld matrix or the current matrix, bunny would rotate or move.
	glMultMatrixf(bunny2wld.matrix());

	if (selectMode == 0)									// selectMode == 1 means backbuffer mode.
	{
		drawAxis(8);										// Draw x, y, and z axis.
	}
	else
	{
		float r, g, b;
		glDisable(GL_LIGHTING);								// Disable lighting in backbuffer mode.
		munge(33, r, g, b);									// Match the corresponding constant color to r, g, b. You can change the color of camera on backbuffer
		glColor3f(r, g, b);
	}
	
	bunny->Draw();											// Draw the bunny 
	/*******************************************************************/
	//(PA #4) : bunny object의 normal을 그리는 함수를 추가하십시오.
	/*******************************************************************/
	if(FN == true)
		bunny->Draw_FN();
	if(VN == true)
		bunny->Draw_VN();

	glPopMatrix();											// Pop the matrix in stack to GL. Change it the matrix before drawing bunny.
}





/*********************************************************************************
* Draw floor on 3D plane.
**********************************************************************************/
void drawFloor()
{
	if (frame == 0)
	{
		// Initialization part.
		// After making checker-patterned texture, use this repetitively.

		// Insert color into checker[] according to checker pattern.
		const int size = 8;
		unsigned char checker[size*size * 3];
		for (int i = 0; i < size*size; i++)
		{
			if (((i / size) ^ i) & 1)
			{
				checker[3 * i + 0] = 100;
				checker[3 * i + 1] = 100;
				checker[3 * i + 2] = 100;
			}
			else
			{
				checker[3 * i + 0] = 200;
				checker[3 * i + 1] = 200;
				checker[3 * i + 2] = 200;
			}
		}

		// Make texture which is accessible through floorTexID. 
		glGenTextures(1, &floorTexID);
		glBindTexture(GL_TEXTURE_2D, floorTexID);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, checker);
	}

	glDisable(GL_LIGHTING);

	// Set background color.
	if (selectMode == 0)
		glColor3d(0.35, 0.2, 0.1);
	else
	{
		// In backbuffer mode.
		float r, g, b;
		munge(34, r, g, b);
		glColor3f(r, g, b);
	}

	// Draw background rectangle.
	glBegin(GL_POLYGON);
	glVertex3f(-2000, -0.2, -2000);
	glVertex3f(-2000, -0.2, 2000);
	glVertex3f(2000, -0.2, 2000);
	glVertex3f(2000, -0.2, -2000);
	glEnd();


	// Set color of the floor.
	if (selectMode == 0)
	{
		// Assign checker-patterned texture.
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, floorTexID);
	}
	else
	{
		// Assign color on backbuffer mode.
		float r, g, b;
		munge(35, r, g, b);
		glColor3f(r, g, b);
	}

	// Draw the floor. Match the texture's coordinates and the floor's coordinates resp. 
	glBegin(GL_POLYGON);
	glTexCoord2d(0, 0);
	glVertex3d(-12, -0.1, -12);		// Texture's (0,0) is bound to (-12,-0.1,-12).
	glTexCoord2d(0, 1);
	glVertex3d(-12, -0.1, 12);		// Texture's (0,1) is bound to (-12,-0.1,12).
	glTexCoord2d(1, 1);
	glVertex3d(12, -0.1, 12);		// Texture's (1,1) is bound to (12,-0.1,12).
	glTexCoord2d(1, 0);
	glVertex3d(12, -0.1, -12);		// Texture's (1,0) is bound to (12,-0.1,-12).

	glEnd();

	if (selectMode == 0)
	{
		glDisable(GL_TEXTURE_2D);
		drawAxis(5);				// Draw x, y, and z axis.
	}
}





void Lighting()
{
	/*******************************************************************/
	//(PA #4) : 다양한 광원을 구현하십시오.
	//1. Point light / Directional light / Spotlight를 서로 다른 색으로 구현하십시오.
	//2. 광원의 위치를 구(sphere)로 표현하십시오.
	//3. Directional light / Spotlight의 경우 빛의 진행방향을 선분으로 표현하십시오.
	/*******************************************************************/
	// Define lighting for the scene

	//불  켜기
	if (dlight) {
		float light_pos[] = { 1.0, 1.0, 1.0, 0.0 };//Directional Light
		float light_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
		float light_diffuse[] = { 0.9, 0.9, 0.9, 1.0 };
		float light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
		glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
		glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
		glEnable(GL_LIGHT0);
	}
	if(plight){
		float light_pos[] = { 1.0, 1.0, 1.0, 1.0f };//Point Light
		float light_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
		float light_diffuse[] = { 0.9, 0.9, 0.9, 1.0 };
		float light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
		glLightfv(GL_LIGHT1, GL_POSITION, light_pos);
		glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
		glEnable(GL_LIGHT1); 
	}
	if (slight)
	{
		float light_pos[] = { 1.0, 1.0, 1.0, 1.0f };//Spot Light
		float light_dir[] = { -0.1f, -0.1f,-0.2f };
		float light_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
		float light_diffuse[] = { 0.9, 0.9, 0.9, 1.0 };
		float light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient);
		glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse);
		glLightfv(GL_LIGHT2, GL_POSITION, light_pos);
		glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, light_dir);
		glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular);
		glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 60.0f);
		glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 2.0f);
		glEnable(GL_LIGHT2);

	}
	
	//불 끄기
	if (!dlight)
	{
		glDisable(GL_LIGHT0);
	}
	if (!plight)
	{
		glDisable(GL_LIGHT1);
	}
	if (!slight)
	{
		glDisable(GL_LIGHT2); 
	}
}





/*********************************************************************************
* Call this part whenever display events are needed.
* Display events are called in case of re-rendering by OS. ex) screen movement, screen maximization, etc.
* Or, user can occur the events by using glutPostRedisplay() function directly.
* this part is called in main() function by registering on glutDisplayFunc(display).
**********************************************************************************/
void display()
{
	if (selectMode == 0)
		glClearColor(0, 0.6, 0.8, 1);								// Clear color setting
	else
		glClearColor(0, 0, 0, 1);									// When the backbuffer mode, clear color is set to black

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);				// Clear the screen

	if (shademodel)
		glShadeModel(GL_FLAT);										// Set Flat Shading
	else
		glShadeModel(GL_SMOOTH);

	drawCamera();													// and draw all of them.
	drawFloor();													// Draw floor.

	drawCow();														// Draw cow.
	drawbunny();


	Lighting();

	glFlush();

	// If it is not backbuffer mode, swap the screen. In backbuffer mode, this is not necessary because it is not presented on screen.
	if (selectMode == 0)
		glutSwapBuffers();

	frame++;
}





/*********************************************************************************
* Call this part whenever size of the window is changed.
* This part is called in main() function by registering on glutReshapeFunc(reshape).
**********************************************************************************/
void reshape(int w, int h)
{
	width = w;
	height = h;
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);            // Select The Projection Matrix
	glLoadIdentity();                       // Reset The Projection Matrix
	float aspect = width / float(height);
	gluPerspective(45, aspect, 1, 1024);

	glMatrixMode(GL_MODELVIEW);             // Select The Modelview Matrix
	glLoadIdentity();                       // Reset The Projection Matrix
}





//------------------------------------------------------------------------------
void initialize()
{
	// Set up OpenGL state
	glEnable(GL_DEPTH_TEST);         // Enables Depth Testing
	// Initialize the matrix stacks
	reshape(width, height);
	

	InitCamera();
	InitCow();
	Initbunny();
}





/*********************************************************************************
* Call this part whenever mouse button is clicked.
* This part is called in main() function by registering on glutMouseFunc(onMouseButton).
**********************************************************************************/
void onMouseButton(int button, int state, int x, int y)
{
	y = height - y - 1;
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			printf("Left mouse click at (%d, %d)\n", x, y);

			// After drawing object on backbuffer, you can recognize which object is selected by reading pixel of (x, y).
			// Change the value of selectMode to 1, then draw the object on backbuffer when display() function is called. 
			selectMode = 1;
			display();
			glReadBuffer(GL_BACK);
			unsigned char pixel[3];
			glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
			int pixel_value = unmunge(pixel[0], pixel[1], pixel[2]);
			printf("pixel = %d\n", pixel_value);

			if (picking) {
				/*******************************************************************/
				//(PA #2,#3) 수행한 내용 추가
				/*******************************************************************/
				if (pixel_value == 32)
				{
					whoisselected = "cow";//소가 선택되면 index를 소로 변환
				}
				else if (pixel_value == 33)
				{
					whoisselected = "bunny";//토끼가 선택되면 index를 토끼로 변환
				}

				picking = false;
			}

			selectMode = 0;
			// Save current clicked location of mouse here, and then use this on onMouseDrag function. 
			oldX = x;
			oldY = y;
		}
	}

	glutPostRedisplay();
}





/*********************************************************************************
* Call this part whenever user drags mouse.
* Input parameters x, y are coordinate of mouse on dragging.
* Value of global variables oldX, oldY is stored on onMouseButton,
* Then, those are used to verify value of x - oldX,  y - oldY to know its movement.
**********************************************************************************/
void onMouseDrag(int x, int y)
{
	y = height - y - 1;
	printf("in drag (%d, %d)\n", x - oldX, y - oldY);
	
	/*******************************************************************/
	//(PA #2,#3)  수행한 내용 추가
	/*******************************************************************/
	if (v_or_m == 'm')
	{
		if (index == 'x' && x - oldX > 0)//x키 상태에서 마우스의 x포인터가 증가할 때
		{
			isminus = false;
		}
		else if (index == 'y' && y - oldY > 0)//y키 상태에서 마우스의 y포인터가 증가할 때
		{
			isminus = false;
		}

		else if (index == 'x' && x - oldX < 0)//x키 상태에서 마우스의 x포인터가 감소할 때
		{
			isminus = true;
		}
		else if (index == 'y' && y - oldY < 0)//y키 상태에서 마우스의 y포인터가 감소할 때
		{
			isminus = true;
		}

		translate();
	}
	else if (v_or_m == 'v')
	{
		if (index == 'r')
		{
			if (x - oldX > 0)//z키 상태에서 마우스의 y포인터가 증가할 때
			{
				isminus = false;
			}
			else if (x - oldX < 0)
			{
				isminus = true;
			}
			//x축을 기준으로 드래그 거리에 따라 회전하게 하십시오.
			rotate();
		}
		else if (index == 'x' || index == 'y')
		{
			translate_x = x - oldX;
			translate_y = y - oldY;
			translate();
		}
		else if (index == 'z')
		{
			if (x - oldX > 0)//z키 상태에서 마우스의 y포인터가 증가할 때
			{
				isminus = false;
			}
			else if (x - oldX < 0)
			{
				isminus = true;
			}
			//카메라 축을 기준으로 z축 평행이동
			translate();
		}
	}

	oldX = x;
	oldY = y;
	glutPostRedisplay();
}





/*********************************************************************************
* Call this part whenever user types keyboard.
* This part is called in main() function by registering on glutKeyboardFunc(onKeyPress).
**********************************************************************************/
void onKeyPress(unsigned char key, int x, int y)
{
	//카메라 시점 변경
	if ((key >= '1') && (key <= '5'))
		cameraIndex = key - '1';

	/*******************************************************************/
	//(PA #2,#3)  수행한 내용 추가
	/*******************************************************************/
	switch (key)
	{
		//selectMode를 통해 과제2랑 과제3을 선택
	case 'm'://m키로 모드 변환
		v_or_m = 'm';
		printf("M pressed \n");
		break;
	case 'v'://v키로 모드 변환
		v_or_m = 'v';
		index = 'r';
		printf("V pressed \n");
		break;
	case 'x':
		index = 'x';//x를 누르면 x축으로 이동방향을 정함
		break;
	case 'y':
		index = 'y';//y를 누르면 y축으로 이동방향을 정함
		break;
	case 'z':
		index = 'z';//z를 누르면 z축으로 이동방향을 정함
		break;
	case 'p':
		picking = true;
		break;
	case 'r':
		if (v_or_m == 'm')
		{
			printf("R pressed \n");
			if (isrotating == false)
			{
				time = 359;
				rotate();
			}
			else
			{
				time = 0;
			}
		}
		else if (v_or_m == 'v')
		{
			index = 'r';
		}
		break;
	case 'f'://f키로 FaceNormal을 껐다 킴
		if (FN == true)
			FN = false;
		else
			FN = true;
		break;
	case 'n'://n키로 VertexNormal을 껐다 킴
		if (VN == true)
			VN = false;
		else
			VN = true;
		break;
	case 27:
		exit(0);
		break;

	}

	if (key == 's') {
		shademodel = !shademodel;
		cow->isFlat = shademodel;
		bunny->isFlat = shademodel;
	}

	glutPostRedisplay();
}






void  SpecialKey(int key, int x, int y)
{
	/*******************************************************************/
	//(PA #4) : F1 / F2 / F3 버튼에 따라 서로 다른 광원이 On/Off 되도록 구현하십시오.
	/*******************************************************************/
	
	switch (key)
	{
	case GLUT_KEY_F1:
		dlight = !dlight;
		break;
	case GLUT_KEY_F2:
		plight = !plight;
		break;
	case GLUT_KEY_F3:
		slight = !slight;
		break;
	}
	
	glutPostRedisplay();
}





void rotate(){
	glPushMatrix();
	glLoadIdentity();

	/*******************************************************************/
	//(PA #2,#3)  수행한 내용 추가
	/*******************************************************************/
	//'r'키로 회전상태가 ON 되었을 경우 선택된 축에 대해 자동으로 회전되게 하십시오.
	if (whoisselected == "bunny")
	{
		if (v_or_m == 'v')
		{
			temp = bunny2wld;

			//매트릭스 배열의 0,1,2,4,5,6,8,9,10번 값을 변환해서 축을 카메라 축으로 변환함
			bunny2wld.matrix()[0] = cam2wld[cameraIndex].matrix()[0];
			bunny2wld.matrix()[1] = cam2wld[cameraIndex].matrix()[1];
			bunny2wld.matrix()[2] = cam2wld[cameraIndex].matrix()[2];
			bunny2wld.matrix()[4] = cam2wld[cameraIndex].matrix()[4];
			bunny2wld.matrix()[5] = cam2wld[cameraIndex].matrix()[5];
			bunny2wld.matrix()[6] = cam2wld[cameraIndex].matrix()[6];
			bunny2wld.matrix()[8] = cam2wld[cameraIndex].matrix()[8];
			bunny2wld.matrix()[9] = cam2wld[cameraIndex].matrix()[9];
			bunny2wld.matrix()[10] = cam2wld[cameraIndex].matrix()[10];
		}
		glMultMatrixf(bunny2wld.matrix()); //load current transformation matrix of object
	}
	else if (whoisselected == "cow")
	{
		if (v_or_m == 'v')
		{
			temp = cow2wld;
			//매트릭스 배열의 0,1,2,4,5,6,8,9,10번 값을 변환해서 축을 카메라 축으로 변환함
			cow2wld.matrix()[0] = cam2wld[cameraIndex].matrix()[0];
			cow2wld.matrix()[1] = cam2wld[cameraIndex].matrix()[1];
			cow2wld.matrix()[2] = cam2wld[cameraIndex].matrix()[2];
			cow2wld.matrix()[4] = cam2wld[cameraIndex].matrix()[4];
			cow2wld.matrix()[5] = cam2wld[cameraIndex].matrix()[5];
			cow2wld.matrix()[6] = cam2wld[cameraIndex].matrix()[6];
			cow2wld.matrix()[8] = cam2wld[cameraIndex].matrix()[8];
			cow2wld.matrix()[9] = cam2wld[cameraIndex].matrix()[9];
			cow2wld.matrix()[10] = cam2wld[cameraIndex].matrix()[10];


		}
		glMultMatrixf(cow2wld.matrix()); //load current transformation matrix of object

	}

	int x = 1, y = 0, z = 0; // 도는 방향을 정해주는 변수

	if (v_or_m == 'm')
		isrotating = true;//도는 중이면 다시 못 돌리기 위해 있는 변수

						  //index 에 따라 회전 방향을 바꿈
	if (v_or_m == 'm')
	{
		if (index == 'x')
		{
			x = 1;
			y = 0;
			z = 0;
		}
		else if (index == 'y')
		{
			x = 0;
			y = 1;
			z = 0;
		}
		else if (index == 'z')
		{
			x = 0;
			y = 0;
			z = 1;
		}
	}
	else if (v_or_m == 'v')
	{
		x = 1, y = 0, z = 0; // 도는 방향을 정해주는 변수
		if (isminus == true)
		{
			x = -x;
		}
	}


	glRotatef(1, x, y, z);



	if (whoisselected == "bunny")
	{
		if (v_or_m == 'v')
		{
			//현재 곱한 카메라 축에 대한 역행렬을 곱해서 단위행렬로 바꿔버림
			glMultMatrixf(bunny2wld.inverse().matrix());

			//다시 원래 토끼 축의 매트릭스 배열로 돌려냄
			bunny2wld.matrix()[0] = temp.matrix()[0];
			bunny2wld.matrix()[1] = temp.matrix()[1];
			bunny2wld.matrix()[2] = temp.matrix()[2];
			bunny2wld.matrix()[4] = temp.matrix()[4];
			bunny2wld.matrix()[5] = temp.matrix()[5];
			bunny2wld.matrix()[6] = temp.matrix()[6];
			bunny2wld.matrix()[8] = temp.matrix()[8];
			bunny2wld.matrix()[9] = temp.matrix()[9];
			bunny2wld.matrix()[10] = temp.matrix()[10];

			//바꾼 토끼 축의 매트릭스를 곱함
			glMultMatrixf(bunny2wld.matrix());

		}
		glGetFloatv(GL_MODELVIEW_MATRIX, bunny2wld.matrix());
	}
	else if (whoisselected == "cow")
	{
		if (v_or_m == 'v')
		{
			//현재 곱한 카메라 축에 대한 역행렬을 곱해서 단위행렬로 바꿔버림
			glMultMatrixf(cow2wld.inverse().matrix());

			//다시 원래 소 축의 매트릭스 배열로 돌려냄
			cow2wld.matrix()[0] = temp.matrix()[0];
			cow2wld.matrix()[1] = temp.matrix()[1];
			cow2wld.matrix()[2] = temp.matrix()[2];
			cow2wld.matrix()[4] = temp.matrix()[4];
			cow2wld.matrix()[5] = temp.matrix()[5];
			cow2wld.matrix()[6] = temp.matrix()[6];
			cow2wld.matrix()[8] = temp.matrix()[8];
			cow2wld.matrix()[9] = temp.matrix()[9];
			cow2wld.matrix()[10] = temp.matrix()[10];

			//바꾼 소 축의 매트릭스를 곱함
			glMultMatrixf(cow2wld.matrix());

		}
		glGetFloatv(GL_MODELVIEW_MATRIX, cow2wld.matrix());
	}

	glPopMatrix();
}





void translate(){
	glPushMatrix();
	glLoadIdentity();

	/*******************************************************************/
	//(PA #2,#3)  수행한 내용 추가
	/*******************************************************************/
	int x = 0, y = 0;

	if (whoisselected == "cow")
	{
		if (v_or_m == 'v')
		{
			//rotate와 마찬가지로 매트릭스를 다룸
			temp = cow2wld;

			cow2wld.matrix()[0] = cam2wld[cameraIndex].matrix()[0];
			cow2wld.matrix()[1] = cam2wld[cameraIndex].matrix()[1];
			cow2wld.matrix()[2] = cam2wld[cameraIndex].matrix()[2];
			cow2wld.matrix()[4] = cam2wld[cameraIndex].matrix()[4];
			cow2wld.matrix()[5] = cam2wld[cameraIndex].matrix()[5];
			cow2wld.matrix()[6] = cam2wld[cameraIndex].matrix()[6];
			cow2wld.matrix()[8] = cam2wld[cameraIndex].matrix()[8];
			cow2wld.matrix()[9] = cam2wld[cameraIndex].matrix()[9];
			cow2wld.matrix()[10] = cam2wld[cameraIndex].matrix()[10];
		}
		glMultMatrixf(cow2wld.matrix()); //load current transformation matrix of object
	}

	else if (whoisselected == "bunny")
	{
		if (v_or_m == 'v')
		{
			//rotate와 마찬가지로 매트릭스를 다룸
			temp = bunny2wld;


			bunny2wld.matrix()[0] = cam2wld[cameraIndex].matrix()[0];
			bunny2wld.matrix()[1] = cam2wld[cameraIndex].matrix()[1];
			bunny2wld.matrix()[2] = cam2wld[cameraIndex].matrix()[2];
			bunny2wld.matrix()[4] = cam2wld[cameraIndex].matrix()[4];
			bunny2wld.matrix()[5] = cam2wld[cameraIndex].matrix()[5];
			bunny2wld.matrix()[6] = cam2wld[cameraIndex].matrix()[6];
			bunny2wld.matrix()[8] = cam2wld[cameraIndex].matrix()[8];
			bunny2wld.matrix()[9] = cam2wld[cameraIndex].matrix()[9];
			bunny2wld.matrix()[10] = cam2wld[cameraIndex].matrix()[10];
		}
		glMultMatrixf(bunny2wld.matrix()); //load current transformation matrix of object
	}



	if (v_or_m == 'm')
	{

		//드래그 거리에 따라 선택된 축에 대해 평행이동하게 구현하십시오.


		if (index == 'x')
		{
			x = 1;
			y = 0;
		}
		else if (index == 'y')
		{
			x = 0;
			y = 1;
		}
		if (isminus == true)
		{
			x = -x;
			y = -y;
		}

		glTranslatef(float(x) / 3, float(y) / 3, 0);






	}
	else if (v_or_m == 'v')
	{
		//x, y키가 입력되었다면 xy 평면에 대해서, z키가 입력되었다면 카메라 시점과 평행하게 앞 뒤로 이동하게 구현하십시오.
		if (index == 'x' || index == 'y')
		{
			glTranslatef(float(translate_x) / 10, float(translate_y) / 10, 0);
		}
		else if (index == 'z')
		{
			int z = 0;
			if (isminus == true)
			{
				z = 1;
			}
			else
			{
				z = -1;
			}
			glTranslatef(0, 0, float(z) / 3);
		}
	}
	if (whoisselected == "cow")
	{
		if (v_or_m == 'v')
		{
			glMultMatrixf(cow2wld.inverse().matrix());
			//rotate와 마찬가지로 매트릭스를 다룸
			cow2wld.matrix()[0] = temp.matrix()[0];
			cow2wld.matrix()[1] = temp.matrix()[1];
			cow2wld.matrix()[2] = temp.matrix()[2];
			cow2wld.matrix()[4] = temp.matrix()[4];
			cow2wld.matrix()[5] = temp.matrix()[5];
			cow2wld.matrix()[6] = temp.matrix()[6];
			cow2wld.matrix()[8] = temp.matrix()[8];
			cow2wld.matrix()[9] = temp.matrix()[9];
			cow2wld.matrix()[10] = temp.matrix()[10];

			glMultMatrixf(cow2wld.matrix());

		}
		glGetFloatv(GL_MODELVIEW_MATRIX, cow2wld.matrix());
	}
	else if (whoisselected == "bunny")
	{
		if (v_or_m == 'v')
		{
			glMultMatrixf(bunny2wld.inverse().matrix());
			//rotate와 마찬가지로 매트릭스를 다룸
			bunny2wld.matrix()[0] = temp.matrix()[0];
			bunny2wld.matrix()[1] = temp.matrix()[1];
			bunny2wld.matrix()[2] = temp.matrix()[2];
			bunny2wld.matrix()[4] = temp.matrix()[4];
			bunny2wld.matrix()[5] = temp.matrix()[5];
			bunny2wld.matrix()[6] = temp.matrix()[6];
			bunny2wld.matrix()[8] = temp.matrix()[8];
			bunny2wld.matrix()[9] = temp.matrix()[9];
			bunny2wld.matrix()[10] = temp.matrix()[10];

			glMultMatrixf(bunny2wld.matrix());

		}
		glGetFloatv(GL_MODELVIEW_MATRIX, bunny2wld.matrix());

	}

	glPopMatrix();
}





void idle() {

	/*******************************************************************/
	//(PA #2,#3)  수행한 내용 추가
	/*******************************************************************/
	if (time>0)
	{
		rotate();
		time = time - 1;
	}
	else
	{
		isrotating = false;
	}

	glutPostRedisplay();
}


// 'cameras' stores infomation of 5 cameras.
float cameras[5][9] =
{
	{ 28, 18, 28, 0, 2, 0, 0, 1, 0 },
	{ 28, 18, -28, 0, 2, 0, 0, 1, 0 },
	{ -28, 18, 28, 0, 2, 0, 0, 1, 0 },
	{ -12, 12, 0, 0, 2, 0, 0, 1, 0 },
	{ 0, 100, 0, 0, 0, 0, 1, 0, 0 }
};
int cameraCount = sizeof(cameras) / sizeof(cameras[0]);

int cameraIndex, camID;
vector<Matrix> wld2cam, cam2wld;

// Variables for 'cow' object.
Matrix cow2wld;
WaveFrontOBJ* cow;
int cowID;

// Variables for 'bunny' object.
Matrix bunny2wld;
WaveFrontOBJ* bunny;
int bunnyID;

unsigned floorTexID;

int frame = 0;
int width, height;
int oldX, oldY;

void translate();
void rotate();

int selectMode;
bool picking = false;

/*******************************************************************/
//(PA #2,#3) 수행한 내용 추가
/*******************************************************************/
char index = 'x';
bool isrotating = false;
bool isminus = false;
int time;

Matrix temp;

int translate_x;//x축 평행이동 시 마우스 포인터 변화 값
int translate_y;//y축 평행이동 시 마우스 포인터 변화 값

char v_or_m = 'm';//v모드 인지 m 모드인지 정하기

char* whoisselected = "cow";//누가 선택됬는지 나타내는 index

bool shademodel = true;

bool VN = false;

bool FN = false;

bool plight = false;
bool dlight = true;
bool slight = false;
//------------------------------------------------------------------------------
void munge(int x, float& r, float& g, float& b)
{
	r = (x & 255) / float(255);
	g = ((x >> 8) & 255) / float(255);
	b = ((x >> 16) & 255) / float(255);
}





//------------------------------------------------------------------------------
int unmunge(float r, float g, float b)
{
	return (int(r) + (int(g) << 8) + (int(b) << 16));
}





/*********************************************************************************
* Draw x, y, z axis of current frame on screen.
* x, y, and z are corresponded Red, Green, and Blue, resp.
**********************************************************************************/
void drawAxis(float len)
{
	glDisable(GL_LIGHTING);		// Lighting is not needed for drawing axis.
	glBegin(GL_LINES);			// Start drawing lines.
	glColor3d(1, 0, 0);			// color of x-axis is red.
	glVertex3d(0, 0, 0);
	glVertex3d(len, 0, 0);		// Draw line(x-axis) from (0,0,0) to (len, 0, 0). 
	glColor3d(0, 1, 0);			// color of y-axis is green.
	glVertex3d(0, 0, 0);
	glVertex3d(0, len, 0);		// Draw line(y-axis) from (0,0,0) to (0, len, 0).
	glColor3d(0, 0, 1);			// color of z-axis is  blue.
	glVertex3d(0, 0, 0);
	glVertex3d(0, 0, len);		// Draw line(z-axis) from (0,0,0) - (0, 0, len).
	glEnd();					// End drawing lines.
	glEnable(GL_LIGHTING);
}





void InitCamera() {

	// initialize camera frame transforms.
	for (int i = 0; i < cameraCount; i++)
	{
		float* c = cameras[i];													// 'c' points the coordinate of i-th camera.
		wld2cam.push_back(Matrix());											// Insert {0} matrix to wld2cam vector.
		glPushMatrix();															// Push the current matrix of GL into stack.

		glLoadIdentity();														// Set the GL matrix Identity matrix.
		gluLookAt(c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7], c[8]);		// Setting the coordinate of camera.
		glGetFloatv(GL_MODELVIEW_MATRIX, wld2cam[i].matrix());					// Read the world-to-camera matrix computed by gluLookAt.
		cam2wld.push_back(wld2cam[i].inverse());								// Get the camera-to-world matrix.

		glPopMatrix();															// Transfer the matrix that was pushed the stack to GL.
	}
	cameraIndex = 0;
}





void drawCamera()
{
	int i;
	// set viewing transformation.
	glLoadMatrixf(wld2cam[cameraIndex].matrix());

	// draw cameras.
	for (i = 0; i < (int)wld2cam.size(); i++)
	{
		if (i != cameraIndex)
		{
			glPushMatrix();													// Push the current matrix on GL to stack. The matrix is wld2cam[cameraIndex].matrix().
			glMultMatrixf(cam2wld[i].matrix());								// Multiply the matrix to draw i-th camera.
			if (selectMode == 0)											// selectMode == 1 means backbuffer mode.
			{
				drawAxis(5);												// Draw x, y, and z axis.
			}
			else
			{
				float r, g, b;
				glDisable(GL_LIGHTING);										// Disable lighting in backbuffer mode.
				munge(i + 1, r, g, b);										// Match the corresponding (i+1)th color to r, g, b. You can change the color of camera on backbuffer.
				glColor3f(r, g, b);											// Set r, g, b the color of camera.
			}

			glutSolidSphere(1.0, 10, 10);

			glPopMatrix();													// Call the matrix on stack. wld2cam[cameraIndex].matrix() in here.
		}
	}
}





/*********************************************************************************
* Draw 'cow' object.
**********************************************************************************/
void InitCow() {
	cow = new WaveFrontOBJ("cow_pa4.obj");

	glPushMatrix();											// Push the current matrix of GL into stack.
	glLoadIdentity();										// Set the GL matrix Identity matrix.
	glTranslatef(0, -cow->bbmin.pos.y, -8);					// Set the location of cow.
	glRotatef(-90, 0, 1, 0);								// Set the direction of cow. These information are stored in the matrix of GL.
	glGetFloatv(GL_MODELVIEW_MATRIX, cow2wld.matrix());		// Read the modelview matrix about location and direction set above, and store it in cow2wld matrix.
	glPopMatrix();											// Pop the matrix on stack to GL.

}





void drawCow()
{
	glPushMatrix();											// Push the current matrix of GL into stack. This is because the matrix of GL will be change while drawing cow.

															// The information about location of cow to be drawn is stored in cow2wld matrix.
															// If you change the value of the cow2wld matrix or the current matrix, cow would rotate or move.
	glMultMatrixf(cow2wld.matrix());

	if (selectMode == 0)									// selectMode == 1 means backbuffer mode.
	{
		drawAxis(5);										// Draw x, y, and z axis.
	}
	else
	{
		float r, g, b;
		glDisable(GL_LIGHTING);								// Disable lighting in backbuffer mode.
		munge(32, r, g, b);									// Match the corresponding constant color to r, g, b. You can change the color of camera on backbuffer
		glColor3f(r, g, b);
	}


	cow->Draw();
	/*******************************************************************/
	//(PA #4) : cow object의 normal을 그리는 함수를 추가하십시오.
	/*******************************************************************/
	if (FN == true)
		cow->Draw_FN();
	if (VN == true)
		cow->Draw_VN();

	glPopMatrix();											// Pop the matrix in stack to GL. Change it the matrix before drawing cow.
}





void Initbunny() {
	bunny = new WaveFrontOBJ("bunny_pa4.obj");

	glPushMatrix();											// Push the current matrix of GL into stack.
	glLoadIdentity();										// Set the GL matrix Identity matrix.
	glTranslatef(0, -bunny->bbmin.pos.y, 8);					// Set the location of bunny.
	glRotatef(180, 0, 1, 0);								// Set the direction of bunny. These information are stored in the matrix of GL.
	glGetFloatv(GL_MODELVIEW_MATRIX, bunny2wld.matrix());		// Read the modelview matrix about location and direction set above, and store it in bunny2wld matrix.
	glPopMatrix();											// Pop the matrix on stack to GL.

}





void drawbunny()
{
	glPushMatrix();		// Push the current matrix of GL into stack. This is because the matrix of GL will be change while drawing bunny.

						// The information about location of bunny to be drawn is stored in bunny2wld matrix.
						// If you change the value of the bunny2wld matrix or the current matrix, bunny would rotate or move.
	glMultMatrixf(bunny2wld.matrix());

	if (selectMode == 0)									// selectMode == 1 means backbuffer mode.
	{
		drawAxis(8);										// Draw x, y, and z axis.
	}
	else
	{
		float r, g, b;
		glDisable(GL_LIGHTING);								// Disable lighting in backbuffer mode.
		munge(33, r, g, b);									// Match the corresponding constant color to r, g, b. You can change the color of camera on backbuffer
		glColor3f(r, g, b);
	}

	bunny->Draw();											// Draw the bunny 
															/*******************************************************************/
															//(PA #4) : bunny object의 normal을 그리는 함수를 추가하십시오.
															/*******************************************************************/
	if (FN == true)
		bunny->Draw_FN();
	if (VN == true)
		bunny->Draw_VN();

	glPopMatrix();											// Pop the matrix in stack to GL. Change it the matrix before drawing bunny.
}





/*********************************************************************************
* Draw floor on 3D plane.
**********************************************************************************/
void drawFloor()
{
	if (frame == 0)
	{
		// Initialization part.
		// After making checker-patterned texture, use this repetitively.

		// Insert color into checker[] according to checker pattern.
		const int size = 8;
		unsigned char checker[size*size * 3];
		for (int i = 0; i < size*size; i++)
		{
			if (((i / size) ^ i) & 1)
			{
				checker[3 * i + 0] = 100;
				checker[3 * i + 1] = 100;
				checker[3 * i + 2] = 100;
			}
			else
			{
				checker[3 * i + 0] = 200;
				checker[3 * i + 1] = 200;
				checker[3 * i + 2] = 200;
			}
		}

		// Make texture which is accessible through floorTexID. 
		glGenTextures(1, &floorTexID);
		glBindTexture(GL_TEXTURE_2D, floorTexID);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, checker);
	}

	glDisable(GL_LIGHTING);

	// Set background color.
	if (selectMode == 0)
		glColor3d(0.35, 0.2, 0.1);
	else
	{
		// In backbuffer mode.
		float r, g, b;
		munge(34, r, g, b);
		glColor3f(r, g, b);
	}

	// Draw background rectangle.
	glBegin(GL_POLYGON);
	glVertex3f(-2000, -0.2, -2000);
	glVertex3f(-2000, -0.2, 2000);
	glVertex3f(2000, -0.2, 2000);
	glVertex3f(2000, -0.2, -2000);
	glEnd();


	// Set color of the floor.
	if (selectMode == 0)
	{
		// Assign checker-patterned texture.
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, floorTexID);
	}
	else
	{
		// Assign color on backbuffer mode.
		float r, g, b;
		munge(35, r, g, b);
		glColor3f(r, g, b);
	}

	// Draw the floor. Match the texture's coordinates and the floor's coordinates resp. 
	glBegin(GL_POLYGON);
	glTexCoord2d(0, 0);
	glVertex3d(-12, -0.1, -12);		// Texture's (0,0) is bound to (-12,-0.1,-12).
	glTexCoord2d(0, 1);
	glVertex3d(-12, -0.1, 12);		// Texture's (0,1) is bound to (-12,-0.1,12).
	glTexCoord2d(1, 1);
	glVertex3d(12, -0.1, 12);		// Texture's (1,1) is bound to (12,-0.1,12).
	glTexCoord2d(1, 0);
	glVertex3d(12, -0.1, -12);		// Texture's (1,0) is bound to (12,-0.1,-12).

	glEnd();

	if (selectMode == 0)
	{
		glDisable(GL_TEXTURE_2D);
		drawAxis(5);				// Draw x, y, and z axis.
	}
}





void Lighting()
{
	/*******************************************************************/
	//(PA #4) : 다양한 광원을 구현하십시오.
	//1. Point light / Directional light / Spotlight를 서로 다른 색으로 구현하십시오.
	//2. 광원의 위치를 구(sphere)로 표현하십시오.
	//3. Directional light / Spotlight의 경우 빛의 진행방향을 선분으로 표현하십시오.
	/*******************************************************************/
	// Define lighting for the scene

	//불  켜기
	if (dlight) {
		float light_pos[] = { 1.0, 1.0, 1.0, 0.0 };//Directional Light
		float light_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
		float light_diffuse[] = { 0.9, 0.9, 0.9, 1.0 };
		float light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
		glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
		glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
		glEnable(GL_LIGHT0);
	}
	if (plight) {
		float light_pos[] = { 1.0, 1.0, 1.0, 1.0f };//Point Light
		float light_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
		float light_diffuse[] = { 0.9, 0.9, 0.9, 1.0 };
		float light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
		glLightfv(GL_LIGHT1, GL_POSITION, light_pos);
		glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
		glEnable(GL_LIGHT1);
	}
	if (slight)
	{
		float light_pos[] = { 1.0, 1.0, 1.0, 1.0f };//Spot Light
		float light_dir[] = { -0.1f, -0.1f,-0.2f };
		float light_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
		float light_diffuse[] = { 0.9, 0.9, 0.9, 1.0 };
		float light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient);
		glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse);
		glLightfv(GL_LIGHT2, GL_POSITION, light_pos);
		glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, light_dir);
		glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular);
		glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 60.0f);
		glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 2.0f);
		glEnable(GL_LIGHT2);

	}

	//불 끄기
	if (!dlight)
	{
		glDisable(GL_LIGHT0);
	}
	if (!plight)
	{
		glDisable(GL_LIGHT1);
	}
	if (!slight)
	{
		glDisable(GL_LIGHT2);
	}
}





/*********************************************************************************
* Call this part whenever display events are needed.
* Display events are called in case of re-rendering by OS. ex) screen movement, screen maximization, etc.
* Or, user can occur the events by using glutPostRedisplay() function directly.
* this part is called in main() function by registering on glutDisplayFunc(display).
**********************************************************************************/
void display()
{
	if (selectMode == 0)
		glClearColor(0, 0.6, 0.8, 1);								// Clear color setting
	else
		glClearColor(0, 0, 0, 1);									// When the backbuffer mode, clear color is set to black

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);				// Clear the screen

	if (shademodel)
		glShadeModel(GL_FLAT);										// Set Flat Shading
	else
		glShadeModel(GL_SMOOTH);

	drawCamera();													// and draw all of them.
	drawFloor();													// Draw floor.

	drawCow();														// Draw cow.
	drawbunny();


	Lighting();

	glFlush();

	// If it is not backbuffer mode, swap the screen. In backbuffer mode, this is not necessary because it is not presented on screen.
	if (selectMode == 0)
		glutSwapBuffers();

	frame++;
}





/*********************************************************************************
* Call this part whenever size of the window is changed.
* This part is called in main() function by registering on glutReshapeFunc(reshape).
**********************************************************************************/
void reshape(int w, int h)
{
	width = w;
	height = h;
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);            // Select The Projection Matrix
	glLoadIdentity();                       // Reset The Projection Matrix
	float aspect = width / float(height);
	gluPerspective(45, aspect, 1, 1024);

	glMatrixMode(GL_MODELVIEW);             // Select The Modelview Matrix
	glLoadIdentity();                       // Reset The Projection Matrix
}





//------------------------------------------------------------------------------
void initialize()
{
	// Set up OpenGL state
	glEnable(GL_DEPTH_TEST);         // Enables Depth Testing
									 // Initialize the matrix stacks
	reshape(width, height);


	InitCamera();
	InitCow();
	Initbunny();
}





/*********************************************************************************
* Call this part whenever mouse button is clicked.
* This part is called in main() function by registering on glutMouseFunc(onMouseButton).
**********************************************************************************/
void onMouseButton(int button, int state, int x, int y)
{
	y = height - y - 1;
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			printf("Left mouse click at (%d, %d)\n", x, y);

			// After drawing object on backbuffer, you can recognize which object is selected by reading pixel of (x, y).
			// Change the value of selectMode to 1, then draw the object on backbuffer when display() function is called. 
			selectMode = 1;
			display();
			glReadBuffer(GL_BACK);
			unsigned char pixel[3];
			glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
			int pixel_value = unmunge(pixel[0], pixel[1], pixel[2]);
			printf("pixel = %d\n", pixel_value);

			if (picking) {
				/*******************************************************************/
				//(PA #2,#3) 수행한 내용 추가
				/*******************************************************************/
				if (pixel_value == 32)
				{
					whoisselected = "cow";//소가 선택되면 index를 소로 변환
				}
				else if (pixel_value == 33)
				{
					whoisselected = "bunny";//토끼가 선택되면 index를 토끼로 변환
				}

				picking = false;
			}

			selectMode = 0;
			// Save current clicked location of mouse here, and then use this on onMouseDrag function. 
			oldX = x;
			oldY = y;
		}
	}

	glutPostRedisplay();
}





/*********************************************************************************
* Call this part whenever user drags mouse.
* Input parameters x, y are coordinate of mouse on dragging.
* Value of global variables oldX, oldY is stored on onMouseButton,
* Then, those are used to verify value of x - oldX,  y - oldY to know its movement.
**********************************************************************************/
void onMouseDrag(int x, int y)
{
	y = height - y - 1;
	printf("in drag (%d, %d)\n", x - oldX, y - oldY);

	/*******************************************************************/
	//(PA #2,#3)  수행한 내용 추가
	/*******************************************************************/
	if (v_or_m == 'm')
	{
		if (index == 'x' && x - oldX > 0)//x키 상태에서 마우스의 x포인터가 증가할 때
		{
			isminus = false;
		}
		else if (index == 'y' && y - oldY > 0)//y키 상태에서 마우스의 y포인터가 증가할 때
		{
			isminus = false;
		}

		else if (index == 'x' && x - oldX < 0)//x키 상태에서 마우스의 x포인터가 감소할 때
		{
			isminus = true;
		}
		else if (index == 'y' && y - oldY < 0)//y키 상태에서 마우스의 y포인터가 감소할 때
		{
			isminus = true;
		}

		translate();
	}
	else if (v_or_m == 'v')
	{
		if (index == 'r')
		{
			if (x - oldX > 0)//z키 상태에서 마우스의 y포인터가 증가할 때
			{
				isminus = false;
			}
			else if (x - oldX < 0)
			{
				isminus = true;
			}
			//x축을 기준으로 드래그 거리에 따라 회전하게 하십시오.
			rotate();
		}
		else if (index == 'x' || index == 'y')
		{
			translate_x = x - oldX;
			translate_y = y - oldY;
			translate();
		}
		else if (index == 'z')
		{
			if (x - oldX > 0)//z키 상태에서 마우스의 y포인터가 증가할 때
			{
				isminus = false;
			}
			else if (x - oldX < 0)
			{
				isminus = true;
			}
			//카메라 축을 기준으로 z축 평행이동
			translate();
		}
	}

	oldX = x;
	oldY = y;
	glutPostRedisplay();
}





/*********************************************************************************
* Call this part whenever user types keyboard.
* This part is called in main() function by registering on glutKeyboardFunc(onKeyPress).
**********************************************************************************/
void onKeyPress(unsigned char key, int x, int y)
{
	//카메라 시점 변경
	if ((key >= '1') && (key <= '5'))
		cameraIndex = key - '1';

	/*******************************************************************/
	//(PA #2,#3)  수행한 내용 추가
	/*******************************************************************/
	switch (key)
	{
		//selectMode를 통해 과제2랑 과제3을 선택
	case 'm'://m키로 모드 변환
		v_or_m = 'm';
		printf("M pressed \n");
		break;
	case 'v'://v키로 모드 변환
		v_or_m = 'v';
		index = 'r';
		printf("V pressed \n");
		break;
	case 'x':
		index = 'x';//x를 누르면 x축으로 이동방향을 정함
		break;
	case 'y':
		index = 'y';//y를 누르면 y축으로 이동방향을 정함
		break;
	case 'z':
		index = 'z';//z를 누르면 z축으로 이동방향을 정함
		break;
	case 'p':
		picking = true;
		break;
	case 'r':
		if (v_or_m == 'm')
		{
			printf("R pressed \n");
			if (isrotating == false)
			{
				time = 359;
				rotate();
			}
			else
			{
				time = 0;
			}
		}
		else if (v_or_m == 'v')
		{
			index = 'r';
		}
		break;
	case 'f'://f키로 FaceNormal을 껐다 킴
		if (FN == true)
			FN = false;
		else
			FN = true;
		break;
	case 'n'://n키로 VertexNormal을 껐다 킴
		if (VN == true)
			VN = false;
		else
			VN = true;
		break;
	case 27:
		exit(0);
		break;

	}

	if (key == 's') {
		shademodel = !shademodel;
		cow->isFlat = shademodel;
		bunny->isFlat = shademodel;
	}

	glutPostRedisplay();
}






void  SpecialKey(int key, int x, int y)
{
	/*******************************************************************/
	//(PA #4) : F1 / F2 / F3 버튼에 따라 서로 다른 광원이 On/Off 되도록 구현하십시오.
	/*******************************************************************/

	switch (key)
	{
	case GLUT_KEY_F1:
		dlight = !dlight;
		break;
	case GLUT_KEY_F2:
		plight = !plight;
		break;
	case GLUT_KEY_F3:
		slight = !slight;
		break;
	}

	glutPostRedisplay();
}





void rotate() {
	glPushMatrix();
	glLoadIdentity();

	/*******************************************************************/
	//(PA #2,#3)  수행한 내용 추가
	/*******************************************************************/
	//'r'키로 회전상태가 ON 되었을 경우 선택된 축에 대해 자동으로 회전되게 하십시오.
	if (whoisselected == "bunny")
	{
		if (v_or_m == 'v')
		{
			temp = bunny2wld;

			//매트릭스 배열의 0,1,2,4,5,6,8,9,10번 값을 변환해서 축을 카메라 축으로 변환함
			bunny2wld.matrix()[0] = cam2wld[cameraIndex].matrix()[0];
			bunny2wld.matrix()[1] = cam2wld[cameraIndex].matrix()[1];
			bunny2wld.matrix()[2] = cam2wld[cameraIndex].matrix()[2];
			bunny2wld.matrix()[4] = cam2wld[cameraIndex].matrix()[4];
			bunny2wld.matrix()[5] = cam2wld[cameraIndex].matrix()[5];
			bunny2wld.matrix()[6] = cam2wld[cameraIndex].matrix()[6];
			bunny2wld.matrix()[8] = cam2wld[cameraIndex].matrix()[8];
			bunny2wld.matrix()[9] = cam2wld[cameraIndex].matrix()[9];
			bunny2wld.matrix()[10] = cam2wld[cameraIndex].matrix()[10];
		}
		glMultMatrixf(bunny2wld.matrix()); //load current transformation matrix of object
	}
	else if (whoisselected == "cow")
	{
		if (v_or_m == 'v')
		{
			temp = cow2wld;
			//매트릭스 배열의 0,1,2,4,5,6,8,9,10번 값을 변환해서 축을 카메라 축으로 변환함
			cow2wld.matrix()[0] = cam2wld[cameraIndex].matrix()[0];
			cow2wld.matrix()[1] = cam2wld[cameraIndex].matrix()[1];
			cow2wld.matrix()[2] = cam2wld[cameraIndex].matrix()[2];
			cow2wld.matrix()[4] = cam2wld[cameraIndex].matrix()[4];
			cow2wld.matrix()[5] = cam2wld[cameraIndex].matrix()[5];
			cow2wld.matrix()[6] = cam2wld[cameraIndex].matrix()[6];
			cow2wld.matrix()[8] = cam2wld[cameraIndex].matrix()[8];
			cow2wld.matrix()[9] = cam2wld[cameraIndex].matrix()[9];
			cow2wld.matrix()[10] = cam2wld[cameraIndex].matrix()[10];


		}
		glMultMatrixf(cow2wld.matrix()); //load current transformation matrix of object

	}

	int x = 1, y = 0, z = 0; // 도는 방향을 정해주는 변수

	if (v_or_m == 'm')
		isrotating = true;//도는 중이면 다시 못 돌리기 위해 있는 변수

						  //index 에 따라 회전 방향을 바꿈
	if (v_or_m == 'm')
	{
		if (index == 'x')
		{
			x = 1;
			y = 0;
			z = 0;
		}
		else if (index == 'y')
		{
			x = 0;
			y = 1;
			z = 0;
		}
		else if (index == 'z')
		{
			x = 0;
			y = 0;
			z = 1;
		}
	}
	else if (v_or_m == 'v')
	{
		x = 1, y = 0, z = 0; // 도는 방향을 정해주는 변수
		if (isminus == true)
		{
			x = -x;
		}
	}


	glRotatef(1, x, y, z);



	if (whoisselected == "bunny")
	{
		if (v_or_m == 'v')
		{
			//현재 곱한 카메라 축에 대한 역행렬을 곱해서 단위행렬로 바꿔버림
			glMultMatrixf(bunny2wld.inverse().matrix());

			//다시 원래 토끼 축의 매트릭스 배열로 돌려냄
			bunny2wld.matrix()[0] = temp.matrix()[0];
			bunny2wld.matrix()[1] = temp.matrix()[1];
			bunny2wld.matrix()[2] = temp.matrix()[2];
			bunny2wld.matrix()[4] = temp.matrix()[4];
			bunny2wld.matrix()[5] = temp.matrix()[5];
			bunny2wld.matrix()[6] = temp.matrix()[6];
			bunny2wld.matrix()[8] = temp.matrix()[8];
			bunny2wld.matrix()[9] = temp.matrix()[9];
			bunny2wld.matrix()[10] = temp.matrix()[10];

			//바꾼 토끼 축의 매트릭스를 곱함
			glMultMatrixf(bunny2wld.matrix());

		}
		glGetFloatv(GL_MODELVIEW_MATRIX, bunny2wld.matrix());
	}
	else if (whoisselected == "cow")
	{
		if (v_or_m == 'v')
		{
			//현재 곱한 카메라 축에 대한 역행렬을 곱해서 단위행렬로 바꿔버림
			glMultMatrixf(cow2wld.inverse().matrix());

			//다시 원래 소 축의 매트릭스 배열로 돌려냄
			cow2wld.matrix()[0] = temp.matrix()[0];
			cow2wld.matrix()[1] = temp.matrix()[1];
			cow2wld.matrix()[2] = temp.matrix()[2];
			cow2wld.matrix()[4] = temp.matrix()[4];
			cow2wld.matrix()[5] = temp.matrix()[5];
			cow2wld.matrix()[6] = temp.matrix()[6];
			cow2wld.matrix()[8] = temp.matrix()[8];
			cow2wld.matrix()[9] = temp.matrix()[9];
			cow2wld.matrix()[10] = temp.matrix()[10];

			//바꾼 소 축의 매트릭스를 곱함
			glMultMatrixf(cow2wld.matrix());

		}
		glGetFloatv(GL_MODELVIEW_MATRIX, cow2wld.matrix());
	}

	glPopMatrix();
}





void translate() {
	glPushMatrix();
	glLoadIdentity();

	/*******************************************************************/
	//(PA #2,#3)  수행한 내용 추가
	/*******************************************************************/
	int x = 0, y = 0;

	if (whoisselected == "cow")
	{
		if (v_or_m == 'v')
		{
			//rotate와 마찬가지로 매트릭스를 다룸
			temp = cow2wld;

			cow2wld.matrix()[0] = cam2wld[cameraIndex].matrix()[0];
			cow2wld.matrix()[1] = cam2wld[cameraIndex].matrix()[1];
			cow2wld.matrix()[2] = cam2wld[cameraIndex].matrix()[2];
			cow2wld.matrix()[4] = cam2wld[cameraIndex].matrix()[4];
			cow2wld.matrix()[5] = cam2wld[cameraIndex].matrix()[5];
			cow2wld.matrix()[6] = cam2wld[cameraIndex].matrix()[6];
			cow2wld.matrix()[8] = cam2wld[cameraIndex].matrix()[8];
			cow2wld.matrix()[9] = cam2wld[cameraIndex].matrix()[9];
			cow2wld.matrix()[10] = cam2wld[cameraIndex].matrix()[10];
		}
		glMultMatrixf(cow2wld.matrix()); //load current transformation matrix of object
	}

	else if (whoisselected == "bunny")
	{
		if (v_or_m == 'v')
		{
			//rotate와 마찬가지로 매트릭스를 다룸
			temp = bunny2wld;


			bunny2wld.matrix()[0] = cam2wld[cameraIndex].matrix()[0];
			bunny2wld.matrix()[1] = cam2wld[cameraIndex].matrix()[1];
			bunny2wld.matrix()[2] = cam2wld[cameraIndex].matrix()[2];
			bunny2wld.matrix()[4] = cam2wld[cameraIndex].matrix()[4];
			bunny2wld.matrix()[5] = cam2wld[cameraIndex].matrix()[5];
			bunny2wld.matrix()[6] = cam2wld[cameraIndex].matrix()[6];
			bunny2wld.matrix()[8] = cam2wld[cameraIndex].matrix()[8];
			bunny2wld.matrix()[9] = cam2wld[cameraIndex].matrix()[9];
			bunny2wld.matrix()[10] = cam2wld[cameraIndex].matrix()[10];
		}
		glMultMatrixf(bunny2wld.matrix()); //load current transformation matrix of object
	}



	if (v_or_m == 'm')
	{

		//드래그 거리에 따라 선택된 축에 대해 평행이동하게 구현하십시오.


		if (index == 'x')
		{
			x = 1;
			y = 0;
		}
		else if (index == 'y')
		{
			x = 0;
			y = 1;
		}
		if (isminus == true)
		{
			x = -x;
			y = -y;
		}

		glTranslatef(float(x) / 3, float(y) / 3, 0);






	}
	else if (v_or_m == 'v')
	{
		//x, y키가 입력되었다면 xy 평면에 대해서, z키가 입력되었다면 카메라 시점과 평행하게 앞 뒤로 이동하게 구현하십시오.
		if (index == 'x' || index == 'y')
		{
			glTranslatef(float(translate_x) / 10, float(translate_y) / 10, 0);
		}
		else if (index == 'z')
		{
			int z = 0;
			if (isminus == true)
			{
				z = 1;
			}
			else
			{
				z = -1;
			}
			glTranslatef(0, 0, float(z) / 3);
		}
	}
	if (whoisselected == "cow")
	{
		if (v_or_m == 'v')
		{
			glMultMatrixf(cow2wld.inverse().matrix());
			//rotate와 마찬가지로 매트릭스를 다룸
			cow2wld.matrix()[0] = temp.matrix()[0];
			cow2wld.matrix()[1] = temp.matrix()[1];
			cow2wld.matrix()[2] = temp.matrix()[2];
			cow2wld.matrix()[4] = temp.matrix()[4];
			cow2wld.matrix()[5] = temp.matrix()[5];
			cow2wld.matrix()[6] = temp.matrix()[6];
			cow2wld.matrix()[8] = temp.matrix()[8];
			cow2wld.matrix()[9] = temp.matrix()[9];
			cow2wld.matrix()[10] = temp.matrix()[10];

			glMultMatrixf(cow2wld.matrix());

		}
		glGetFloatv(GL_MODELVIEW_MATRIX, cow2wld.matrix());
	}
	else if (whoisselected == "bunny")
	{
		if (v_or_m == 'v')
		{
			glMultMatrixf(bunny2wld.inverse().matrix());
			//rotate와 마찬가지로 매트릭스를 다룸
			bunny2wld.matrix()[0] = temp.matrix()[0];
			bunny2wld.matrix()[1] = temp.matrix()[1];
			bunny2wld.matrix()[2] = temp.matrix()[2];
			bunny2wld.matrix()[4] = temp.matrix()[4];
			bunny2wld.matrix()[5] = temp.matrix()[5];
			bunny2wld.matrix()[6] = temp.matrix()[6];
			bunny2wld.matrix()[8] = temp.matrix()[8];
			bunny2wld.matrix()[9] = temp.matrix()[9];
			bunny2wld.matrix()[10] = temp.matrix()[10];

			glMultMatrixf(bunny2wld.matrix());

		}
		glGetFloatv(GL_MODELVIEW_MATRIX, bunny2wld.matrix());

	}

	glPopMatrix();
}





void idle() {

	/*******************************************************************/
	//(PA #2,#3)  수행한 내용 추가
	/*******************************************************************/
	if (time>0)
	{
		rotate();
		time = time - 1;
	}
	else
	{
		isrotating = false;
	}

	glutPostRedisplay();
}





//------------------------------------------------------------------------------
void main(int argc, char* argv[])
{
	
	width = 800;
	height = 600;
	frame = 0;
	glutInit(&argc, argv);							// Initialize openGL.
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);	// Initialize display mode. This project will use float buffer and RGB color.
	glutInitWindowSize(width, height);				// Initialize window size.
	glutInitWindowPosition(100, 100);				// Initialize window coordinate.
	glutCreateWindow("PA4");
	
	glutDisplayFunc(display);						// Register display function to call that when drawing screen event is needed.
	glutReshapeFunc(reshape);						// Register reshape function to call that when size of the window is changed.
	glutKeyboardFunc(onKeyPress);					// Register onKeyPress function to call that when user presses the keyboard.
	glutMouseFunc(onMouseButton);					// Register onMouseButton function to call that when user moves mouse.
	glutMotionFunc(onMouseDrag);					// Register onMouseDrag function to call that when user drags mouse.
	
	glutIdleFunc(idle);

	glutSpecialFunc(SpecialKey);

	initialize();

	//glew 하드웨어 지원 확인
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		printf("Load Fail : GLEW Library\n");
		cerr << glewGetErrorString(err) << endl;
	}
	else
		printf("Ready For GLEW Library\n");
	//WindowSize
	int WINDOW_HEIGHT=800;
	int WINDOW_WIDTH=600;
	//Camera
	float g_zoom;
	float g_Rotate[2];
	float g_Translate[2];
	//Mouse
	int g_Mouse_Coord[2];
	unsigned char g_Mouse_Event[3];

	// Initialize the other thing.
	glutMainLoop();									// Execute the loop which handles events.
}
void main(int argc, char* argv[]) {
	//lectureShader.InitShader("triangle.vert", "triangle.frag");

	glutInit(&argc, argv);							// Initialize openGL.
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);	// Initialize display mode. This project will use float buffer and RGB color.
	glutInitWindowSize(width, height);				// Initialize window size.
	glutInitWindowPosition(100, 100);				// Initialize window coordinate.
	glutCreateWindow("PA4");

	glutDisplayFunc(display);						// Register display function to call that when drawing screen event is needed.
	glutReshapeFunc(reshape);						// Register reshape function to call that when size of the window is changed.
	glutKeyboardFunc(onKeyPress);					// Register onKeyPress function to call that when user presses the keyboard.
	glutMouseFunc(onMouseButton);					// Register onMouseButton function to call that when user moves mouse.
	glutMotionFunc(onMouseDrag);					// Register onMouseDrag function to call that when user drags mouse.

	glutIdleFunc(idle);

	glutSpecialFunc(SpecialKey);

	initialize();

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();

	if (GLEW_OK != err)
	{
		printf(" Load Fail : GLEW Library \n");
		cerr << glewGetErrorString(err) << endl;
	}
	else
		printf("Ready for GLEW Library \n");
	/*
	//Buffer 생성
	glGenVertexArraysAPPLE(1, &VAO);
	glGenBuffers(1, &VBO);

	//Buffer Binding
	glBindVertexArrayAPPLE(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//Data 삽입
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Buffer 해제
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArrayAPPLE(0);
	*/
	/*
	glUseProgram(lectureShader.GetProgram());
	glBindVertexArrayAPPLE(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArrayAPPLE(0);
	*/
}