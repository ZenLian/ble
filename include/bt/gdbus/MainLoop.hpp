#include <glib.h>

namespace bt::glib
{
class MainLoop
{
public:
    MainLoop();
    ~MainLoop();
    void run();
    void quit();
    bool isRunning();
private:
    GMainLoop *_loop;
};
} // namespace bt::glib
