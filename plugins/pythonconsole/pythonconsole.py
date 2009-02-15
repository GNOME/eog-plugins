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

# Parts from "Interactive Python-GTK Console" (stolen from gedit's python console.pe which was actually stolen from epiphany's console.py)
#     Copyright (C), 1998 James Henstridge <james@daa.com.au>
#     Copyright (C), 2005 Adam Hooper <adamh@densi.com>
#     Copyrignt (C), 2005 Raphaël Slinckx

import gtk
import eog
from console import PythonConsole

ui_str = """
	<ui>
	  <menubar name="MainMenu">
	    <menu name="ToolsMenu" action="Tools">
	      <separator/>
	      <menuitem name="PythonConsole" action="PythonConsole"/>
	      <separator/>
	    </menu>
	  </menubar>
	</ui>
	"""

class PythonConsolePlugin(eog.Plugin):

	def __init__(self):
		eog.Plugin.__init__(self)
		self.console_window = None
			
	def activate(self, window):
		data = dict()
		ui_manager = window.get_ui_manager()
		data['group'] = gtk.ActionGroup('PythonConsole')
		data['group'].add_actions([('PythonConsole', None, 'P_ython Console', None, None, self.console_cb)], window)
		ui_manager.insert_action_group(data['group'], 0)
		data['ui_id'] = ui_manager.add_ui_from_string(ui_str)
		window.set_data('PythonConsolePluginInfo', data)
		window.connect('delete-event', self.self_deactivate)
	
	def deactivate(self, window):
		data = window.get_data('PythonConsolePluginInfo')
		ui_manager = window.get_ui_manager()
		ui_manager.remove_ui(data['ui_id'])
		ui_manager.remove_action_group(data['group'])
		ui_manager.ensure_update()
		window.set_data("PythonConsolePluginInfo", None)
		if self.console_window is not None:
			self.console_window.destroy()
		
	def console_cb(self, action, window):
		if not self.console_window:
			self.console_window = gtk.Window()
			console = PythonConsole(namespace = {'__builtins__' : __builtins__,
		    	                                 'eog' : eog,
		        	                             'window' : window})
			console.set_size_request(600, 400)
			console.eval('print "You can access the main window through ' \
		    	         '\'window\' :\\n%s" % window', False)
			
			
			self.console_window.set_title('Python Console')
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

# This is a workaround for a ref-counting problem with python plugins.
# This deactivates the plugin once the window that was used to activate
# it is closed. Breaks multi-window-usability though.
# See bug #460781 for more information.
	def self_deactivate(self, window, event):
		self.deactivate(window)

