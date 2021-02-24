# -*- coding: utf-8 -*-
#
# export.py -- export plugin for eog
#
# Copyright (c) 2012  Jendrik Seipp (jendrikseipp@web.de)
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

import gettext
import os
import shutil

from gi.repository import GObject, GLib, Eog, Gio, Gtk, PeasGtk

_ = gettext.translation('eog-plugins', fallback=True).gettext

_MENU_ID = 'Export'
_ACTION_NAME = 'export-to-folder'

EXPORT_DIR = os.path.join(os.path.expanduser('~'), 'exported-images')
BASE_KEY = 'org.gnome.eog.plugins.export-to-folder'


class ExportPlugin(GObject.Object, Eog.WindowActivatable):
    window = GObject.property(type=Eog.Window)

    def __init__(self):
        GObject.Object.__init__(self)
        self.settings = Gio.Settings.new(BASE_KEY)
        self.selection_changed_handler_id = None

    @property
    def export_dir(self):
        target_dir = self.settings.get_string('target-folder')
        if target_dir == "":
            return EXPORT_DIR

        return target_dir

    def do_activate(self):
        model = self.window.get_gear_menu_section('plugins-section')
        action = Gio.SimpleAction.new(_ACTION_NAME)
        action.connect('activate', self.export_cb, self.window)

        self.window.add_action(action)
        menu = Gio.Menu()
        menu.append(_('_Export'), 'win.' + _ACTION_NAME)
        item = Gio.MenuItem.new_section(None, menu)
        item.set_attribute([('id', 's', _MENU_ID)])
        model.append_item(item)

        thumbview = self.window.get_thumb_view()
        self.selection_changed_handler_id = \
            thumbview.connect('selection-changed', self.update_action_state)
        self.update_action_state(thumbview)

        # Add accelerator key
        app = Eog.Application.get_instance()
        app.set_accels_for_action('win.' + _ACTION_NAME, ['E', None])

    def do_deactivate(self):
        menu = self.window.get_gear_menu_section('plugins-section')
        for i in range(0, menu.get_n_items()):
            value = menu.get_item_attribute_value(i, 'id',
                                                  GLib.VariantType.new('s'))

            if value and value.get_string() == _MENU_ID:
                menu.remove(i)
                break

        # Disable accelerator key
        app = Eog.Application.get_instance()
        app.set_accels_for_action('win.' + _ACTION_NAME, ['E', None])

        if self.selection_changed_handler_id is not None:
            thumbview = self.window.get_thumb_view()
            thumbview.disconnect(self.selection_changed_handler_id)
            self.selection_changed_handler_id = None

        self.window.remove_action(_ACTION_NAME)

    def export_cb(self, action, parameter, window):
        # Get path to current image.
        image = window.get_image()
        if not image:
            print('No image can be exported')
            return
        src = image.get_file().get_path()
        name = os.path.basename(src)
        dest = os.path.join(self.export_dir, name)
        # Create directory if it doesn't exist.
        try:
            os.makedirs(self.export_dir)
        except OSError:
            pass
        shutil.copy2(src, dest)
        print('Copied %s into %s' % (name, self.export_dir))

    def update_action_state(self, thumbview=None):
        action = self.window.lookup_action(_ACTION_NAME)
        enable = False
        if thumbview is None:
            thumbview = self.window.get_thumb_view()

        if thumbview is not None:
            enable = (thumbview.get_n_selected() > 0)

        action.set_enabled(enable)


class ExportConfigurable(GObject.Object, PeasGtk.Configurable):

    def __init__(self):
        GObject.Object.__init__(self)
        self.settings = Gio.Settings.new(BASE_KEY)

    def do_create_configure_widget(self):
        # Create preference dialog
        signals = {'file_set_cb': self.file_set_cb}
        builder = Gtk.Builder()
        builder.set_translation_domain('eog-plugins')
        builder.add_from_file(os.path.join(self.plugin_info.get_data_dir(),
                                           'preferences_dialog.ui'))
        builder.connect_signals(signals)

        self.export_dir_button = builder.get_object('export_dir_button')
        self.preferences_dialog = builder.get_object('preferences_box')
        target_dir = self.settings.get_string('target-folder')
        if target_dir == "":
            target_dir = EXPORT_DIR
        self.export_dir_button.set_filename(target_dir)

        return self.preferences_dialog

    def file_set_cb(self, button):
        self.settings.set_string('target-folder', button.get_filename())
