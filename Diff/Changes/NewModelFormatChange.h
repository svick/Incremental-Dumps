#pragma once

#include "Change.h"

/**
 * A change that represents adding new combination of content model and format to the index.
 */
class NewModelFormatChange : public Change
{
public:
    /**
     * The ID in the index.
     */
    std::uint8_t id;
    /**
     * The content model.
     */
    std::string model;
    /**
     * The content format.
     */
    std::string format;

    NewModelFormatChange(std::uint8_t id, const std::string &model, const std::string &format)
        : id(id), model(model), format(format)
    {}

    static NewModelFormatChange Read(std::istream &stream);
    virtual void WriteInternal() OVERRIDE;
    virtual std::uint32_t NewLength() OVERRIDE;
};