#ifndef SHADERS_H
#define SHADERS_H

bool setShaders();

void useShader();

void destroyShaders();

void setProjMatrix(float* projPointer);

void setViewMatrix(float* viewPointer);

void setWorldMatrix(float* worldPointer);

#endif // SHADERS_H
