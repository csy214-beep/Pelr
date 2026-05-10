#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Math/CubismMatrix44.hpp>
#include <Math/CubismViewMatrix.hpp>
#include "CubismFramework.hpp"
#include <Rendering/OpenGL/CubismRenderTarget_OpenGLES2.hpp>
#include "LAppView_Common.hpp"

class TouchManager_Common;
class LAppSprite;
class LAppModel;
class LAppSpriteShader;

class LAppView : public LAppView_Common
{
public:
    enum SelectTarget
    {
        SelectTarget_None,
        SelectTarget_ModelFrameBuffer,
        SelectTarget_ViewFrameBuffer,
    };

    LAppView();
    ~LAppView();

    virtual void Initialize(int width, int height) override;
    void Render();
    void InitializeSprite();
    void ResizeSprite();

    void OnTouchesBegan(float pointX, float pointY) const;
    void OnTouchesMoved(float pointX, float pointY) const;
    void OnTouchesEnded(float pointX, float pointY) const;

    void PreModelDraw(LAppModel &refModel);
    void PostModelDraw(LAppModel &refModel);

    float GetSpriteAlpha(int assign) const;
    void SwitchRenderingTarget(SelectTarget targetType);
    void SetRenderTargetClearColor(float r, float g, float b);

    void DestroySpriteRenderTarget(); // 自定义方法

private:
    TouchManager_Common *_touchManager;
    LAppSprite *_back;
    LAppSprite *_gear;
    LAppSprite *_power;
    LAppSprite *_renderSprite;
    Csm::Rendering::CubismRenderTarget_OpenGLES2 _renderBuffer;
    SelectTarget _renderTarget;
    float _clearColor[4];
    LAppSpriteShader *_spriteShader;
};