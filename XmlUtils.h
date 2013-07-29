#pragma once

#include <string>
#include "XML/xmlinput.h"

std::string readElementData(XML::Element &elem);

std::string escapeElementText(std::string original);