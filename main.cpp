#include <cstring>
#include <format>
#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

using std::cout, std::endl, std::string;

static bool readFile(const char* filename, string& outfile);

struct Vertex {
  glm::vec3 pos;
  glm::vec3 color;

  Vertex() {}

  Vertex(float x, float y, float z) {
    pos = glm::vec3(x, y, z);

    float red = (float)rand() / (float)RAND_MAX;
    float green = (float)rand() / (float)RAND_MAX;
    float blue = (float)rand() / (float)RAND_MAX;
    color = glm::vec3(red, green, blue);
  }

  Vertex(float x, float y) {
    pos = glm::vec3(x, y, 0.0f);

    float red = (float)rand() / (float)RAND_MAX;
    float green = (float)rand() / (float)RAND_MAX;
    float blue = (float)rand() / (float)RAND_MAX;
    color = glm::vec3(red, green, blue);
  }
};

class Engine {
public:
  Engine();
  ~Engine();

  bool init();

  void mainloop();

private:
  bool createCubeVAO();
  bool createPoleVAO();
  bool compileShaders();
  bool addShader(GLuint shaderProgram, const char* pShaderText, GLenum shaderType);
  void render();

  GLFWwindow* window;

  GLuint cubeVAO = -1;
  GLuint cubeVBO = -1;
  GLuint cubeIBO = -1;

  GLuint poleVAO = -1;
  GLuint poleVBO = -1;
  GLuint poleIBO = -1;

  GLint mvpLocation;
  GLint timeLocation;

  void processInput(GLFWwindow* window);
  void resizeWindow(GLFWwindow* window, int width, int height);

  clock_t starttime;

  float x = 0.0f;
  float delta = 0.01f;

  int windowWidth, windowHeight;
};

Engine::Engine() {
  windowWidth = WINDOW_WIDTH;
  windowHeight = WINDOW_HEIGHT;
}

Engine::~Engine() {
  if (cubeVAO != -1) {
    glDeleteVertexArrays(1, &cubeVAO);
  }
  if (cubeVBO != -1) {
    glDeleteBuffers(1, &cubeVBO);
  }
  if (cubeIBO != -1) {
    glDeleteBuffers(1, &cubeIBO);
  }
  if (poleVAO != -1) {
    glDeleteVertexArrays(1, &poleVAO);
  }
  if (poleVBO != -1) {
    glDeleteBuffers(1, &poleVBO);
  }
  if (poleIBO != -1) {
    glDeleteBuffers(1, &poleIBO);
  }
  glfwTerminate();
}

bool Engine::init() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_POSITION_X, 900);
  glfwWindowHint(GLFW_POSITION_Y, 400);
  //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  window = glfwCreateWindow(windowWidth, windowHeight, "lets go", NULL, NULL);
  if (window == NULL) {
    cout << "Failed to create GLFW window" << endl;
    glfwTerminate();
    return false;
  }
  glfwMakeContextCurrent(window);

  GLenum res = glewInit();
  if (res != GLEW_OK) {
    cout << "Failed to initialize GLEW" << endl;
    return false;
  }

  glViewport(0, 0, windowWidth, windowHeight);

  glfwSetWindowUserPointer(window, this);

  auto func = [](GLFWwindow* w, int x, int y)
  {
    static_cast<Engine*>(glfwGetWindowUserPointer(w))->resizeWindow(w, x, y);
  };

  glfwSetFramebufferSizeCallback(window, func);
  GLclampf red = 0.0f, green = 0.0f, blue = 0.0f, alpha = 0.0f;
  glClearColor(red, green, blue, alpha);

  glEnable(GL_CULL_FACE);
  glFrontFace(GL_CW);
  glCullFace(GL_BACK);

  if (!createCubeVAO()) return false;
  if (!createPoleVAO()) return false;

  glBindVertexArray(cubeVAO);

  if (!compileShaders()) return false;

  starttime = clock();

  return true;
}

void Engine::mainloop() {
  // render loop
  while (!glfwWindowShouldClose(window)) {
    // input
    processInput(window);

    // render
    render();

    // check and call events, and swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}


bool Engine::createCubeVAO() {
  glGenVertexArrays(1, &cubeVAO);
  glBindVertexArray(cubeVAO);

  Vertex vertices[8];

  vertices[0] = Vertex(0.5f, 0.5f, 0.5f);
  vertices[1] = Vertex(-0.5f, 0.5f, -0.5f);
  vertices[2] = Vertex(-0.5f, 0.5f, 0.5f);
  vertices[3] = Vertex(0.5f, -0.5f, -0.5f);
  vertices[4] = Vertex(-0.5f, -0.5f, -0.5f);
  vertices[5] = Vertex(0.5f, 0.5f, -0.5f);
  vertices[6] = Vertex(0.5f, -0.5f, 0.5f);
  vertices[7] = Vertex(-0.5f, -0.5f, 0.5f);

  glGenBuffers(1, &cubeVBO);
  glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // position
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

  // color
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

  unsigned int indices[] = {
                             0, 1, 2,
                             1, 3, 4,
                             5, 6, 3,
                             7, 3, 6,
                             2, 4, 7,
                             0, 7, 6,
                             0, 5, 1,
                             1, 5, 3,
                             5, 0, 6,
                             7, 4, 3,
                             2, 1, 4,
                             0, 2, 7
  };

  glGenBuffers(1, &cubeIBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeIBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glBindVertexArray(0);
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  return true;
}

bool Engine::createPoleVAO() {
  glGenVertexArrays(1, &poleVAO);
  glBindVertexArray(poleVAO);
  Vertex vertices[19];

  // center
  vertices[0] = Vertex(0.0f, 0.0f);

  // top row
  vertices[1] = Vertex(-1.0f,  1.0f);
  vertices[2] = Vertex(-0.75f, 1.0f);
  vertices[3] = Vertex(-0.5f,  1.0f);
  vertices[4] = Vertex(-0.25f, 1.0f);
  vertices[5] = Vertex(-0.0f,  1.0f);
  vertices[6] = Vertex(0.25f,  1.0f);
  vertices[7] = Vertex(0.5f,   1.0f);
  vertices[8] = Vertex(0.75f,  1.0f);
  vertices[9] = Vertex(1.0f,   1.0f);

  // bottom row
  vertices[10] = Vertex(-1.0f,  -1.0f);
  vertices[11] = Vertex(-0.75f, -1.0f);
  vertices[12] = Vertex(-0.5f,  -1.0f);
  vertices[13] = Vertex(-0.25f, -1.0f);
  vertices[14] = Vertex(-0.0f,  -1.0f);
  vertices[15] = Vertex(0.25f,  -1.0f);
  vertices[16] = Vertex(0.5f,   -1.0f);
  vertices[17] = Vertex(0.75f,  -1.0f);
  vertices[18] = Vertex(1.0f,   -1.0f);

  glGenBuffers(1, &poleVBO);
  glBindBuffer(GL_ARRAY_BUFFER, poleVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // position
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

  // color
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));


  unsigned int indices[] = { // top triangles
    0, 2, 1,
    0, 3, 2,
    0, 4, 3,
    0, 5, 4,
    0, 6, 5,
    0, 7, 6,
    0, 8, 7,
    0, 9, 8,

    // bottom triangles
    0, 10, 11,
    0, 11, 12,
    0, 12, 13,
    0, 13, 14,
    0, 14, 15,
    0, 15, 16,
    0, 16, 17,
    0, 17, 18,

    // left triangle
    0, 1, 10,

    // right triangle
    0, 18, 9 };

  glGenBuffers(1, &poleIBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, poleIBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glBindVertexArray(0);
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  return true;
}

bool Engine::compileShaders() {
  GLuint shaderProgram = glCreateProgram();

  if (shaderProgram == 0) {
    cout << "Error creating shader program" << endl;
    return false;
  }

  string vs, fs;

  if (!readFile("../standard.vert", vs)) {
    cout << "Error reading vertex shader" << endl;
    return false;
  }

  if (!addShader(shaderProgram, vs.c_str(), GL_VERTEX_SHADER)) {
    return false;
  }

  if (!readFile("../standard.frag", fs)) {
    cout << "Error reading fragment shader" << endl;
    return false;
  }

  if (!addShader(shaderProgram, fs.c_str(), GL_FRAGMENT_SHADER)) {
    return false;
  }

  GLint success = 0;
  GLchar errorLog[1024] = { 0 };

  glLinkProgram(shaderProgram);
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

  if (success == 0) {
    glGetProgramInfoLog(shaderProgram, sizeof(errorLog), NULL, errorLog);
    cout << std::format("Error linking shader program: {}", errorLog) << endl;
    return false;
  }

  mvpLocation = glGetUniformLocation(shaderProgram, "mvp");
  if (mvpLocation == -1) {
    cout << "Error getting uniform location of 'mvp'" << endl;
    return false;
  }

  timeLocation = glGetUniformLocation(shaderProgram, "time");
  if (timeLocation == -1) {
    cout << "Error getting uniform location of 'time'" << endl;
    return false;
  }

  glValidateProgram(shaderProgram);
  glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, sizeof(errorLog), NULL, errorLog);
    cout << std::format("Invalid shader program: {}", errorLog) << endl;
    return false;
  }

  glUseProgram(shaderProgram);

  return true;
}

bool Engine::addShader(GLuint shaderProgram, const char *pShaderText, GLenum shaderType) {
  GLuint shaderObj = glCreateShader(shaderType);

  if (shaderObj == 0) {
    cout << std::format("Error creating shader type {}", shaderType) << endl;
    return false;
  }

  const GLchar* p[1];
  p[0] = pShaderText;

  GLint lengths[1];
  lengths[0] = std::strlen(pShaderText);

  glShaderSource(shaderObj, 1, p, lengths);

  glCompileShader(shaderObj);

  GLint success;
  glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &success);

  if (!success) {
    GLchar infoLog[1024];
    glGetShaderInfoLog(shaderObj, 1024, NULL, infoLog);
    cout << std::format("Error compiling shader type {}: {}", shaderType, infoLog) << endl;
    return false;
  }

  glAttachShader(shaderProgram, shaderObj);

  return true;
}

void Engine::render() {
  glClear(GL_COLOR_BUFFER_BIT);

  x += delta;
  if ((x >= 6.28f) || (x <= -6.28f)) {
    x *= -1.0f;
  }

  float scale = 1.0f;

  glm::mat4x4 scaling = glm::mat4x4(scale, 0.0f, 0.0f, 0.0f,
                                    0.0f, scale, 0.0f, 0.0f,
                                    0.0f, 0.0f, scale, 0.0f,
                                    0.0f, 0.0f, 0.0f, 1.0f);

  glm::mat4x4 rotation = glm::mat4x4(glm::cos(x), 0.0f,  glm::sin(x), 0.0f,
                                     0.0f,        1.0f,  0.0f,         0.0f,
                                     -glm::sin(x), 0.0f, glm::cos(x), 0.0f,
                                     0.0f,        0.0f,  0.0f,        1.0f);

  glm::mat4x4 translation = glm::mat4x4(1.0f, 0.0f, 0.0f, 0.0f,
                                        0.0f, 1.0f, 0.0f, 0.0f,
                                        0.0f, 0.0f, 1.0f, 0.0f,
                                        0.0f, 0.0f, 2.0f, 1.0f);

  // IMPORTANT!! it seems that these matrices are multiplied LEFT TO RIGHT
  // and for a TRS matrix we wanna do SCALE, ROTATION then TRANSLATION
  glm::mat4 world = translation * rotation * scaling;

  glm::vec3 camPos(0.0f, 0.0f, 0.0f);

  glm::vec3 u(1.0f, 0.0f, 0.0f);
  glm::vec3 v(0.0f, 1.0f, 0.0f);
  glm::vec3 n(0.0f, 0.0f, 1.0f);

  glm::mat4 camera(u.x,       v.x,       n.x,       0.0f,
                   u.y,       v.y,       n.y,        0.0f,
                   u.z,       v.z,       n.z,       0.0f,
                   -camPos.x, -camPos.y, -camPos.z, 1.0f);

  float FOV = 90.0f;
  float tanHalfFOV = glm::tan(glm::radians(FOV / 2.0f));
  float d = 1.0f/tanHalfFOV;

  float ar = (float)windowWidth / (float)windowHeight;

  float nearZ = 1.0f;
  float farZ = 10.0f;

  float zRange = nearZ - farZ;
  float a = (-farZ - nearZ)/zRange;
  float b = (2*farZ*nearZ)/zRange;

  glm::mat4x4 projection = glm::mat4x4(d/ar, 0.0f, 0.0f, 0.0f,
                                       0.0f, d,    0.0f,  0.0f,
                                       0.0f, 0.0f, a,    1.0f,
                                       0.0f, 0.0f, b,    0.0f);

  glm::mat4x4 mvp = projection * camera * world;
  // glm::mat4x4 transform = glm::mat4x4(1.0f, 0.0f, 0.0f, 0.0f,
  //                                     0.0f, 1.0f, 0.0f, 0.0f,
  //                                     0.0f, 0.0f, 1.0f, 0.0f,
  //                                     0.0f, 0.0f, 0.0f, 1.0f);

  auto currenttime = clock();
  auto time = float(currenttime - starttime) / float(CLOCKS_PER_SEC);

  glUniform1f(timeLocation, time);
  glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, &mvp[0][0]);

  GLint currentVAO;
  glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentVAO);

  if (currentVAO == cubeVAO) {
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
  }
  else {
    glDrawElements(GL_TRIANGLES, 54, GL_UNSIGNED_INT, 0);
  }
}

void Engine::processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
  if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    glBindVertexArray(cubeVAO);
  if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    glBindVertexArray(poleVAO);
}

void Engine::resizeWindow(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
  windowWidth = width;
  windowHeight = height;
}

int main() {
  srand(getpid());

  auto engine = Engine();
  if (!engine.init()) return 1;

  engine.mainloop();

  return 0;
}

static bool readFile(const char* filename, string& outfile) {
  std::ifstream f(filename);

  if (f.fail()) {
    cout << std::strerror(errno) << endl;
  }

  bool ret = false;
  if (f.is_open()) {
    string line;
    while (std::getline(f, line)) {
      outfile.append(line);
      outfile.append("\n");
    }

    f.close();

    ret = true;
  }

  return ret;
}