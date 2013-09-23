#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include "libexecstream/exec-stream.h"

/**
 * Used to retrieve text from MediaWiki using the @c fetchText.php maintenance script.
 */
class FetchText
{
private:
    std::unique_ptr<exec_stream_t> stream;
public:
    /**
    * @param stream Read-write stream that represents the call to @c fetchText.php.
    */
    FetchText(std::unique_ptr<exec_stream_t> stream)
        : stream(std::move(stream))
    {}

    /**
     * Returns the text for the given @a textId.
     */
    std::string GetText(std::uint32_t textId);
};