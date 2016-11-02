void runOv4(GLFWwindow* window){

  unsigned int vaoid = createCircleVAO(3,3);
  int indLen = 3 * 3 * 3 * 2;
  //unsigned int vaoid = createVAO(vertices,vertLen,indices,indLen,colors,colorsLen);
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
