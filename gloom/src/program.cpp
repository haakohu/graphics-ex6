// Local headers
#include "program.hpp"
#include "gloom/gloom.hpp"
#include "gloom/shader.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/vec3.hpp"
#include "glm/gtc/matrix_transform.hpp"

glm::vec3 cameraPosition = glm::vec3(0,0,5);
float horRotate = 0.0;
float verRotate = 0.0;



void setup(GLFWwindow* window){
  glfwSetKeyCallback(window, keyboardCallback);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_CULL_FACE);
  glClearColor(0.3f, 0.3f, 0.4f, 1.0f);
  printGLError();
}
void runProgram(GLFWwindow* window){

  setup(window);
  int slices = 10;
  int layers = 10;
  int PRIMITIVES_PER_RECTANGLE = 2;
  int VERTICES_PER_TRIANGLE = 3;
  int vaoid = createCircleVAO(slices,layers);
  int indLen = slices * layers * PRIMITIVES_PER_RECTANGLE * VERTICES_PER_TRIANGLE;
  Gloom::Shader shader;
  shader.makeBasicShader("/home/shomec/h/haakohu/Documents/programmering/tdt4195/graphics/ov4/gloom/gloom/shaders/simple.vert","/home/shomec/h/haakohu/Documents/programmering/tdt4195/graphics/ov4/gloom/gloom/shaders/simple.frag");

  while (!glfwWindowShouldClose(window))
  {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glm::mat4 view = glm::rotate(glm::mat4(1.f), horRotate, glm::vec3(0,1,0));
      view = glm::rotate(view,verRotate, glm::vec3(1,0,0));
      view = glm::translate(view,-cameraPosition);

      glm::mat4x4 projection = glm::perspective(glm::radians(45.f),1.f,1.f,100.f);
      //glm::mat4x4 projection = glm::perspective(45.f,1.f, 1.f,100.f);
      glm::mat4x4 transform2 = projection*view;
      // Draw your scene here
      shader.activate();
      glBindVertexArray(vaoid);
      glUniformMatrix4fv(3,1,GL_FALSE,&transform2[0][0]);
      glDrawElements(GL_TRIANGLES, indLen, GL_UNSIGNED_INT,0);
      shader.deactivate();

      glfwPollEvents();
      glfwSwapBuffers(window);
  }
}

void runProgram2(GLFWwindow* window){
    setup(window);
    int slices = 40;
    int layers = 40;
    int PRIMITIVES_PER_RECTANGLE = 2;
    int VERTICES_PER_TRIANGLE = 3;
    int vaoid = createCircleVAO(slices,layers);
    int indLen = slices * layers * PRIMITIVES_PER_RECTANGLE * VERTICES_PER_TRIANGLE;
    Gloom::Shader shader;
    shader.makeBasicShader("/home/shomec/h/haakohu/Documents/programmering/tdt4195/graphics/ov4/gloom/gloom/shaders/simple.vert","/home/shomec/h/haakohu/Documents/programmering/tdt4195/graphics/ov4/gloom/gloom/shaders/simple.frag");
    SceneNode* mainPlanet = generateSystem();
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::mat4 view = glm::rotate(glm::mat4(1.f), horRotate, glm::vec3(0,1,0));
        view = glm::rotate(view,verRotate, glm::vec3(1,0,0));
        view = glm::translate(view,-cameraPosition);

        glm::mat4x4 projection = glm::perspective(glm::radians(45.f),1.f,1.f,100.f);
        //glm::mat4x4 projection = glm::perspective(45.f,1.f, 1.f,100.f);
        glm::mat4x4 transform2 = projection*view;
        // Draw your scene here
        shader.activate();
        glBindVertexArray(mainPlanet->vertexArrayObjectID);
        glUniformMatrix4fv(3,1,GL_FALSE,&transform2[0][0]);
        glDrawElements(GL_TRIANGLES, indLen, GL_UNSIGNED_INT,0);
        shader.deactivate();

        glfwPollEvents();
        glfwSwapBuffers(window);
    }
}

void updatePlanet(SceneNode* planet,float timeDelta,glm::mat4 oldMat){
    if(timeDelta == NULL){
      timeDelta = getTimeDeltaSeconds();
      oldMat = glm::mat4(1);
    }
    float moved = planet->rotationSpeedRadians * timeDelta;
    glm::vec3 rotationVector = planet->rotationDirection;
    glm::mat4 rotationDirection = glm::mat4(1);
    rotationDirection[0][0] = rotationVector.x;
    rotationDirection[1][1] = rotationVector.y;
    rotationDirection[2][2] = rotationVector.z;
    glm::mat4 transMat = moved * oldMat * rotationDirection;
    planet->currentTransformationMatrix = transMat;
    for(int i = 0; planet->children[i] != 0 ; i++){
      updatePlanet(planet->children[i],timeDelta,transMat);
    }


}

// Man ganger det på bunnen først, altså tærne LOL

SceneNode* generateSystem(){
  SceneNode* sun = createSceneNode();
  SceneNode* moon = createSceneNode();
  SceneNode* planet = createSceneNode();
  addChild(sun,planet);
  addChild(planet,moon);
  sun->rotationSpeedRadians = 1;
  sun->rotationDirection = glm::vec3(0,1,0);
  moon->rotationSpeedRadians = 3;
  moon->rotationDirection = glm::vec3(1,0,0);
  planet->rotationDirection = glm::vec3(0.5,0.5,0);
  planet->rotationSpeedRadians = 2;
  int slices = 40;
  int layers = 40;
  int PRIMITIVES_PER_RECTANGLE = 2;
  int VERTICES_PER_TRIANGLE = 3;
  int vaoid = createCircleVAO(slices,layers);
  planet->vertexArrayObjectID = vaoid;
  return sun;
}

unsigned int createVAO(float* vertices, int vertLen, unsigned int* indices, int indLen, float* colors, int colorsLen){
  unsigned int vaoid = 0;
  glGenVertexArrays(1,&vaoid);
  glBindVertexArray(vaoid);

  unsigned int vboid = 0;
  glGenBuffers(1,&vboid);
  glBindBuffer(GL_ARRAY_BUFFER, vboid);
  glBufferData(GL_ARRAY_BUFFER, vertLen*sizeof(float), vertices, GL_STATIC_DRAW);
  unsigned int vaid = 0;
  glVertexAttribPointer(vaid,3,GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vaid);

  unsigned int vboid2= 0;
  glGenBuffers(1,&vboid2);
  glBindBuffer(GL_ARRAY_BUFFER,vboid2);
  glBufferData(GL_ARRAY_BUFFER,colorsLen*sizeof(float),colors,GL_STATIC_DRAW);
  unsigned int vaid2 = 1;
  glVertexAttribPointer(vaid2,4,GL_FLOAT,GL_FALSE,0,0);
  glEnableVertexAttribArray(vaid2);

  unsigned int indid = 0;
  glGenBuffers(1,&indid);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,indid);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indLen*sizeof(unsigned int),indices, GL_STATIC_DRAW);
  return vaoid;
}



void keyboardCallback(GLFWwindow* window, int key, int scancode,
                      int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
      fprintf(stderr, "Could not open GLFW window\n");
      glfwSetWindowShouldClose(window, GL_TRUE);
    }
    //right
    else if (key == GLFW_KEY_D && action == GLFW_PRESS){
      cameraPosition.x = cameraPosition.x + 0.1;
    }
    //left
    else if (key == GLFW_KEY_A && action == GLFW_PRESS){
      cameraPosition.x = cameraPosition.x -0.1;
    }
    //up
    else if (key == GLFW_KEY_W && action == GLFW_PRESS){
      cameraPosition.y = cameraPosition.y +0.1;
    }
    //down
    else if (key == GLFW_KEY_S && action == GLFW_PRESS){
      cameraPosition.y = cameraPosition.y-0.1;
    }
    //forward
    else if (key == GLFW_KEY_R && action == GLFW_PRESS){
      cameraPosition.z -= 0.1;
    }
    //backward
    else if (key == GLFW_KEY_F && action == GLFW_PRESS){
      cameraPosition.z += 0.1;
    }
    //horizontal left
    else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS){
      horRotate += 0.1;
    }
    //horizontal right
    else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS){
      horRotate -= 0.1;
    }
    //vertical up
    else if (key == GLFW_KEY_UP && action == GLFW_PRESS){
      verRotate += 0.1;
    }
    //vertical down
    else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS){
      verRotate -= 0.1;
    }

}
