#pragma once

#include "Change.h"

class NewModelFormatChange : public Change
{
public:
    std::uint8_t id;
    std::string model;
    std::string format;

    NewModelFormatChange(std::uint8_t id, const std::string &model, const std::string &format)
        : id(id), model(model), format(format)
    {}

    static NewModelFormatChange Read(std::istream &stream);
    virtual void WriteInternal() override;
    virtual std::uint32_t NewLength() override;
};