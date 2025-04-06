#include "unit_tests.hpp"

//-----------------------------------------------------------------------------------------

int main (int argc, char* argv[]) {
    std::cout << GREEN_C << "START OF UNIT TESTING:\n" << RESET_C;

    testing::InitGoogleTest(&argc, argv);
    int ret_val = RUN_ALL_TESTS();

    std::cout << GREEN_C << "END OF UNIT TESTING\n" << RESET_C;
    return 0;
}
