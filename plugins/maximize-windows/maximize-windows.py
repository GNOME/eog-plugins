# -*- coding: utf-8 -*-
# Maximize Windows Plugin for Eye of GNOME
# Copyright (C) 2015 Felix Riemann <friemann@gnome.org>
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software Foundation,
# Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

from gi.repository import GObject, Eog


class MaximizeWindows(GObject.Object, Eog.WindowActivatable):
    """Plugin causing new EogWindows to open maximized"""

    window = GObject.property(type=Eog.Window)

    def __init__(self):
        super(MaximizeWindows, self).__init__()

    def do_activate(self):
        """Maximize each window this plugin is activated for"""
        self.window.maximize()

    def do_deactivate(self):
        """Does nothing on deactivation"""
        pass
