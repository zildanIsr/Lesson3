#include "Demo.h"



Demo::Demo() {

}


Demo::~Demo() {
}



void Demo::Init() {
	// build and compile our shader program
	// ------------------------------------
	shaderProgram = BuildShader("vertexShader.vert", "fragmentShader.frag", nullptr);

	BuildColoredCube();

	BuildColoredPlane();
}

void Demo::DeInit() {
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &EBO2);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Demo::ProcessInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void Demo::Update(double deltaTime) {

}

void Demo::Render() {
	glViewport(0, 0, this->screenWidth, this->screenHeight);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_CULL_FACE);

	glEnable(GL_DEPTH_TEST);

	// Pass perspective projection matrix
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)this->screenWidth / (GLfloat)this->screenHeight, 0.1f, 100.0f);
	GLint projLoc = glGetUniformLocation(this->shaderProgram, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// LookAt camera (position, target/direction, up)
	glm::mat4 view = glm::lookAt(glm::vec3(0, 3, 8), glm::vec3(0, 0, 0), glm::vec3(0.0f, 1.0f, 0.0f));
	GLint viewLoc = glGetUniformLocation(this->shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	DrawColoredCube();

	DrawColoredPlane();

	glDisable(GL_DEPTH_TEST);
}

void Demo::BuildColoredCube() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("crate.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
		-4.0, 1.2, 1.0, 0, 0,  // 0
		4.0, 1.2, 1.0, 1, 0,   // 1
		4.0,  1.5, 1.0, 1, 1,   // 2
		-4.0,  1.5, 1.0, 0, 1,  // 3

		// right
		4.0,  1.5,  1.0, 0, 0,  // 4
		4.0,  1.5, -1.0, 1, 0,  // 5
		4.0, 1.2, -1.0, 1, 1,  // 6
		4.0, 1.2,  1.0, 0, 1,  // 7

		// back
		-4.0, 1.2, -1.0, 0, 0, // 8 
		4.0,  1.2, -1.0, 1, 0, // 9
		4.0,   1.5, -1.0, 1, 1, // 10
		-4.0,  1.5, -1.0, 0, 1, // 11

		// left
		-4.0, 1.2, -1.0, 0, 0, // 12
		-4.0, 1.2,  1.0, 1, 0, // 13
		-4.0,  1.5,  1.0, 1, 1, // 14
		-4.0,  1.5, -1.0, 0, 1, // 15

		// upper
		4.0, 1.5,  1.0, 0, 0,   // 16
		-4.0, 1.5,  1.0, 1, 0,  // 17
		-4.0, 1.5, -1.0, 1, 1,  // 18
		4.0, 1.5, -1.0, 0, 1,   // 19

		// bottom
		-4.0, 1.2, -1.0, 0, 0, // 20
		4.0, 1.2, -1.0, 1, 0,  // 21
		4.0, 1.2,  1.0, 1, 1,  // 22
		-4.0, 1.2,  1.0, 0, 1, // 23

		//leg right side
		//front
		3.5,-2.0, 1.0, 0, 0,   // 24
		4.0,-2.0, 1.0, 1, 0,  // 25
		4.0, 1.2,1.0, 1, 1,  // 26
		3.5,1.2,  1.0, 0, 1, // 27

		//right
		4.0, 1.2, 1.0, 0, 0,	//28
		4.0, 1.2, -1.0, 1, 0,  // 29
		4.0,-2.0, -1.0, 1, 1,  // 30
		4.0,-2.0, 1.0, 0, 1,  // 31

		// back
		3.5, -2.0, -1.0, 0, 0,   // 32
		4.0, -2.0, -1.0, 1, 0,   // 33
		4.0, 1.2, -1.0, 1, 1,    // 34
		3.5, 1.2,  -1.0, 0, 1,   // 35

		//left
		3.5, -2.0, -1.0, 0, 0,	//36
		3.5, -2.0, 1.0, 1, 0,  // 37
		3.5, 1.0, 1.0, 1, 1,  // 38
		3.5, 1.0, -1.0, 0, 1,  // 39

		// upper
		4.0, 1.2,  1.0, 0, 0,   // 40
		3.5, 1.2,  1.0, 1, 0,  // 41
		3.5, 1.2,-1.0, 1, 1,  // 42
		4.0, 1.2,-1.0, 0, 1,   // 43

		// bottom
		3.5, -2.0, -1.0, 0, 0, // 44
		4.0, -2.0, -1.0, 1, 0,  // 45
		4.0, -2.0,  1.0, 1, 1,  // 46
		3.5, -2.0,  1.0, 0, 1, // 47

		//leg left side
		//front
		-4.0,-2.0, 1.0, 0, 0,   // 48
		-3.5,-2.0, 1.0, 1, 0,  // 49
		-3.5, 1.2, 1.0, 1, 1,  // 50
		-4.0, 1.2, 1.0, 0, 1, // 51

		//right
		-3.5, 1.2, 1.0, 0, 0,	//52
		-3.5, 1.2, -1.0, 1, 0,  // 53
		-3.5,-2.0, -1.0, 1, 1,  // 54
		-3.5,-2.0, 1.0, 0, 1,  // 55

		// back
		-4.0, -2.0, -1.0, 0, 0,   // 56
		-3.5, -2.0, -1.0, 1, 0,   // 57
		-3.5, 1.2, -1.0, 1, 1,    // 58
		-4.0, 1.2,  -1.0, 0, 1,   // 59

		//left
		-4.0, -2.0, -1.0, 0, 0,	//60
		-4.0, -2.0, 1.0, 1, 0,  // 61
		-4.0, 1.2, 1.0, 1, 1,  // 62
		-4.0, 1.2,-1.0, 0, 1,  // 63

		// upper
		-3.5, 1.2,  1.0, 0, 0,   // 64
		-4.0, 1.2,  1.0, 1, 0,  // 65
		-4.0, 1.2, -1.0, 1, 1,  // 66
		-3.5, 1.2, -1.0, 0, 1,   // 67

		// bottom
		-4.0, -2.0, -1.0, 0, 0, // 68
		-3.5, -2.0, -1.0, 1, 0,  // 69
		-3.5, -2.0,  1.0, 1, 1,  // 70
		-4.0, -2.0,  1.0, 0, 1, // 71

		// format position, tex coords
		// front
		-4.0, 1.2, 1.0, 0, 0,  // 0
		4.0, 1.2, 1.0, 1, 0,   // 1
		4.0,  1.5, 1.0, 1, 1,   // 2
		-4.0,  1.5, 1.0, 0, 1,  // 3

		// right
		4.0,  1.5,  1.0, 0, 0,  // 4
		4.0,  1.5, -1.0, 1, 0,  // 5
		4.0, 1.2, -1.0, 1, 1,  // 6
		4.0, 1.2,  1.0, 0, 1,  // 7

		// back
		-4.0, 1.2, -1.0, 0, 0, // 8 
		4.0,  1.2, -1.0, 1, 0, // 9
		4.0,   1.5, -1.0, 1, 1, // 10
		-4.0,  1.5, -1.0, 0, 1, // 11

		// left
		-4.0, 1.2, -1.0, 0, 0, // 12
		-4.0, 1.2,  1.0, 1, 0, // 13
		-4.0,  1.5,  1.0, 1, 1, // 14
		-4.0,  1.5, -1.0, 0, 1, // 15

		// upper
		4.0, 1.5,  1.0, 0, 0,   // 16
		-4.0, 1.5,  1.0, 1, 0,  // 17
		-4.0, 1.5, -1.0, 1, 1,  // 18
		4.0, 1.5, -1.0, 0, 1,   // 19

		// bottom
		-4.0, 1.2, -1.0, 0, 0, // 20
		4.0, 1.2, -1.0, 1, 0,  // 21
		4.0, 1.2,  1.0, 1, 1,  // 22
		-4.0, 1.2,  1.0, 0, 1, // 23

		// format position, tex coords
		// front
		-4.0, 1.2, 1.0, 0, 0,  // 0
		4.0, 1.2, 1.0, 1, 0,   // 1
		4.0, 1.5, 1.0, 1, 1,   // 2
		-4.0, 1.5, 1.0, 0, 1,  // 3

		// right
		4.0, 1.5, 1.0, 0, 0,  // 4
			4.0, 1.5, -1.0, 1, 0,  // 5
			4.0, 1.2, -1.0, 1, 1,  // 6
			4.0, 1.2, 1.0, 0, 1,  // 7

			// back
			-4.0, 1.2, -1.0, 0, 0, // 8 
			4.0, 1.2, -1.0, 1, 0, // 9
			4.0, 1.5, -1.0, 1, 1, // 10
			-4.0, 1.5, -1.0, 0, 1, // 11

			// left
			-4.0, 1.2, -1.0, 0, 0, // 12
			-4.0, 1.2, 1.0, 1, 0, // 13
			-4.0, 1.5, 1.0, 1, 1, // 14
			-4.0, 1.5, -1.0, 0, 1, // 15

			// upper
			4.0, 1.5, 1.0, 0, 0,   // 16
			-4.0, 1.5, 1.0, 1, 0,  // 17
			-4.0, 1.5, -1.0, 1, 1,  // 18
			4.0, 1.5, -1.0, 0, 1,   // 19

			// bottom
			-4.0, 1.2, -1.0, 0, 0, // 20
			4.0, 1.2, -1.0, 1, 0,  // 21
			4.0, 1.2, 1.0, 1, 1,  // 22
			-4.0, 1.2, 1.0, 0, 1, // 23

	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22,  // bottom

		//leg right
		24, 25, 26, 24, 26, 27, // front
		28, 29, 30, 28, 30, 31,// right
		32, 33, 34, 32, 34, 35, // back
		36, 38, 37, 36, 39, 38, // left
		40, 42, 41, 40, 43, 42, // upper
		44, 46, 45, 44, 47, 46, // bottom

		//leg left
		48, 49, 50, 48, 50, 51, // front
		52, 53, 54, 52, 54, 55,// right
		56, 57, 58, 56, 58, 59, // back
		60, 62, 61, 60, 63, 62, // left
		64, 66, 65, 64, 67, 66, // upper
		68, 70, 69, 68, 71, 70,// bottom

	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawColoredCube()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glDrawElements(GL_TRIANGLES,1000,GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildColoredPlane()
{
	// Load and create a texture 
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("marble.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Build geometry
	GLfloat vertices[] = {
		// format position, tex coords
		// bottom
		-50.0, -0.5, -50.0,  0,  0,
		 50.0, -0.5, -50.0, 50,  0,
		 50.0, -0.5,  50.0, 50, 50,
		-50.0, -0.5,  50.0,  0, 50,


	};

	GLuint indices[] = { 0,  2,  1,  0,  3,  2 };

	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &EBO2);

	glBindVertexArray(VAO2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO
}



void Demo::DrawColoredPlane()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 1);

	glBindVertexArray(VAO2); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

int main(int argc, char** argv) {
	RenderEngine &app = Demo();
	app.Start("Texture Mapping: Draw textured cube and plane", 800, 600, true, false);
}