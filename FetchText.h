#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include "libexecstream/exec-stream.h"

class FetchText
{
private:
    std::unique_ptr<exec_stream_t> stream;
public:
    FetchText(std::unique_ptr<exec_stream_t> stream)
        : stream(std::move(stream))
    {}

    std::string GetText(std::uint32_t textId);
};