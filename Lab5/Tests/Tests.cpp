#define CATCH_CONFIG_MAIN

#include "../../../catch2/catch.hpp"

#include "../CommandPattern/Invoker.h"
#include "../CommandPattern/History.h"
#include "../CommandPattern/CommandFactory.h"
#include "../CommandPattern/Mergeable.h"
#include "../CommandPattern/Unexecutable.h"
#include "../CommandPattern/DocumentItems.h"
#include "../CommandPattern/Document.h"
#include "../CommandPattern/Command.h"

#include <fstream>
#include <filesystem>
#include <sstream>
#include <iostream>

namespace fs = std::filesystem;

void CreateTestImage(const std::string& filename) 
{
    std::ofstream file(filename);
    file << "fake image content";
    file.close();
}

std::string ReadFileContent(const fs::path& path) 
{
    std::ifstream file(path);
    return std::string(std::istreambuf_iterator<char>(file), {});
}

class TestHistoryManager : public IHistoryManager 
{
public:
    void Undo() override { undoCalled = true; }
    void Redo() override { redoCalled = true; }
    bool CanUndo() const override { return canUndoValue; }
    bool CanRedo() const override { return canRedoValue; }

    bool undoCalled = false;
    bool redoCalled = false;
    bool canUndoValue = true;
    bool canRedoValue = true;
};

// command.h 

TEST_CASE("Undo/Redo commands call document methods", "[commands]") 
{
    TestHistoryManager testManager;
    auto doc = std::make_shared<HtmlDocument>(testManager);

    SECTION("Undo command") 
    {
        testManager.canUndoValue = true;
        UndoCommand undoCommand(doc);
        REQUIRE_NOTHROW(undoCommand.Execute());
        REQUIRE(testManager.undoCalled);
    }

    SECTION("Redo command") 
    {
        testManager.canRedoValue = true;
        RedoCommand redoCommand(doc);
        REQUIRE_NOTHROW(redoCommand.Execute());
        REQUIRE(testManager.redoCalled);
    }
}

TEST_CASE("Undo command throws when cannot undo", "[commands]") 
{
    TestHistoryManager testManager;
    testManager.canUndoValue = false;
    auto doc = std::make_shared<HtmlDocument>(testManager);

    UndoCommand undoCommand(doc);
    REQUIRE_THROWS_AS(undoCommand.Execute(), std::runtime_error);
}

TEST_CASE("Redo command throws when cannot redo", "[commands]") 
{
    TestHistoryManager testManager;
    testManager.canRedoValue = false;
    auto doc = std::make_shared<HtmlDocument>(testManager);

    RedoCommand redoCommand(doc);
    REQUIRE_THROWS_AS(redoCommand.Execute(), std::runtime_error);
}                               

TEST_CASE("List command outputs all item types correctly", "[commands]") 
{
    TestHistoryManager testManager;
    auto doc = std::make_shared<HtmlDocument>(testManager);

    std::stringstream coutBuffer;
    auto oldBuf = std::cout.rdbuf(coutBuffer.rdbuf());

    doc->SetTitle("Test Document");
    doc->InsertParagraph("First paragraph");
    doc->InsertParagraph("Second paragraph");

    auto& item1 = doc->GetItem(0);
    item1.SetIsDeleted(true);

    ListCommand listCommand(doc);
    listCommand.Execute();

    std::string output = coutBuffer.str();
    std::cout.rdbuf(oldBuf);

    REQUIRE(output.find("Title: Test Document") != std::string::npos);
    REQUIRE(output.find("1. Paragraph: First paragraph [DELETED]") != std::string::npos);
    REQUIRE(output.find("[DELETED]") != std::string::npos);
}

TEST_CASE("Help command outputs help text", "[commands]") 
{
    std::stringstream coutBuffer;
    auto oldBuf = std::cout.rdbuf(coutBuffer.rdbuf());

    HelpCommand helpCommand;
    helpCommand.Execute();

    std::string output = coutBuffer.str();
    std::cout.rdbuf(oldBuf);

    REQUIRE(output.find("Available commands:") != std::string::npos);
    REQUIRE(output.find("InsertParagraph") != std::string::npos);
    REQUIRE(output.find("InsertImage") != std::string::npos);
    REQUIRE(output.find("SetTitle") != std::string::npos);
    REQUIRE(output.find("List") != std::string::npos);
    REQUIRE(output.find("Undo") != std::string::npos);
    REQUIRE(output.find("Redo") != std::string::npos);
    REQUIRE(output.find("Save") != std::string::npos);
}

TEST_CASE("Save command calls document save", "[commands]") 
{
    TestHistoryManager testManager;
    auto doc = std::make_shared<HtmlDocument>(testManager);

    SaveCommand saveCommand(doc, "test.html");
    REQUIRE_NOTHROW(saveCommand.Execute());

    if (fs::exists("test.html")) {
        fs::remove("test.html");
    }
}

// mergeable.h

TEST_CASE("Replace text command execute and unexecute multiple times", "[mergable]") 
{
    TestHistoryManager testManager;
    auto doc = std::make_shared<HtmlDocument>(testManager);
    doc->InsertParagraph("Initial text", 0);

    ReplaceTextCommand command(doc, 0, "First change");

    command.Execute();
    REQUIRE(doc->GetItem(0).GetParagraph()->GetText() == "First change");
    REQUIRE(command.IsExecuted());

    command.Unexecute();
    REQUIRE(doc->GetItem(0).GetParagraph()->GetText() == "Initial text");

    command.Execute();
    REQUIRE(doc->GetItem(0).GetParagraph()->GetText() == "First change");

    command.Unexecute();
    REQUIRE(doc->GetItem(0).GetParagraph()->GetText() == "Initial text");
}

TEST_CASE("Replace text commands merge and unexecute together", "[mergable]") 
{
    TestHistoryManager testManager;
    auto doc = std::make_shared<HtmlDocument>(testManager);
    doc->InsertParagraph("Initial text", 0);

    auto command1 = std::make_shared<ReplaceTextCommand>(doc, 0, "First change");
    auto command2 = std::make_shared<ReplaceTextCommand>(doc, 0, "Second change");

    command1->Execute();
    bool merged = command1->TryReplace(command2);

    REQUIRE(merged);
    REQUIRE(doc->GetItem(0).GetParagraph()->GetText() == "Second change");

    command1->Unexecute();
    REQUIRE(doc->GetItem(0).GetParagraph()->GetText() == "Initial text");
}

TEST_CASE("Multiple replace text commands with different paragraphs", "[mergable]") 
{
    TestHistoryManager testManager;
    auto doc = std::make_shared<HtmlDocument>(testManager);
    doc->InsertParagraph("First initial", 0);
    doc->InsertParagraph("Second initial", 1);

    auto command1 = std::make_shared<ReplaceTextCommand>(doc, 0, "First changed");
    auto command2 = std::make_shared<ReplaceTextCommand>(doc, 0, "First changed again");
    auto command3 = std::make_shared<ReplaceTextCommand>(doc, 1, "Second changed");
    auto command4 = std::make_shared<ReplaceTextCommand>(doc, 0, "First final");

    command1->Execute();
    command1->TryReplace(command2);
    command3->Execute();
    command1->TryReplace(command4);

    REQUIRE(doc->GetItem(0).GetParagraph()->GetText() == "First final");
    REQUIRE(doc->GetItem(1).GetParagraph()->GetText() == "Second changed");

    command1->Unexecute();
    REQUIRE(doc->GetItem(0).GetParagraph()->GetText() == "First initial");
    REQUIRE(doc->GetItem(1).GetParagraph()->GetText() == "Second changed");

    command3->Unexecute();
    REQUIRE(doc->GetItem(0).GetParagraph()->GetText() == "First initial");
    REQUIRE(doc->GetItem(1).GetParagraph()->GetText() == "Second initial");
}

class TestMergableCommand : public MergableCommand 
{
public:
    TestMergableCommand(std::weak_ptr<IDocument> document) : MergableCommand(document) {}
    void Execute() override { m_executed = true; }
    void Unexecute() override { m_executed = false; }
    bool TryReplace(std::shared_ptr<MergableCommand> command) override { return false; }
};

TEST_CASE("Different mergable commands do not merge", "[mergable]") 
{
    TestHistoryManager testManager;
    auto doc = std::make_shared<HtmlDocument>(testManager);
    doc->InsertParagraph("Initial text", 0);

    auto command1 = std::make_shared<ReplaceTextCommand>(doc, 0, "First change");
    auto command2 = std::make_shared<TestMergableCommand>(doc);

    command1->Execute();
    bool merged = command1->TryReplace(command2);

    REQUIRE_FALSE(merged);
    REQUIRE(doc->GetItem(0).GetParagraph()->GetText() == "First change");

    command1->Unexecute();
    REQUIRE(doc->GetItem(0).GetParagraph()->GetText() == "Initial text");
}

TEST_CASE("Replace text command throws when position is image", "[mergable]") 
{
    TestHistoryManager testManager;
    auto doc = std::make_shared<HtmlDocument>(testManager);

    fs::remove_all("tmp");
    CreateTestImage("test_image.png");
    doc->InsertImage("test_image.png", 100, 200, 0);

    ReplaceTextCommand command(doc, 0, "New text");

    REQUIRE_THROWS_AS(command.Execute(), std::runtime_error);
    REQUIRE_FALSE(command.IsExecuted());

    fs::remove_all("tmp");
    fs::remove("test_image.png");
}

// unexecutable.h

TEST_CASE("Set title command execute and unexecute", "[unexecutable]") 
{
    fs::remove_all("tmp");

    TestHistoryManager testManager;
    auto doc = std::make_shared<HtmlDocument>(testManager);
    doc->SetTitle("Initial Title");

    SetTitleCommand command(doc, "New Title");

    command.Execute();
    REQUIRE(doc->GetTitle() == "New Title");
    REQUIRE(command.IsExecuted());

    command.Unexecute();
    REQUIRE(doc->GetTitle() == "Initial Title");

    command.Execute();
    REQUIRE(doc->GetTitle() == "New Title");

    command.Unexecute();
    REQUIRE(doc->GetTitle() == "Initial Title");
}

TEST_CASE("Resize image command execute and unexecute", "[unexecutable]") 
{
    fs::remove_all("tmp");

    TestHistoryManager testManager;
    auto doc = std::make_shared<HtmlDocument>(testManager);
    CreateTestImage("test_image.png");
    doc->InsertImage("test_image.png", 100, 200, 0);

    ResizeImageCommand command(doc, 0, 300, 400);

    command.Execute();
    REQUIRE(doc->GetItem(0).GetImage()->GetWidth() == 300);
    REQUIRE(doc->GetItem(0).GetImage()->GetHeight() == 400);
    REQUIRE(command.IsExecuted());

    command.Unexecute();
    REQUIRE(doc->GetItem(0).GetImage()->GetWidth() == 100);
    REQUIRE(doc->GetItem(0).GetImage()->GetHeight() == 200);

    command.Execute();
    REQUIRE(doc->GetItem(0).GetImage()->GetWidth() == 300);
    REQUIRE(doc->GetItem(0).GetImage()->GetHeight() == 400);

    fs::remove_all("tmp");
    fs::remove("test_image.png");
}

TEST_CASE("Insert paragraph command execute and unexecute", "[unexecutable]") 
{
    fs::remove_all("tmp");

    TestHistoryManager testManager;
    auto doc = std::make_shared<HtmlDocument>(testManager);

    InsertParagraphCommand command(doc, std::nullopt, "Test Paragraph");

    command.Execute();
    REQUIRE(doc->GetItemsCount() == 1);
    REQUIRE(doc->GetItem(0).GetParagraph()->GetText() == "Test Paragraph");
    REQUIRE_FALSE(doc->GetItem(0).IsDeleted());
    REQUIRE(command.IsExecuted());

    command.Unexecute();
    REQUIRE(doc->GetItemsCount() == 1);
    REQUIRE(doc->GetItem(0).IsDeleted());

    command.Execute();
    REQUIRE(doc->GetItemsCount() == 1);
    REQUIRE_FALSE(doc->GetItem(0).IsDeleted());

    command.Unexecute();
    REQUIRE(doc->GetItem(0).IsDeleted());
}

TEST_CASE("Insert image command execute and unexecute", "[unexecutable]") 
{
    fs::remove_all("tmp");

    TestHistoryManager testManager;
    auto doc = std::make_shared<HtmlDocument>(testManager);
    CreateTestImage("test_image.png");

    InsertImageCommand command(doc, std::nullopt, "test_image.png", 100, 200);

    command.Execute();
    REQUIRE(doc->GetItemsCount() == 1);
    REQUIRE(doc->GetItem(0).GetImage()->GetWidth() == 100);
    REQUIRE(doc->GetItem(0).GetImage()->GetHeight() == 200);
    REQUIRE_FALSE(doc->GetItem(0).IsDeleted());
    REQUIRE(command.IsExecuted());

    command.Unexecute();
    REQUIRE(doc->GetItemsCount() == 1);
    REQUIRE(doc->GetItem(0).IsDeleted());

    command.Execute();
    REQUIRE(doc->GetItemsCount() == 1);
    REQUIRE_FALSE(doc->GetItem(0).IsDeleted());

    fs::remove_all("tmp");
    fs::remove("test_image.png");
}

TEST_CASE("Delete item command execute and unexecute", "[unexecutable]") 
{
    fs::remove_all("tmp");

    TestHistoryManager testManager;
    auto doc = std::make_shared<HtmlDocument>(testManager);
    doc->InsertParagraph("Test Paragraph", 0);

    DeleteItemCommand command(doc, 0);

    command.Execute();
    REQUIRE(doc->GetItem(0).IsDeleted());
    REQUIRE(command.IsExecuted());

    command.Unexecute();
    REQUIRE_FALSE(doc->GetItem(0).IsDeleted());

    command.Execute();
    REQUIRE(doc->GetItem(0).IsDeleted());
}

TEST_CASE("Unexecute without execute does nothing", "[unexecutable]") 
{
    TestHistoryManager testManager;
    auto doc = std::make_shared<HtmlDocument>(testManager);
    doc->SetTitle("Initial Title");

    SetTitleCommand command(doc, "New Title");

    REQUIRE_NOTHROW(command.Unexecute());
    REQUIRE(doc->GetTitle() == "Initial Title");
    REQUIRE_FALSE(command.IsExecuted());
}

TEST_CASE("Insert paragraph command destructor deletes item", "[unexecutable]") 
{
    TestHistoryManager testManager;
    auto doc = std::make_shared<HtmlDocument>(testManager);

    {
        auto command = std::make_unique<InsertParagraphCommand>(doc, std::nullopt, "Test Paragraph");
        command->Execute();
        command->Unexecute();
        REQUIRE(doc->GetItemsCount() == 1);
        REQUIRE(doc->GetItem(0).IsDeleted());
    }

    REQUIRE(doc->GetItemsCount() == 0);
}

TEST_CASE("Insert image command destructor deletes item", "[unexecutable]") 
{
    fs::remove_all("tmp");

    TestHistoryManager testManager;
    auto doc = std::make_shared<HtmlDocument>(testManager);
    CreateTestImage("test_image.png");

    {
        auto command = std::make_unique<InsertImageCommand>(doc, std::nullopt, "test_image.png", 100, 200);
        command->Execute();
        command->Unexecute();
        REQUIRE(doc->GetItemsCount() == 1);
        REQUIRE(doc->GetItem(0).IsDeleted());
    }

    REQUIRE(doc->GetItemsCount() == 0);

    fs::remove_all("tmp");
    fs::remove("test_image.png");
}

TEST_CASE("Delete item command destructor deletes item", "[unexecutable]") 
{
    TestHistoryManager testManager;
    auto doc = std::make_shared<HtmlDocument>(testManager);

    {
        doc->InsertParagraph("Test Paragraph", 0);
        auto command = std::make_unique<DeleteItemCommand>(doc, 0);
        command->Execute();
        REQUIRE(doc->GetItem(0).IsDeleted());
    }

    REQUIRE(doc->GetItemsCount() == 0);
}

TEST_CASE("Multiple commands execute and unexecute in reverse order", "[unexecutable]") 
{
    fs::remove_all("tmp");

    TestHistoryManager testManager;
    auto doc = std::make_shared<HtmlDocument>(testManager);
    CreateTestImage("test_image.png");

    doc->SetTitle("Initial Title");
    doc->InsertParagraph("Initial Paragraph", 0);

    std::string initialTitle = doc->GetTitle();
    std::string initialParagraph = doc->GetItem(0).GetParagraph()->GetText();
    size_t initialCount = doc->GetItemsCount();

    {
        auto command1 = std::make_unique<SetTitleCommand>(doc, "New Title 1");
        auto command2 = std::make_unique<InsertParagraphCommand>(doc, 0, "New Paragraph");
        auto command3 = std::make_unique<SetTitleCommand>(doc, "New Title 2");
        auto command4 = std::make_unique<InsertImageCommand>(doc, std::nullopt, "test_image.png", 100, 200);
        auto command5 = std::make_unique<DeleteItemCommand>(doc, 1);

        command1->Execute();
        command2->Execute();
        command3->Execute();
        command4->Execute();
        command5->Execute();

        command5->Unexecute();
        command4->Unexecute();
        command3->Unexecute();
        command2->Unexecute();
        command1->Unexecute();
    }

    REQUIRE(doc->GetTitle() == initialTitle);
    REQUIRE(doc->GetItemsCount() == initialCount);
    REQUIRE(doc->GetItem(0).GetParagraph()->GetText() == initialParagraph);
    REQUIRE_FALSE(doc->GetItem(0).IsDeleted());

    fs::remove_all("tmp");
    fs::remove("test_image.png");
}

// DocumentItems.h

TEST_CASE("Paragraph text operations", "[items]") 
{
    auto paragraph = std::make_shared<Paragraph>("Test text");
    REQUIRE(paragraph->GetText() == "Test text");

    paragraph->SetText("New text");
    REQUIRE(paragraph->GetText() == "New text");
}

TEST_CASE("Image operations", "[items]") 
{
    auto image = std::make_shared<Image>("path.png", 100, 200);
    REQUIRE(image->GetWidth() == 100);
    REQUIRE(image->GetHeight() == 200);

    image->Resize(300, 400);
    REQUIRE(image->GetWidth() == 300);
    REQUIRE(image->GetHeight() == 400);
}

TEST_CASE("Const document item with paragraph", "[items]") 
{
    auto paragraph = std::make_shared<Paragraph>("Const paragraph");
    ConstDocumentItem item(nullptr, paragraph, true);

    REQUIRE(item.GetParagraph()->GetText() == "Const paragraph");
    REQUIRE(item.IsDeleted());
    REQUIRE(item.GetImage() == nullptr);
}

TEST_CASE("Const document item with image", "[items]") 
{
    auto image = std::make_shared<Image>("image.png", 150, 250);
    ConstDocumentItem item(image, nullptr, false);

    REQUIRE(item.GetImage()->GetWidth() == 150);
    REQUIRE(item.GetImage()->GetHeight() == 250);
    REQUIRE_FALSE(item.IsDeleted());
    REQUIRE(item.GetParagraph() == nullptr);
}

TEST_CASE("Document item with paragraph modification", "[items]") 
{
    auto paragraph = std::make_shared<Paragraph>("Original");
    DocumentItem item(paragraph);

    item.GetParagraph()->SetText("Modified");
    REQUIRE(item.GetParagraph()->GetText() == "Modified");
    REQUIRE_FALSE(item.IsDeleted());

    item.SetIsDeleted(true);
    REQUIRE(item.IsDeleted());
}

TEST_CASE("Document item with image modification", "[items]") 
{
    auto image = std::make_shared<Image>("img.png", 100, 100);
    DocumentItem item(image);

    item.GetImage()->Resize(200, 300);
    REQUIRE(item.GetImage()->GetWidth() == 200);
    REQUIRE(item.GetImage()->GetHeight() == 300);
    REQUIRE_FALSE(item.IsDeleted());

    item.SetIsDeleted(true);
    REQUIRE(item.IsDeleted());
}

