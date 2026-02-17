#pragma once

#include "IShape.h"
#include "ShapeFactory.h"

struct IDesigner
{
	virtual PictureDraft CreateDraft(std::istream& is) = 0;

	virtual ~IDesigner() = default;
};

class Designer final : public IDesigner
{
public:
	Designer(std::unique_ptr<IShapeFactory> shapeFactory) 
		: m_shapeFactory(std::move(shapeFactory)) 
	{}

	PictureDraft CreateDraft(std::istream& is) override
	{
		std::vector<std::unique_ptr<IShape>> shapes{};
		std::string line{};

		while (std::getline(is, line))
		{
			if (!line.empty())
			{
				try
				{
					auto shape = m_shapeFactory->CreateShape(line);
					shapes.push_back(std::move(shape));
				}
				catch (const std::exception& ex)
				{
					throw std::runtime_error("Failed to create shape" + std::string(ex.what()));
				}
			}
		}
		return PictureDraft(std::move(shapes));
	}

private:
	std::unique_ptr<IShapeFactory> m_shapeFactory{};
};