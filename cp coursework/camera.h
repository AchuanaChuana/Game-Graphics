#pragma once
#include "mathLibrary.h"
#include "Checkkey.h"
#include <cmath>

class FPCamera : public Transform
{
public:
    Checkkey check;
    Vec4 position;      // 相机位置
    Vec4 target;        // 目标点（视线指向的方向）
    Vec4 upVector;      // 上方向向量
    float fov;          // 视场角（Field of View）
    float aspect;       // 屏幕宽高比
    float nearPlane;    // 近平面
    float farPlane;     // 远平面

    // 构造函数
    FPCamera()
        : position(Vec4(0.0f, 0.0f, -5.0f, 1.0f)),  // 默认位置
        target(Vec4(0.0f, 0.0f, 0.0f, 1.0f)),      // 默认目标
        upVector(Vec4(0.0f, 1.0f, 0.0f, 1.0f)),    // 默认上方向
        fov(45.0f), aspect(4.0f / 3.0f), nearPlane(0.1f), farPlane(100.0f)
    {
        setProjectionMatrix(fov, aspect, nearPlane, farPlane); // 设置投影矩阵
        setViewMatrix(position, target, upVector);           // 设置视图矩阵
    }

    // 设置相机的位置
    void setPosition(const Vec4& newPosition)
    {
        position = newPosition;
        setViewMatrix(position, target, upVector); // 更新视图矩阵
    }

    // 设置相机的目标（方向）
    void setTarget(const Vec4& newTarget)
    {
        target = newTarget;
        setViewMatrix(position, target, upVector); // 更新视图矩阵
    }

    // 更新视图矩阵
    void updateViewMatrix()
    {
        setViewMatrix(position, target, upVector);
    }

    // 移动函数
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

    void moveUp(float amount)
    {
        position = position + upVector * amount;
        target = target + upVector * amount;
        updateViewMatrix();
    }

    void moveDown(float amount)
    {
        moveUp(-amount);
    }

    // 旋转函数
    void rotate(float angleX, float angleY)
    {
        // 绕 Y 轴旋转（左右）
        float radY = angleY * (3.14159f / 180.0f);
        Vec4 direction = position - target;

        float sinY = sin(radY);
        float cosY = cos(radY);
        float newX = direction.x * cosY - direction.z * sinY;
        float newZ = direction.x * sinY + direction.z * cosY;
        direction.x = newX;
        direction.z = newZ;

        // 绕 X 轴旋转（上下）
        float radX = angleX * (3.14159f / 180.0f);
        float sinX = sin(radX);
        float cosX = cos(radX);
        float newY = direction.y * cosX - direction.z * sinX;
        direction.y = newY;

        target = position + direction; // 更新目标位置
        updateViewMatrix();
    }

    // 设置投影矩阵
    void setProjection(float newFov, float newAspect, float newNear, float newFar)
    {
        fov = newFov;
        aspect = newAspect;
        nearPlane = newNear;
        farPlane = newFar;
        setProjectionMatrix(fov, aspect, nearPlane, farPlane);
    }

    // 获取视图矩阵
    const Matrix44& getViewMatrix() const
    {
        return viewMatrix;
    }

    // 获取投影矩阵
    const Matrix44& getProjectionMatrix() const
    {
        return projectionMatrix;
    }

    void setViewMatrix(const Vec4& eye, const Vec4& center, const Vec4& up)
    {
        viewMatrix = Matrix44::worldtoCam(eye, center, up);
    }

    // 设置投影矩阵的具体实现
    void setProjectionMatrix(float fov, float aspect, float nearP, float farP)
    {
        projectionMatrix = Matrix44::camtoScreen(fov, aspect, nearP, farP);
    }
};
