#pragma once
#include "mathLibrary.h"
#include "Checkkey.h"
#include <cmath>

class FPCamera : public Transform
{
public:
    Checkkey check;
    Vec4 position;    
    Vec4 target;      
    Vec4 upVector;    
    float fov;       
    float aspect;      
    float nearPlane;   
    float farPlane;   
    float pitch=0.0f;
    float yaw=90.0f;

    FPCamera()
        : position(Vec4(0.0f, 0.0f, -5.0f, 1.0f)),
        target(Vec4(0.0f, 0.0f, 0.0f, 1.0f)),     
        upVector(Vec4(0.0f, 1.0f, 0.0f, 1.0f)),   
        fov(45.0f), aspect(4.0f / 3.0f), nearPlane(0.1f), farPlane(100.0f)
    {
        setProjectionMatrix(fov, aspect, nearPlane, farPlane); 
        setViewMatrix(position, target, upVector);          
    }

    void setPosition(const Vec4& newPosition)
    {
        position = newPosition;
        setViewMatrix(position, target, upVector); 
    }

    void setTarget(const Vec4& newTarget)
    {
        target = newTarget;
        setViewMatrix(position, target, upVector); 
    }

    void setProjection(float newFov, float newAspect, float newNear, float newFar)
    {
        fov = newFov;
        aspect = newAspect;
        nearPlane = newNear;
        farPlane = newFar;
        setProjectionMatrix(fov, aspect, nearPlane, farPlane);
    }

    void updateViewMatrix()
    {
        setViewMatrix(position, target, upVector);
    }

    const Matrix44& getViewMatrix() const
    {
        return viewMatrix;
    }

    const Matrix44& getProjectionMatrix() const
    {
        return projectionMatrix;
    }

    void setViewMatrix(const Vec4& eye, const Vec4& center, const Vec4& up)
    {
        viewMatrix = Matrix44::worldtoCam(eye, center, up);
    }

    void setProjectionMatrix(float fov, float aspect, float nearP, float farP)
    {
        projectionMatrix = Matrix44::camtoScreen(fov, aspect, nearP, farP);
    }

    void moveForward(float speed,float dt)
    {
        Vec4 forward = NormalizenoW(MinusnoW(target, position));

        if (check.keyPressed('W'))
        { 
            position = AddnoW(position,forward * speed * dt);
            target = AddnoW(target,forward * speed * dt);
             updateViewMatrix();
        }
    }

    void moveBackward(float speed, float dt)
    {
        Vec4 forward =NormalizenoW(MinusnoW(target, position));

        if (check.keyPressed('S'))
        { 
            position = MinusnoW(position, forward * speed * dt);
            target = MinusnoW(target, forward * speed * dt);
            updateViewMatrix();
        }
    }

    void moveRight(float speed,float dt)
    {
        Vec4 forward = NormalizenoW( MinusnoW(target, position));
        Vec4 right = NormalizenoW(Cross(forward, upVector)); 

        if (check.keyPressed('D'))
        { 
        position = AddnoW( position,right * speed*dt);
        target = AddnoW(target , right * speed*dt);
        updateViewMatrix();
        }
    }

    void moveLeft(float speed, float dt)
    {
        Vec4 forward = NormalizenoW(MinusnoW(target, position));
        Vec4 right = NormalizenoW(Cross(forward, upVector));

        if (check.keyPressed('A'))
        {
            position = MinusnoW(position, right * speed * dt);
            target = MinusnoW(target, right * speed * dt);
            updateViewMatrix();
        }
    }

    void processMouseInput(float mouseX, float mouseY, float sensitivity, float dt)
    {
        float deltaX = mouseX * sensitivity* dt;
        float deltaY = mouseY * sensitivity* dt;

        yaw += deltaX;
        pitch -= deltaY;

        if (pitch > 40.0f) pitch = 40.0f;
        if (pitch < -40.0f) pitch = -40.0f;

    }

    void updateCameraDirection()
    {
        float radPitch = pitch * (3.14159265f / 180.0f);
        float radYaw = yaw * (3.14159265f / 180.0f);

        Vec4 direction;
        direction.x = cosf(radYaw) * cosf(radPitch);
        direction.y = sinf(radPitch);
        direction.z = sinf(radYaw) * cosf(radPitch);
        direction.w = 0.0f;
        Vec4 positionS = Vec4(position.x - 100, position.y, position.z - 100, position.w);
        target = AddnoW(position, NormalizenoW(direction));
        updateViewMatrix();
    }

  

    //void moveUp(float amount)
//{
//    position = position + upVector * amount;
//    target = target + upVector * amount;
//    updateViewMatrix();
//}

//void moveDown(float amount)
//{
//    moveUp(-amount);
//}

};

class FPcamManager
{
public:
    FPCamera camera;

    void setAll( Vec4 eye, Vec4 center, Vec4 up, float fov, float aspectRatio, float nearPlane, float farPlane)
    {
        camera.setPosition(eye);
        camera.setTarget(center);
        camera.setViewMatrix(eye, center, up);
        camera.setProjectionMatrix(fov, aspectRatio, nearPlane, farPlane);
    }

    void cameraMovement(Checkmouse& checkMouse, float sensitivity, float dt)
    {
        float mouseX, mouseY;
        checkMouse.getMouseMovement(mouseX, mouseY);
        camera.processMouseInput(mouseX, mouseY, sensitivity, dt);
        camera.updateCameraDirection();
        camera.moveForward(700.f, dt);
        camera.moveBackward(500.f, dt);
        camera.moveRight(500.f, dt);
        camera.moveLeft(500.f, dt);
    }

};
