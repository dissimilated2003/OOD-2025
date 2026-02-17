#pragma once

#include "IDocument.h"
#include "DocumentItems.h"

#include <iostream>
#include <stdexcept>
#include <memory>

struct ICommand
{
	virtual void Execute() = 0;

	virtual ~ICommand() = default;
};

class AbstractCommand : public ICommand
{
protected:
	AbstractCommand(std::weak_ptr<IDocument> doc) : m_document(doc) {}

	IDocument& TryGetDocument() const
	{
		auto document = m_document.lock();
		if (!document)
		{
			throw std::runtime_error("Document no longer exists");
		}
		return *document;
	}

	std::weak_ptr<IDocument> GetDocument() const
	{
		return m_document;
	}

private:
	std::weak_ptr<IDocument> m_document;
};

class HelpCommand : public ICommand
{
public:
	HelpCommand() = default;

	void Execute() override
	{
		std::cout << "Available commands:\n"
			<< "InsertParagraph <position>|end <text>\n"
			<< "InsertImage <position>|end <width> <height> <path>\n"
			<< "SetTitle <title>\n"
			<< "List\n"
			<< "ReplaceText <position> <text>\n"
			<< "ResizeImage <position> <width> <height>\n"
			<< "DeleteItem <position>\n"
			<< "Help\n"
			<< "Undo\n"
			<< "Redo\n"
			<< "Save <path>\n";
	}
};

class UndoCommand : public AbstractCommand
{
public:
	UndoCommand(std::weak_ptr<IDocument> document)
		: AbstractCommand(document)
	{}

	void Execute() override
	{
		auto& document = TryGetDocument();
		if (!document.CanUndo())
		{
			throw std::runtime_error("Cannot undo");
		}
		document.Undo();
	}
};

class RedoCommand : public AbstractCommand
{
public:
	RedoCommand(std::weak_ptr<IDocument> document) : AbstractCommand(document) {}

	void Execute() override
	{
		auto& document = TryGetDocument();
		if (!document.CanRedo())
		{
			throw std::runtime_error("Cannot redo");
		}
		document.Redo();
	}
};

class SaveCommand : public AbstractCommand
{
public:
	SaveCommand(std::weak_ptr<IDocument> document, const std::string& path)
		: AbstractCommand(document)
		, m_path(path)
	{}

	void Execute() override
	{
		auto& document = TryGetDocument();
		document.Save(m_path);
	}

private:
	std::string m_path;
};

class ListCommand : public AbstractCommand
{
public:
	ListCommand(std::weak_ptr<IDocument> document) : AbstractCommand(document) {}

	void Execute() override
	{
		auto& document = TryGetDocument();
		std::cout << "Title: " << document.GetTitle() << std::endl;

		for (size_t k = 0; k < document.GetItemsCount(); ++k)
		{
			auto item = document.GetItem(k);
			std::cout << (k + 1) << ". ";

			if (auto paragraph = item.GetParagraph())
			{
				std::cout << "Paragraph: " << paragraph->GetText();
			}
			else if (auto image = item.GetImage())
			{
				std::cout << "Image: " << image->GetWidth() << " "
					<< image->GetHeight() << " " << image->GetPath();
			}

			if (item.IsDeleted())
			{
				std::cout << " [DELETED]";
			}

			std::cout << '\n';
		}
	}
};