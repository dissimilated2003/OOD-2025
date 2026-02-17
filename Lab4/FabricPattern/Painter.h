#pragma once

#include "IShape.h"

class Painter
{
public:
	static void DrawPicture(PictureDraft& draft, ICanvas& canvas)
	{
		for (size_t k = 0; k < draft.GetShapesCount(); ++k)
		{
			IShape& shape = draft.GetShape(k);
			shape.Draw(canvas);
		}
		canvas.Render();
	}
};