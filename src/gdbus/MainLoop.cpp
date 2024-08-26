#include "bt/gdbus/MainLoop.hpp"

namespace bt::glib
{
MainLoop::MainLoop()
    : _loop(nullptr)
{
    _loop = g_main_loop_new(NULL, FALSE);
}

MainLoop::~MainLoop()
{
    if (_loop) {
        g_main_loop_unref(_loop);
    }
}

void MainLoop::run()
{
    g_main_loop_run(_loop);
}

void MainLoop::quit()
{
    g_main_loop_quit(_loop);
}

bool MainLoop::isRunning()
{
    return g_main_loop_is_running(_loop);
}

} // namespace bt::glib
