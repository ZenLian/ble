#include <vector>

namespace ble
{
    class Adapter
    {
    public:
        static std::vector<Adapter> GetAdapters();
    };

}