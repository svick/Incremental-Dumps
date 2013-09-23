#pragma once

#include "ChangeProcessor.h"

/**
 * Class that contains static data that is used from both DiffReader and DiffWriter.
 */
class Diff
{
private:
    Diff(){};
public:
    /**
     * Each diff dump has to start with this string.
     */
    static const std::string MagicNumber;
};

/**
 * Reads changes from diff dump and sends them to ChangeProcessor.
 */
class DiffReader
{
private:
    std::unique_ptr<std::istream> stream;
    ChangeProcessor &changeProcessor;
public:
    /**
     * @param fileName Path to the diff dump.
     * @param changeProcessor All changes read from the diff dump are sent to this object.
     */
    DiffReader(std::string fileName, ChangeProcessor &changeProcessor);

    /**
     * Starts reading from the diff dump.
     */
    void Read();
};