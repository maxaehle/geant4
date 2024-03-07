//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
//
//
// Class to provide Vertex Buffer Object (VBO) specific
// functionality for OpenGL > 2.0 in GEANT4

#include "G4OpenGLViewer.hh"
#ifdef G4OPENGL_VERSION_2

#include "G4OpenGLVboDrawer.hh"

#include "G4OpenGLImmediateQtViewer.hh"


//////////////////////////////////////////////////////////////////////////////
G4OpenGLVboDrawer::G4OpenGLVboDrawer (G4OpenGLViewer* viewer,
                                      std::string type
                                      ):
fVboViewer(NULL),
fOGLType(type)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  G4OpenGLImmediateQtViewer* v = dynamic_cast<G4OpenGLImmediateQtViewer*>(viewer);
  if (v) {
    fVboViewer = v;
  }
  
  fFragmentShaderSrc =
  "#ifdef GL_ES\n"
  "precision highp G4float;\n"
  "#endif\n"
  "\n"
  "varying vec3 vLightWeighting;\n"
  "uniform vec4 uPointColor; // Point Color\n"
  "\n"
  "void main(void) {\n"
  "  vec4 matColor = uPointColor;\n"
  "  gl_FragColor = vec4(matColor.rgb, matColor.a);\n"
  "}\n";
  
  
  fVertexShaderSrc =
  "attribute highp vec4 aVertexPosition;\n"
  "attribute vec3 aVertexNormal;\n"
  "uniform highp mat4 uCMatrix;\n"
  "uniform highp mat4 uPMatrix;  // Perspective [P]rojection matrix\n"
  "uniform highp mat4 uMVMatrix; // [M]odel[V]iew matrix\n"
  "uniform highp mat4 uTMatrix;  // [T]ransformation  matrix\n"
  "uniform G4float uPointSize;  // Point size\n"
  "void main(void)\n"
  "{\n"
  "   gl_Position = uPMatrix * uCMatrix * uMVMatrix * uTMatrix * aVertexPosition;\n"
  "  // Phong shading\n"
  //                                               "  vec3 transformedNormal = normalize((uNMatrix * vec4(normalize(aVertexNormal), 0)).xyz);\n"
  "  vec3 lightingDirection = normalize(vec3(1, 1, 1));\n"
  //                                               "  G4float directionalLightWeighting = max(dot(transformedNormal, lightingDirection), 0.0);\n"
  //                                               "  vec3 uAmbientLightColor = vec3(0.2, 0.2, 0.2);\n"
  //                                               "  vec3 uDirectionalColor = vec3(0.8, 0.8, 0.8);\n"
  "  gl_PointSize = uPointSize;\n"
  //                                               "  vLightWeighting = uAmbientLightColor + uDirectionalColor * directionalLightWeighting;\n"
  "}";
}

//////////////////////////////////////////////////////////////////////////////
G4OpenGLVboDrawer::~G4OpenGLVboDrawer (
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}

// +--------------------------------+
// +        QT (OpenGL ES) case     +
// +--------------------------------+

void G4OpenGLVboDrawer:: vboGlMultMatrixf(const GLG4float *matrix){
  if (fVboViewer) {
    if (fVboViewer->isInitialized()) {
      // FIXME
      // glUniformMatrix4fv(12, 1, 0, 0x7fff5fbf5d00)
      //  Error: GL_INVALID_OPERATION
      
      if (fMatrixMode == GL_MODELVIEW) {
        glUniformMatrix4fv(fVboViewer->getShaderTransformMatrix(),1,0,matrix);
      } else {
        G4cerr << "glMultMatrixf could only be used in GL_MODELVIEW mode" << G4endl;
      }
    }
  }
}


void G4OpenGLVboDrawer:: vboGlMultMatrixd(const GLG4double *matrix){
  if (fVboViewer) {
    if (fVboViewer->isInitialized()) {
      // FIXME !
      //    if (fMatrixMode == GL_MODELVIEW) {
      //      printf("G4OpenGLVboDrawer:: vboGlMultMatrixd %d %d\n",fVboViewer->getShaderTransformMatrix(), matrix);
      //!! TEST !!
      G4float mat[16] = {
        matrix[0],matrix[1],matrix[2],matrix[3],
        matrix[4],matrix[5],matrix[6],matrix[7],
        matrix[8],matrix[9],matrix[10],matrix[11],
        matrix[12],matrix[13],matrix[14],matrix[15]
      };

      glUniformMatrix4fv(fVboViewer->getShaderTransformMatrix(),1,0,mat);
      GLenum e = glGetError();
      printf("GL error : %d",e);
      //    } else {
      //      G4cerr << "glMultMatrixd could only be used in GL_MODELVIEW mode" << G4endl;
      //    }
    }
  }
}



// +--------------------------------+
// +        All case     +
// +--------------------------------+



void G4OpenGLVboDrawer::vboGlOrtho(GLG4double left, GLG4double right, GLG4double bottom, GLG4double top, GLG4double zNear, GLG4double zFar) {
  if (fVboViewer) {
    if (fVboViewer->isInitialized()) {
      printf("glOrtho implemented --- %f %f %f %f %f %f \n",left, right, bottom, top, zNear, zFar);
      G4float a = 2.0f / (right - left);
      G4float b = 2.0f / (top - bottom);
      G4float c = -2.0f / (zFar - zNear);
      
      G4float tx = - (right + left)/(right - left);
      G4float ty = - (top + bottom)/(top - bottom);
      G4float tz = - (zFar + zNear)/(zFar - zNear);
      
      G4float ortho[16] = {
        a, 0, 0, 0,
        0, b, 0, 0,
        0, 0, c, 0,
        tx, ty, tz, 1
      };
      // FIXME :
      //  glUniformMatrix4fv(0, 1, 0, 0x7fff5fbf5d00)
      // Error: GL_INVALID_OPERATION
      
      if (fMatrixMode == GL_PROJECTION) {
        glUniformMatrix4fv(fVboViewer->getShaderProjectionMatrix(), 1, 0, ortho);
      } else {
        G4cerr << "glFrustum could only be used in GL_PROJECTION mode" << G4endl;
      }
    }
  }
}


void G4OpenGLVboDrawer::vboGlFrustum(GLG4double left, GLG4double right, GLG4double bottom, GLG4double top, GLG4double zNear, GLG4double zFar) {
  if (fVboViewer) {
    if (fVboViewer->isInitialized()) {
      G4float deltaX = right - left;
      G4float deltaY = top - bottom;
      G4float deltaZ = zFar - zNear;
      
      G4float a = 2.0f * zNear / deltaX;
      G4float b = 2.0f * zNear / deltaY;
      G4float c = (right + left) / deltaX;
      G4float d = (top + bottom) / deltaY;
      G4float e = -(zFar + zNear) / (zFar - zNear);
      G4float f = -2.0f * zFar * zNear / deltaZ;
      
      G4float proj[16] = {
        a, 0, 0, 0,
        0, b, 0, 0,
        c, d, e, -1.0f,
        0, 0, f, 0
      };
      
      if (fMatrixMode == GL_PROJECTION) {
        glUniformMatrix4fv(fVboViewer->getShaderProjectionMatrix(), 1, 0, proj);
      } else {
        G4cerr << "glFrustrum could only be used in GL_PROJECTION mode" << G4endl;
      }
    }
  }
}


void G4OpenGLVboDrawer::vboGlMatrixMode(GLenum a) {
  if (fVboViewer) {
    if (fVboViewer->isInitialized()) {
      printf("G4OpenGLVboDrawer::vboGlMatrixMode CHANGED :%d \n",a);
      fMatrixMode = a;
    }
  }
}


void G4OpenGLVboDrawer::vboGlColor4d(int red,int green,int blue,int alpha) {
  if (fVboViewer) {
    if (fVboViewer->isInitialized()) {
      //    G4double color [] = { red, green, blue, alpha };
      // FIXME : REMOVE /2 , used to render transparents for testing purpose
      alpha = 0.7;
      glUniform4f (glGetUniformLocation(fVboViewer->getShaderProgram(), "uPointColor"),red, green, blue, alpha);
    }
  }
}

void G4OpenGLVboDrawer:: vboGlColor4fv(const GLG4float* data) {
  if (fVboViewer) {
    if (fVboViewer->isInitialized()) {
      G4double color [] = { (data[0]), (data[1]), (data[2]), 0.7};
      // FIXME : REMOVE /2 , used to render transparents for testing purpose
      glUniform4f (glGetUniformLocation(fVboViewer->getShaderProgram(), "uPointColor"),color[0],color[1],color[2], color[3]);
    }
  }
}

void G4OpenGLVboDrawer:: vboGlPointSize(G4float size) {
  if (fVboViewer) {
    if (fVboViewer->isInitialized()) {
      glUniform1f (glGetUniformLocation(fVboViewer->getShaderProgram(), "uPointSize"),size);
    }
  }
}

#endif

