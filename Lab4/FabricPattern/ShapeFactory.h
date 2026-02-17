#pragma once

#include "IShape.h"
#include "Color.h"
#include "Point.h"

#include <memory>
#include <sstream>

enum class ShapeType
{
	TRIANGLE,
	RECTANGLE,
	ELLIPSE,
	REGULAR_POLYGON,
	TEXT,
	UNKNOWN
};

struct IShapeFactory
{
	virtual std::unique_ptr<IShape> CreateShape(const std::string& desc) = 0;

	virtual ~IShapeFactory() = default;
};

class ShapeFactory final : public IShapeFactory
{
public:
	std::unique_ptr<IShape> CreateShape(const std::string& desc) override
	{
		std::istringstream isus{ desc };
		std::string shapeType{};
		isus >> shapeType;

		ShapeType type = ExtractShapeType(shapeType);
		switch (type)
		{
		case ShapeType::TRIANGLE:
			return CreateTriange(desc);
		case ShapeType::RECTANGLE:
			return CreateRectangle(desc);
		case ShapeType::ELLIPSE:
			return CreateEllipse(desc);
		case ShapeType::REGULAR_POLYGON:
			return CreateRegularPolygon(desc);
		case ShapeType::TEXT:
			return CreateText(desc);
		default:
			throw std::invalid_argument("Unknown shape type");
		}
	}


private:
	ShapeType ExtractShapeType(const std::string& type)
	{
		if (type == "triangle") return ShapeType::TRIANGLE;
		if (type == "rectangle") return ShapeType::RECTANGLE;
		if (type == "ellipse") return ShapeType::ELLIPSE;
		if (type == "polygon") return ShapeType::REGULAR_POLYGON;
		if (type == "text") return ShapeType::TEXT;
		return ShapeType::UNKNOWN;
	}

	// triangle xVert1 yVert1 xVert2 yVert2 xVert3 yVert3 #colorRGBA
	// triangle 200 100 100 200 300 200 #BBAAFFFF
	std::unique_ptr<IShape> CreateTriange(const std::string desc)
	{
		std::istringstream isus{ desc };
		std::string type{};
		std::string colorStr{};
		Point v1{}, v2{}, v3{};

		isus >> type >> v1.x >> v1.y >> v2.x >> v2.y >> v3.x >> v3.y >> colorStr;
		if (isus.fail())
		{
			throw std::invalid_argument("Cannot parse triangle query");
		}

		return std::make_unique<CTriangle>(v1, v2, v3, ColorRGBA{ colorStr });
	}

	// rectangle xLeftTop yLeftTop xRightBottom yRightBottom #colorRGBA
	// rectangle 100 100 300 200 #FFBBAAFF
	std::unique_ptr<IShape> CreateRectangle(const std::string desc)
	{
		std::istringstream isus{ desc };
		std::string type{};
		std::string colorStr{};
		Point leftTop{}, rightBottom{};

		isus >> type >> leftTop.x >> leftTop.y >> rightBottom.x >> rightBottom.y >> colorStr;
		if (isus.fail())
		{
			throw std::invalid_argument("Cannot parse rectangle query");
		}

		return std::make_unique<CRectangle>(leftTop, rightBottom, ColorRGBA{ colorStr });
	}

	// ellipse xCenter yCenter xRadius yRadius #colorRGBA
	// ellipse 400 100 30 50 #FEEDBEFF 
	// ellipse 300 100 50 50 #FFFF11FF
	std::unique_ptr<IShape> CreateEllipse(const std::string desc)
	{
		std::istringstream isus{ desc };
		std::string type{};
		std::string colorStr{};
		Point center{};
		float radX{}, radY{};

		isus >> type >> center.x >> center.y >> radX >> radY >> colorStr;
		if (isus.fail())
		{
			throw std::invalid_argument("Cannot parse ellipse query");
		}

		return std::make_unique<CEllipse>(center, radX, radY, ColorRGBA{ colorStr });
	}

	// polygon xCenter yCenter ownCirlceRadius vertices #colorRGBA
	// polygon 400 300 100 8 #FF0000FF
	std::unique_ptr<IShape> CreateRegularPolygon(const std::string desc)
	{
		std::istringstream isus{ desc };
		std::string type{};
		std::string colorStr{};
		Point center{};
		float radius{};
		unsigned verticesCount{};

		isus >> type >> center.y >> center.x >> radius >> verticesCount >> colorStr;
		if (isus.fail())
		{
			throw std::invalid_argument("Cannot parse polygon query");
		}

		return std::make_unique<CRegularPolygon>(center, radius, verticesCount, ColorRGBA{ colorStr });
	}

	// text xLeftTop yLeftTop fontSize "message" #colorRGBA
	// text 100 100 16 "the house looks like complete dog shit" #FFFF00FF
	std::unique_ptr<IShape> CreateText(const std::string desc)
	{
		std::istringstream isus{ desc };
		std::string type{};
		std::string colorStr{};
		Point leftTop{};
		unsigned fontSize{};
		std::string message{};

		isus >> type >> leftTop.y >> leftTop.x >> fontSize;
		if (isus.fail())
		{
			throw std::invalid_argument("Cannot parse text query");
		}

		char scan{};
		while (isus.get(scan) && scan != '"') {};
		std::getline(isus, message, '"');
		isus >> colorStr;
		if (isus.fail())
		{
			throw std::invalid_argument("Cannot parse text query");
		}

		return std::make_unique<CTextMessage>(leftTop, fontSize, message, ColorRGBA{colorStr});
	}
};