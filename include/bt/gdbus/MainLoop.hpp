#include <glib.h>

namespace bt::glib
{
class MainLoop
{
public:
    MainLoop();
    ~MainLoop();
    void Run();
    void Quit();
    bool IsRunning();
private:
    GMainLoop *_loop;
};
} // namespace bt::glib
