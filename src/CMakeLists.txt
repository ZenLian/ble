add_library(blecpp
    gdbus/InterfaceProxy.cpp gdbus/MainLoop.cpp gdbus/ObjectProxy.cpp gdbus/ObjectManager.cpp
    api/Adapter.cpp api/Device.cpp api/Manager.cpp
    AdapterPrivate.cpp ManagerPrivate.cpp)