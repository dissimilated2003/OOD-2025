#pragma once

#include "CommonTypes.h"

// интерфейс "Холст", на котором будут рисоваться фигуры
struct ICanvas
{
	virtual void SetLineColor(RGBAColor color) = 0;
	virtual void BeginFill(RGBAColor color) = 0;
	virtual void EndFill() = 0;
	virtual void MoveTo(float x, float y) = 0;
	virtual void LineTo(float x, float y) = 0;
	virtual void DrawEllipse(Frame frame) = 0;
	virtual void SetStrokeDepth(float depth) = 0;
	
	virtual ~ICanvas() = default;
};

