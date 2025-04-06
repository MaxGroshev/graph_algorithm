#include "ui.hpp"
import <iostream>;

//-----------------------------------------------------------------------------------------

int main() {
    using namespace graph_ui;

    auto start_time = time_control::chrono_cur_time ();
    graph_ui::run();
    auto end_time = time_control::chrono_cur_time ();

    std::clog << "----------------------------------------------\n";
    std::clog << "Total run time: " << (end_time - start_time) / 0.1ms
    << " * 10^(-5) sec\n";

    return 0;
}
