/*****************************************************************************/
/* This is the program skeleton for homework 2 in CSE167 by Ravi Ramamoorthi */
/* Extends HW 1 to deal with shading, more transforms and multiple objects   */
/*****************************************************************************/

// This file is display.cpp.  It includes the skeleton for the display routine

// Basic includes to get this file to work.
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
// OSX systems require different headers
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif
#include "Transform.h"
#include "Geometry.h"

using namespace std ;
#include "variables.h"
#include "readfile.h"

// New helper transformation function to transform vector by modelview
// May be better done using newer glm functionality.
// Provided for your convenience.  Use is optional.
// Some of you may want to use the more modern routines in readfile.cpp
// that can also be used.
void transformvec (const GLfloat input[4], GLfloat output[4])
{
  glm::vec4 inputvec(input[0], input[1], input[2], input[3]);
  glm::vec4 outputvec = modelview * inputvec;
  output[0] = outputvec[0];
  output[1] = outputvec[1];
  output[2] = outputvec[2];
  output[3] = outputvec[3];
}

void display()
{
  // glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
  glClearColor(0, 0, 1, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Either use the built-in lookAt function or the lookAt implemented by the user.
  if (useGlu) {
    modelview = glm::lookAt(eye,center,up);
  } else {
    modelview = Transform::lookAt(eye,center,up);
  }
  glUniformMatrix4fv(modelviewPos, 1, GL_FALSE, &modelview[0][0]);

  // Lights are transformed by current modelview matrix.
  // The shader can't do this globally.
  // So we need to do so manually.
  if (numused) {
    glUniform1i(enablelighting,true);

    // YOUR CODE FOR HW 2 HERE.
    // You need to pass the light positions and colors to the shader.
    // glUniform4fv() and similar functions will be useful. See FAQ for help with these functions.
    // The lightransf[] array in variables.h and transformvec() might also be useful here.
    // Remember that light positions must be transformed by modelview.

    	for (size_t i = 0; i < numused; i++)
    	{
    		GLfloat orig[4] = { lightposn[(i * 4)], lightposn[(i * 4) + 1], lightposn[(i * 4) + 2], lightposn[(i * 4) + 3] };
    		GLfloat trans[4];
    		transformvec(orig, trans);
    		lightransf[(i * 4)] = trans[0];
    		lightransf[(i * 4) + 1] = trans[1];
    		lightransf[(i * 4) + 2] = trans[2];
    		lightransf[(i * 4) + 3] = trans[3];
    	}
    	glUniform1i(numusedcol, numused); //set the variable number of lights used
    	glUniform4fv(lightpos, numused, lightransf);//change to handle postion of the light position
    	glUniform4fv(lightcol, numused, lightcolor);//stores the color of the light

  } else {
    glUniform1i(enablelighting,false);
  }

  // Transformations for objects, involving translation and scaling
  mat4 sc(1.0) , tr(1.0), transf(1.0);
  sc = Transform::scale(sx,sy,1.0);
  tr = Transform::translate(tx,ty,0.0);

  // YOUR CODE FOR HW 2 HERE.
  // You need to use scale, translate and modelview to
  // set up the net transformation matrix for the objects.
  // Account for GLM issues, matrix order, etc.
  transf = modelview *tr*sc; //multiply the current modelview matrix with the tranlate and the scale matrix.

  // The object draw functions will need to further modify the top of the stack,
  // so assign whatever transformation matrix you intend to work with to modelview
  // rather than use a uniform variable for that.
  modelview = transf; // append the transf matrix to modelView

  for (int i = 0 ; i < numobjects ; i++) {
    object* obj = &(objects[i]); // Grabs an object struct.

    // YOUR CODE FOR HW 2 HERE.
    // Set up the object transformations
    // And pass in the appropriate material properties
    // Again glUniform() related functions will be useful

    //load the ambient, diffuse, specular, emission, shininess into the Fragment shader
    glUniform4fv(ambientcol, 1, obj->ambient);
  	glUniform4fv(diffusecol, 1, obj->diffuse);
    glUniform4fv(specularcol, 1, obj->specular);
  	glUniform4fv(emissioncol, 1, obj->emission);
  	const GLfloat tempshininess[] = { obj->shininess };
  	glUniform1fv(shininesscol, 1, tempshininess);

  	mat4 mvp = transf*obj->transform;
    modelview = mvp; // load the objects to the modelview

    // Actually draw the object
    // We provide the actual drawing functions for you.
    // Remember that obj->type is notation for accessing struct fields
    if (obj->type == cube) {
      solidCube(obj->size);
    }
    else if (obj->type == sphere) {
      const int tessel = 20;
      solidSphere(obj->size, tessel, tessel);
    }
    else if (obj->type == teapot) {
      solidTeapot(obj->size);
    }
  }

  glutSwapBuffers();
}
