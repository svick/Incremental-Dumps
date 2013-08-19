#pragma once

#include "ChangeProcessor.h"

class DiffReader
{
private:
    std::unique_ptr<std::istream> stream;
    ChangeProcessor &changeProcessor;
public:
    DiffReader(std::string fileName, ChangeProcessor &changeProcessor);

    void Read();
};