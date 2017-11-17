#!/usr/bin/env python

import pygtk
pygtk.require('2.0')
import gtk
import os
import gobject
import subprocess

class ControllerTest:

    def delete_event(self, widget, event, data=None):
        # If you return FALSE in the "delete_event" signal handler,
        # GTK will emit the "destroy" signal. Returning TRUE means
        # you don't want the window to be destroyed.
        # This is useful for popping up 'are you sure you want to quit?'
        # type dialogs.
        print "delete event occurred"

        # Change FALSE to TRUE and the main window will not be destroyed
        # with a "delete_event".
        return False

    def destroy(self, widget, data=None):
        print "destroy signal occurred"
        gtk.main_quit()

    def readupstream(self, widget, data=None):
        p = self.upstream.readline();
        print(p)
        if p == '0\n':
          s = str(self.up.get_active()) + '\n'
          self.downstream.write(s)
          print(s)
        elif p == '1\n':
          s = str(self.run.get_active()) + '\n'
          self.downstream.write(s)
          print(s)
        elif p == 'status\n':
          if self.running.get_active() == True:
            self.downstream.write('0\n')
          else:
            self.downstream.write('3\n')
        elif p == 'start\n':
          self.progindicator.set(gtk.ARROW_UP, gtk.SHADOW_IN)
        elif p == 'stop\n':
          self.progindicator.set(gtk.ARROW_DOWN, gtk.SHADOW_IN)
        elif p == '/sbin/poweroff\n':
          self.procindicator.set(gtk.ARROW_DOWN, gtk.SHADOW_IN)
        else:
          print("Unknown pin: ", p)
        return True

    def readcerr(self, widget, dada=None):
        p = self.cerror.readline();
        print(p)
        return True

    def __init__(self):
        # create a new window
        self.window = gtk.Window(gtk.WINDOW_TOPLEVEL)
        self.window.set_title("Controller Test")
        self.basebox = gtk.VBox(False, 10)
        self.basebox.show()
        self.window.add(self.basebox)
        self.statusbox = gtk.HBox(False, 10)
        self.basebox.pack_start(self.statusbox, True, True, 10)
        self.commandbox = gtk.HBox(False, 0)
        self.basebox.pack_start(self.commandbox, True, True, 10)
    
        # When the window is given the "delete_event" signal (this is given
        # by the window manager, usually by the "close" option, or on the
        # titlebar), we ask it to call the delete_event () function
        # as defined above. The data passed to the callback
        # function is NULL and is ignored in the callback function.
        self.window.connect("delete_event", self.delete_event)
    
        # Here we connect the "destroy" event to a signal handler.  
        # This event occurs when we call gtk_widget_destroy() on the window,
        # or if we return FALSE in the "delete_event" callback.
        self.window.connect("destroy", self.destroy)
    
        # Sets the border width of the window.
        self.window.set_border_width(10)
    
        # The up/down arrows.
        self.progindicator = gtk.Arrow(gtk.ARROW_DOWN, gtk.SHADOW_IN)
        self.progindicator.set_tooltip_text("Program")
        self.procindicator = gtk.Arrow(gtk.ARROW_UP, gtk.SHADOW_IN)
        self.procindicator.set_tooltip_text("Processor")
    
        self.statusbox.pack_start(self.progindicator, True, True, 0)
        self.statusbox.pack_start(self.procindicator, True, True, 0)

        self.run = gtk.CheckButton("Run")
        self.run.set_active(False)
        self.run.show()
        self.up  = gtk.CheckButton("Up")
        self.up.set_active(True)
        self.up.show()
        self.running = gtk.CheckButton("Running")
        self.running.set_active(True)
        self.running.show()
        self.status = gtk.Statusbar()
        self.status.show()

        self.commandbox.pack_start(self.run, True, True, 10)
        self.commandbox.pack_start(self.up, True, True, 10)
        self.commandbox.pack_start(self.running, True, True, 10)
        
        # Run program under test
        p = subprocess.Popen("tee ./upstream.txt | ~/src/Gliding-Ops/out/build/LogKeeper/Pi/src/x86-linux-default/controller-x86-linux-default | tee ./downstream.txt",
                             shell=True,
                             bufsize=1,
                             stdin=subprocess.PIPE,
                             stdout=subprocess.PIPE,
                             stderr=subprocess.PIPE,
                             close_fds=True)
        (self.upstream, self.downstream, self.cerror) = (p.stdout, p.stdin, p.stderr)

        # Setup pipe callbacks.
        gobject.io_add_watch(self.upstream, gobject.IO_IN, self.readupstream)
        gobject.io_add_watch(self.cerror,   gobject.IO_IN, self.readcerr)
    
        # The final step is to display this newly created widget.
        self.progindicator.show()
        self.procindicator.show()
        self.statusbox.show()
        self.commandbox.show()
    
        # and the window
        self.window.show()

    def main(self):
        # All PyGTK applications must have a gtk.main(). Control ends here
        # and waits for an event to occur (like a key press or mouse event).
        gtk.main()

# If the program is run directly or passed as an argument to the python
# interpreter then create a ControllerTest instance and show it
if __name__ == "__main__":
    test = ControllerTest()
    test.main()
