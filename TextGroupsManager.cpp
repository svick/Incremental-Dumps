#include "TextGroupsManager.h"
#include "Dump.h"
#include "Indexes/Index.h"

TextGroupsManager::TextGroupsManager(std::weak_ptr<Dump> dump)
    : dump(dump), textGroupModified(false)
{ }

std::unique_ptr<TextGroup> TextGroupsManager::CreateNewGroup()
{
    std::uint32_t newId = getNewId(*dump.lock()->textGroupIdIndex);
    return std::unique_ptr<TextGroup>(new TextGroup(dump, newId));
}

std::pair<std::uint32_t, std::uint8_t> TextGroupsManager::AddTextToGroup(const std::string& text)
{
    auto dumpRef = dump.lock();

    if (currentGroup == nullptr)
    {
        auto streamPos = dumpRef->stream->tellg();

        currentGroup = CreateNewGroup();
        textGroupModified = false;

        // this method can be written in the middle of writing an object,
        // so it can't change the stream position
        dumpRef->stream->seekg(streamPos);
    }

    auto textId = currentGroup->AddText(text);
    auto textGroupId = currentGroup->GetTextGroupId();
    textGroupModified = true;

    return std::make_pair(textGroupId, textId);
}

void TextGroupsManager::EndGroup(DiffWriter* diffWriter)
{
    if (currentGroup == nullptr)
        return;

    if (textGroupModified)
    {
        currentGroup->Write();

        if (diffWriter != nullptr)
            diffWriter->SetTextGroup(currentGroup->GetCompressedTexts());
    }

    currentGroup = nullptr;
    textGroupModified = false;
}

void TextGroupsManager::WriteTextGroupIfFull(DiffWriter* diffWriter)
{
    if (currentGroup != nullptr && currentGroup->IsFull())
        EndGroup(diffWriter);
}

void TextGroupsManager::DeleteTextFromGroup(std::uint32_t textGroupId, std::uint8_t textId)
{
    if (currentDeletedGroup == nullptr || currentDeletedGroup->GetTextGroupId() != textGroupId)
    {
        EndDeletedGroup();

        currentDeletedGroup = std::unique_ptr<TextGroup>(new TextGroup(dump, textGroupId));
    }

    if (currentDeletedGroup->DeleteText(textId))
    {
        currentDeletedGroup = nullptr;
    }
}

void TextGroupsManager::EndDeletedGroup()
{
    if (currentDeletedGroup == nullptr)
        return;

    currentDeletedGroup->Write();
    currentDeletedGroup = nullptr;
}

std::string TextGroupsManager::GetTextFromGroup(std::uint32_t textGroupId, std::uint8_t textId)
{
    if (currentGroup == nullptr || currentGroup->GetTextGroupId() != textGroupId)
    {
        EndGroup(nullptr);

        currentGroup = std::unique_ptr<TextGroup>(new TextGroup(dump, textGroupId));
        textGroupModified = false;
    }

    return currentGroup->GetText(textId);
}

std::uint32_t TextGroupsManager::ImportTextGroup(const std::string& compressedTexts)
{
    auto group = CreateNewGroup();
    group->SetCompressedTexts(compressedTexts);
    group->Write();

    return group->GetTextGroupId();
}

void TextGroupsManager::Complete(DiffWriter* diffWriter)
{
    EndGroup(diffWriter);
    EndDeletedGroup();
}