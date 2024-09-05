#!/usr/bin/env python3

from __future__ import absolute_import, print_function, unicode_literals

from optparse import OptionParser, make_option
import os
import sys
import dbus
import dbus.service
import dbus.mainloop.glib
from gi.repository import GLib

class Profile(dbus.service.Object):
    def __init__(self, bus, path):
        print("Create Profile: %s" % (path))
        self.bus = bus
        self.path = path
        if self.uuid == None:
            raise Exception("missing uuid")
        
        super().__init__(bus, path)
    
    def Register(self):
        manager = dbus.Interface(bus.get_object("org.bluez",
                    "/org/bluez"), "org.bluez.ProfileManager1")
        options = {
            "AutoConnect" : False,
            # "Name": "SP",
            # "Role": "server",
        }
        if self.channel != None:
            options["Channel"] = dbus.UInt16(self.channel)
        if hasattr(self, "role") and self.role in ["server", "client"]:
            options["Role"] = dbus.String(self.role)
        manager.RegisterProfile(profile.path, self.uuid, options)

    @dbus.service.method("org.bluez.Profile1",
                    in_signature="", out_signature="")
    def Release(self):
        print("Release")
        mainloop.quit()

    @dbus.service.method("org.bluez.Profile1",
                    in_signature="", out_signature="")
    def Cancel(self):
        print("Cancel")

    @dbus.service.method("org.bluez.Profile1",
                in_signature="oha{sv}", out_signature="")
    def NewConnection(self, path, fd, properties):
        self.fd = fd.take()
        print("[Profile] NewConnection(%s, %d)" % (path, self.fd))
        for key in properties.keys():
            if key == "Version" or key == "Features":
                print("  %s = 0x%04x" % (key, properties[key]))
            else:
                print("  %s = %s" % (key, properties[key]))

    @dbus.service.method("org.bluez.Profile1",
                in_signature="o", out_signature="")
    def RequestDisconnection(self, path):
        print("RequestDisconnection(%s)" % (path))

        if (self.fd > 0):
            os.close(self.fd)
            self.fd = -1

class SerialPortProfile(Profile):
    def __init__(self, bus, path):
        self.path = path
        self.uuid = "1101"
        self.channel = 1
        self.role = "server"
        super().__init__(bus, path)
    
    def NewConnection(self, path, fd, properties):
        self.fd = fd.take()
        print("NewConnection(%s, %d)" % (path, self.fd))
        GLib.io_add_watch(self.fd, GLib.IO_IN, self.OnReadReady)
    
    def OnReadReady(self, fd, condition):
        buf = os.read(fd, 100)
        print("[READ]%s" % (buf.decode(),))
        os.write(fd, buf)
        GLib.io_add_watch(self.fd, GLib.IO_IN, self.OnReadReady)


if __name__ == '__main__':
    dbus.mainloop.glib.DBusGMainLoop(set_as_default=True)

    bus = dbus.SystemBus()

    profile = SerialPortProfile(bus, "/my/spp")

    mainloop = GLib.MainLoop()

    profile.Register()

    print("running...")
    mainloop.run()