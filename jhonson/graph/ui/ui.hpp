#pragma once

//-----------------------------------------------------------------------------------------

#include "debug_utils.hpp"
#include "graph_impl.hpp"
#include "graphviz.h"
#include "time_control.hpp"
#include <set>
#include <utility>
#include <vector>

//-----------------------------------------------------------------------------------------

namespace graph_ui {

using namespace time_control;

void run(std::istream &in_strm = std::cin);

} // namespace graph_ui
