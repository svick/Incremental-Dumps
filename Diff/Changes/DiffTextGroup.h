#pragma once

#include "Change.h"

class DiffTextGroup : public Change
{
public:
    const std::string compressedTexts;

    DiffTextGroup(const std::string &compressedTexts);

    static DiffTextGroup Read(std::istream &stream);
    virtual std::uint32_t NewLength() override;
    virtual void WriteInternal() override;
};