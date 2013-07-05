#pragma once

#include <cstdint>
#include <memory>
#include "SpaceManager.h"

using std::int64_t;
using std::weak_ptr;

class WritableDump;

class SpaceManager
{
private:
    weak_ptr<WritableDump> dump;
public:
    SpaceManager(weak_ptr<WritableDump> dump = weak_ptr<WritableDump>());
    int64_t GetSpace(int64_t length);
    void Delete(int64_t offset);
};