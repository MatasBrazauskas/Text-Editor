#include <vector>
#include <iostream>

int main(void)
{
    std::vector vec = {1,2,3,4,5};
    
    for(const auto& i : vec)
    {
        std::cout << i << '\n';
    }
}

