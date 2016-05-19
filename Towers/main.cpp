#include <iostream>
#include "display.h"
#include <GL/glew.h>
#include "shader.h"
#include "mesh.h"
#include "texture.h"
#include "transform.h"
#include "camera.h"
#include <vector>
#include "stb_image.h"
#include <iostream>
#include <stack>
#include <glm/glm.hpp>
#include < glm/gtc/matrix_transform.hpp >
#include < glm/gtc/type_ptr.hpp >


#define WIDTH 1600
#define HEIGHT 1200

/*
Method for loading cubemaps
*/
GLuint loadCubemap(std::vector<const GLchar*> faces)
{
	
	GLuint textureID;
	glGenTextures(1, &textureID);

	int width, height, numComponents;
	unsigned char* image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	for (GLuint i = 0; i < faces.size(); i++)
	{
		image = stbi_load(faces[i], &width, &height, &numComponents, 3);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		stbi_image_free(image);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
}

int main(int argc, char** argv)
{
	// Print insructions
	std::cout << "Use the 1, 2 and 3 keys to select a stand and then again to select the stand to move the ring to. \nThe C key can be used to cancel your selection.\n"
		<< "Press R to reset the game and use Z to increase the ring count and X to reduce it.\n"
		<< "Use LEFT and RIGHT arrow keys to orbit the scene.\n" <<
		"Use the UP and DOWN arrows to move the camera view vertically.\n" <<
		"Use the ESC key to exit the program. \n";

	Display display(1600, 1200, "The Tower of Hanoi");

	Vertex vertices[] = { Vertex(glm::vec3(-0.5, -0.5, 0), glm::vec2(0,0)),
						  Vertex(glm::vec3(0, 0.5, 0), glm::vec2(0.5,1.0)),
						  Vertex(glm::vec3(0.5, -0.5, 0), glm::vec2(1.0,0)), };

	unsigned int indices[] = { 0, 1, 2 };

	// Load the meshes
	Mesh mesh(vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices)/sizeof(indices[0]));
	Mesh disc("./res/disc.obj");
	Mesh stand("./res/stand.obj");
	Mesh arrow("./res/arrow.obj");
	Mesh one("./res/one.obj");
	Mesh two("./res/two.obj");
	Mesh three("./res/three.obj");
	Mesh winner("./res/winner.obj");
	
	// Load the shaders
	Shader skyShader("./res/sky");
	Shader discShader("./res/basic");
	Shader standShader("./res/basic2");
	Shader baseShader("./res/cube");
	Shader arrowShader("./res/basic");
	Shader numberShader("./res/basic");
	Shader emptyShader("./res/basic");

	// Create the transforms
	Transform transformStand;
	Transform transformDisc;
	Transform transformArrow;
	Transform transformNumber;
	Transform transformWinner;
	Texture standTexture("./res/wood.jpg");

	// Create camera
	Camera camera(glm::vec3(0, 0, -8), 70.0f, (float)WIDTH/(float)HEIGHT, 0.01f, 1000.0f);

	// Disc position vectors
	glm::vec3 discXLPos(-3.0f, 0.2f, 0.0f);
	glm::vec3 discBigPos(-3.0f, 0.2f, 0.0f);
	glm::vec3 discMedPos(-3.0f, 0.5f, 0.0f);
	glm::vec3 discSmallPos(-3.0f, 0.8f, 0.0f);
	glm::vec3 discXSPos(-3.0f, 1.0f, 0.0f);

	// Stacks for each stand
	std::stack<int> st1;
	std::stack<int> st2;
	std::stack<int> st3;

	int discCount = 3;
	int turnCount = 0;

	// Start game with 3 rings on stand 1
	st1.push(4);
	st1.push(3);
	st1.push(2);

	int selection = 0;
	bool finished = true;

	// Set up skybox vertices
	GLfloat skyboxVertices[] = {
		// Positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};
	// Setup skybox VAO
	GLuint skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);


	// Cubemap (Skybox)
	std::vector<const GLchar*> faces;
	faces.push_back("skybox/right.png");
	faces.push_back("skybox/left.png");
	faces.push_back("skybox/top.png");
	faces.push_back("skybox/bottom.png");
	faces.push_back("skybox/back.png");
	faces.push_back("skybox/front.png");
	GLuint skyboxTexture = loadCubemap(faces);


	float counter = 0.0f;
	float counter2 = 0.0f;


	while (!display.IsClosed())
	{
		// Sin and cosine calculations for the circular camera motion
		float sinCounter = sinf(counter);
		float cosCounter = cosf(counter);
		float sinCounter2 = sinf(counter2);
		float cosCounter2 = cosf(counter2);
		if (display.spinRight) {counter2 += 0.001f;} // If right arrow is pressed, rotae camera right
		if (display.spinLeft)  {counter2 -= 0.001f;} // If left arrow is pressed, roate camera left
		camera.setPosition(glm::vec3(sinCounter2 * 8, display.yValue, cosCounter2 * 8)); // Set camera's position
		camera.setForward(glm::vec3(-sinCounter2, 0, -cosCounter2)); // Set camera's forward direction
		transformNumber.setRot(glm::vec3(0.0f, counter2, 0.0f)); // Change number's roations if the camera has moved so they always face the camera

		// Clear buffers
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 model;
		glm::mat4 view = camera.GetViewProjection();
		glm::mat4 projection = glm::perspective(70.0f, (float)1600 / (float)1200, 0.1f, 1000.0f);

		// STANDS ////////////////////////////////

		// Draw stands
		transformStand.getPos().x = 0.0f; // Set stand position
		standShader.Bind(); // Bind the stand shader
		standTexture.Bind(0); // Bind the stand texture
		standShader.Update(transformStand, camera); // Update the stand shader for the stand and camera position
		stand.Draw();
		transformStand.getPos().x = 3.0f;
		standShader.Update(transformStand, camera);
		stand.Draw();
		transformStand.getPos().x = -3.0f;
		standShader.Update(transformStand, camera);
		stand.Draw();
		
		// Draw numbers
		numberShader.Bind(); // Bind the number shader
		GLint winnerLoc = glGetUniformLocation(numberShader.getProg(), "tint"); // Get the tint uniform from the shader
		glUniform4f(winnerLoc, 0.0f, 0.0f, 0.0f, 1.0f); // Set the tint colour
		transformNumber.setPos(glm::vec3(-3.0f, -1.0f, 0.0f)); // Position the number
		numberShader.Update(transformNumber, camera); // Update the shader for number and camera position
		one.Draw(); // Draw the number 1
		transformNumber.setPos(glm::vec3(0.0f, -1.0f, 0.0f));
		numberShader.Update(transformNumber, camera);
		two.Draw(); // Draw the number 2
		transformNumber.setPos(glm::vec3(3.0f, -1.0f, 0.0f));
		numberShader.Update(transformNumber, camera);
		three.Draw(); // Draw the number 3

		// WINNER //////////////////////////////////

		// Check if the game is complete by checking if the height of the 3rd stand is equal to the current number of discs in play
		if (st3.size() == discCount) {
			numberShader.Bind(); // Bind the shader
			glUniform4f(winnerLoc, 1.0f, 0.0f, 0.0f, 1.0f); // Set the tint
			transformWinner.setPos(glm::vec3(0.0f, 0.0f, 2.0f)); // Position the model
			numberShader.Update(transformWinner, camera); // Update the shader
			winner.Draw(); // Draw the winner text
			if (finished) {
				std::cout << "Good job! You finished " << discCount << " discs in " << turnCount << " moves! Press R to reset.\n";
				finished = false;
			}
		}

		// POSITION DISCS /////////////////////////////

		std::stack<int> temp = st1; // Create temp stack to hold each stand's stack 

		// Move through stand 1's contents and find out which discs are currently in it and set their position based on their height
		while (!temp.empty()) {
			float height = 0.2f + (temp.size() - 1.0f) * 0.3f;
			switch (temp.top()) {
				case 1:	discXSPos = glm::vec3(-3.0f, height, 0.0f); break;
				case 2: discSmallPos = glm::vec3(-3.0f,height, 0.0f); break;
				case 3: discMedPos = glm::vec3(-3.0f,height, 0.0f); break;
				case 4: discBigPos = glm::vec3(-3.0f, height, 0.0f); break;
				case 5: discXLPos = glm::vec3(-3.0f, height, 0.0f); break;
			}
			temp.pop();
		}

		temp = st2; // Set the temp stack to stand 2
		// Move through stack 2's contents and find out which discs are currently in it and set their position based on their height
		while (!temp.empty()) {
			float height = 0.2f + (temp.size() - 1.0f) * 0.3f;
			switch (temp.top()) {
			case 1:	discXSPos = glm::vec3(0.0f, height, 0.0f); break;
			case 2: discSmallPos = glm::vec3(0.0f, height, 0.0f); break;
			case 3: discMedPos = glm::vec3(0.0f, height, 0.0f); break;
			case 4: discBigPos = glm::vec3(0.0f, height, 0.0f); break;
			case 5: discXLPos = glm::vec3(0.0f, height, 0.0f); break;
			}
			temp.pop();
		}

		temp = st3; // Set the temp stack to stand 3
		// Move through stand 3's contents and find out which discs are currently in it and set their position based on their height
		while (!temp.empty()) {
			float height = 0.2f + (temp.size() - 1.0f) * 0.3f;
			switch (temp.top()) {
			case 1:	discXSPos = glm::vec3(3.0f, height, 0.0f); break;
			case 2: discSmallPos = glm::vec3(3.0f, height, 0.0f); break;
			case 3: discMedPos = glm::vec3(3.0f, height, 0.0f); break;
			case 4: discBigPos = glm::vec3(3.0f, height, 0.0f); break;
			case 5: discXLPos = glm::vec3(3.0f, height, 0.0f); break;
			}
			temp.pop();
		}


		// DRAW DISCS /////////////////////////////////
		discShader.Bind(); // Bind the disc shader for use
		GLint discLoc = glGetUniformLocation(discShader.getProg(), "tint");

		// BIG DISC
		glUniform4f(discLoc, 0.2f, 0.1f, 0.0f, 1.0f); // Apply disc tint
		transformDisc.setScale(glm::vec3(0.8f, 0.8f, 0.8f)); // Scale disc
		transformDisc.setPos(discBigPos); // Update disc position
		discShader.Update(transformDisc, camera); // Update disc shader for position and camera position
		disc.Draw(); // Draw the disc

		// MEDIUM DISC
		glUniform4f(discLoc, 0.2f, 0.2f, 0.0f, 1.0f);
		transformDisc.setScale(glm::vec3(0.6f, 0.8f, 0.6f));
		transformDisc.setPos(discMedPos);
		discShader.Update(transformDisc, camera);
		disc.Draw();

		// SMALL DISC
		glUniform4f(discLoc, 0.0f, 0.2f, 0.0f, 1.0f);
		transformDisc.setScale(glm::vec3(0.45f, 0.8f, 0.45f));
		transformDisc.setPos(discSmallPos);
		discShader.Update(transformDisc, camera);
		disc.Draw();

		// Extra small disc drawing for the 4th opotional disc
		if (discCount >= 4) {
			glUniform4f(discLoc, 0.0f, 0.0f, 0.2f, 1.0f);
			transformDisc.setScale(glm::vec3(0.3f, 0.8f, 0.3f));
			transformDisc.setPos(discXSPos);
			discShader.Update(transformDisc, camera);
			disc.Draw();
		}

		// Extra large disc drawing for the 5th opotional disc
		if (discCount == 5) {
			glUniform4f(discLoc, 0.2f, 0.0f, 0.0f, 1.0f);
			transformDisc.setScale(glm::vec3(0.9f, 0.8f, 0.9f));
			transformDisc.setPos(discXLPos);
			discShader.Update(transformDisc, camera);
			disc.Draw();
		}

		// DISC CONTROL ///////////////////////// 

		// Selecting the 1st disc stand
		if (display.oneP && selection == 0 && !st1.empty()) {
			if (st2.empty() || st3.empty() || (st2.top() > st1.top() || st3.top() > st1.top())) {
				selection = 1;
			}
		}
		// Selecting the 2nd disc stand
		if (display.twoP && selection == 0 && !st2.empty()) {
			if (st1.empty() || st3.empty() || (st1.top() > st2.top() || st3.top() > st2.top())) {
				selection = 2;
			}
		}
		// Selecting the 3rd disc stand
		if (display.threeP && selection == 0 && !st3.empty()) {
			if (st1.empty() || st2.empty() || (st1.top() > st3.top() || st2.top() > st3.top())) {
				selection = 3;
			}
		}

		// Moving discs from stand 2 to 1
		if (display.oneP && selection == 2) {
			if (st1.empty() || st2.top() < st1.top()) {
				st1.push(st2.top());
				st2.pop();
				selection = 4;
				turnCount++;
			}
		}
		// Moving discs from stand 3 to 1
		if (display.oneP && selection == 3) {
			if (st1.empty() || st3.top() < st1.top()) {
				st1.push(st3.top());
				st3.pop();
				selection = 4;
				turnCount++;
			}
		}
		// Moving discs from stand 1 to 2
		if (display.twoP && selection == 1) {
			if (st2.empty() || st1.top() < st2.top()) {
				st2.push(st1.top());
				st1.pop();
				selection = 4;
				turnCount++;
			}
		}
		// Moving discs from stand 3 to 2
		if (display.twoP && selection == 3) {
			if (st2.empty() || st3.top() < st2.top()) {
				st2.push(st3.top());
				st3.pop();
				selection = 4;
				turnCount++;
			}
		}
		// Moving discs from stand 2 to 3
		if (display.threeP && selection == 2) {
			if (st3.empty() || st2.top() < st3.top()) {
				st3.push(st2.top());
				st2.pop();
				selection = 4;
				turnCount++;
			}
		}
		// Moving discs from stand 1 to 3
		if (display.threeP && selection == 1) {
			if (st3.empty() || st1.top() < st3.top()) {
				st3.push(st1.top());
				st1.pop();
				selection = 4;
				turnCount++;
			}
		}
		// Cancel the stand selection
		if ((selection == 4 && display.newInput) || display.cancel) {
			selection = 0;
			display.cancel = false;
		}

		// ARROW /////////////////////////////////

		// Positing the arrow based on which stand is selected and which ring is at the top of that stand
		if (selection > 0 && selection < 4) {
			arrowShader.Bind();
			transformArrow.setScale(glm::vec3(0.3f, 0.3f, 0.3f));
			transformArrow.setRot(glm::vec3(0.0f, 30.0f, 0.0f));
			switch (selection) {
			case 1: switch (st1.size()) {
						case 1: transformArrow.setPos(glm::vec3(-4.5f, 0.2f, 0.0f)); break;
						case 2: transformArrow.setPos(glm::vec3(-4.5f, 0.5f, 0.0f)); break;
						case 3: transformArrow.setPos(glm::vec3(-4.5f, 0.9f, 0.0f)); break;
						case 4: transformArrow.setPos(glm::vec3(-4.5f, 1.1f, 0.0f)); break;
						case 5: transformArrow.setPos(glm::vec3(-4.5f, 1.5f, 0.0f)); break;
			} break;
			case 2:	switch (st2.size()) {
						case 1: transformArrow.setPos(glm::vec3(-1.5f, 0.2f, 0.0f)); break;
						case 2: transformArrow.setPos(glm::vec3(-1.5f, 0.5f, 0.0f)); break;
						case 3: transformArrow.setPos(glm::vec3(-1.5f, 0.8f, 0.0f)); break;
						case 4: transformArrow.setPos(glm::vec3(-1.5f, 1.1f, 0.0f)); break;
						case 5: transformArrow.setPos(glm::vec3(-1.5f, 1.5f, 0.0f)); break;
			} break;
			case 3:	switch (st3.size()) {
						case 1: transformArrow.setPos(glm::vec3(1.6f, 0.2f, 0.0f)); break;
						case 2: transformArrow.setPos(glm::vec3(1.6f, 0.5f, 0.0f)); break;
						case 3: transformArrow.setPos(glm::vec3(1.6f, 0.9f, 0.0f)); break;
						case 4: transformArrow.setPos(glm::vec3(1.6f, 1.1f, 0.0f)); break;
						case 5: transformArrow.setPos(glm::vec3(1.6f, 1.5f, 0.0f)); break;
			} break;
			}
			glDisable(GL_CULL_FACE);
			arrowShader.Update(transformArrow, camera);
			arrow.Draw();
		}


		// SKY BOX /////////////////////////////////
		
		glDisable(GL_CULL_FACE); // Disable face culling
		glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
		skyShader.Bind(); // Bind the skybox shader
		view = glm::mat4(glm::mat3(camera.GetViewProjection()));	// Remove any translation component of the view matrix
		glUniformMatrix4fv(glGetUniformLocation(skyShader.getProg(), "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skyShader.getProg(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		// SKYBOX CUBE ///////////////////////////////// 
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0); // Set active texture
		glUniform1i(glGetUniformLocation(skyShader.getProg(), "skybox"), 0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture); // Bind the cube map texture
		glDrawArrays(GL_TRIANGLES, 0, 36); // Draw the skybox
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // Set depth function back to default

		// SETTINGS /////////////////////////////

		// RESET
		// If reset is pressed then reset each stand
		if (display.reset) {
			while (!st1.empty()) {
				st1.pop();
			}
			while (!st2.empty()) {
				st2.pop();
			}
			while (!st3.empty()) {
				st3.pop();
			}
			switch (discCount) {
			case 3: st1.push(4); st1.push(3); st1.push(2); break; 
			case 4: st1.push(4); st1.push(3); st1.push(2); st1.push(1); break;
			case 5: st1.push(5); st1.push(4); st1.push(3); st1.push(2); st1.push(1); break;
			}
			selection = 0;
			turnCount = 0;
			display.reset = false;
			finished = true;
	
		}

		// DISC COUNT UP
		// If disc up key is pressed, increase the disc count up to a max of 5
		if (display.upDisc && (display.discChanged == false)) {
			if (discCount < 5) {
				discCount++;
				display.discChanged = true;
				display.reset = true;
			}
		}
		// DISC COUNT DOWN
		// If disc down key is pressed reduce the disc count to a lower limit of 3
		if (display.downDisc && (display.discChanged == false)) {
			if (discCount > 3) {
				discCount--;
				display.discChanged = true;
				display.reset = true;
			}
		}

		// Update the display
		display.Update();
		// Increment the counter used for camera roation
		counter += 0.0001f;
	}

	return 0;
}

