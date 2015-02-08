# -*- coding: utf-8 -*-

# __init__.py -- plugin object
#
# Copyright (C) 2007 - Diego Escalante Urrelo
# Copyright (C) 2006 - Steve Frécinaux
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

# Parts from "Interactive Python-GTK Console" (stolen from gedit's python
# console.py which was actually stolen from epiphany's console.py)
#     Copyright (C), 1998 James Henstridge <james@daa.com.au>
#     Copyright (C), 2005 Adam Hooper <adamh@densi.com>
#     Copyrignt (C), 2005 Raphaël Slinckx

from gi.repository import GObject, Gio, GLib, Gtk, Eog, PeasGtk

from .console import PythonConsole
from .config import PythonConsoleConfigWidget

import gettext
_ = gettext.translation('eog-plugins', fallback=True).gettext

_MENU_ID = 'PythonConsole'
_ACTION_NAME = 'python-console'


class PythonConsolePlugin(GObject.Object, Eog.WindowActivatable,
                          PeasGtk.Configurable):

    # Override EogWindowActivatable's window property
    window = GObject.property(type=Eog.Window)
    action_group = None

    def __init__(self):
        GObject.Object.__init__(self)
        self.console_window = None
        self.ui_id = 0

    def do_activate(self):
        model = self.window.get_gear_menu_section('plugins-section')
        action = Gio.SimpleAction.new(_ACTION_NAME)
        action.connect('activate', self.console_cb, self.window)

        self.window.add_action(action)
        menu = Gio.Menu()
        menu.append(_('P_ython Console'), 'win.python-console')
        item = Gio.MenuItem.new_section(None, menu)
        item.set_attribute([('id', 's', _MENU_ID)])
        model.append_item(item)

    def do_deactivate(self):
        menu = self.window.get_gear_menu_section('plugins-section')
        for i in range(0, menu.get_n_items()):
            value = menu.get_item_attribute_value(i, 'id',
                                                  GLib.VariantType.new('s'))

            if value and value.get_string() == _MENU_ID:
                menu.remove(i)
                break
        self.window.remove_action(_ACTION_NAME)
        if self.console_window is not None:
            self.console_window.destroy()
            self.console_window = None

    def console_cb(self, simple, parameter, window):
        if not self.console_window:
            self.console_window = Gtk.Window()
            console = PythonConsole(namespace={'__builtins__': __builtins__,
                                               'Eog': Eog,
                                               'window': window})
            console.set_size_request(600, 400)
            console.eval('print("You can access the main window through '
                         '\'window\' :\\n%s" % window)', False)
            self.console_window.set_title(_('Python Console'))
            self.console_window.add(console)

            self.console_window.connect('delete-event', self.on_delete_cb)
            self.console_window.show_all()
            self.console_window.set_transient_for(window)
            self.console_window.set_destroy_with_parent(True)
        else:
            self.console_window.show_all()
        self.console_window.grab_focus()

    def on_delete_cb(self, window, event):
        window.destroy()
        self.console_window = None

    def do_create_configure_widget(self):
        data_dir = self.plugin_info.get_data_dir()
        config_widget = PythonConsoleConfigWidget(data_dir)

        return config_widget.configure_widget()
