#!/usr/bin/env python

import os
from ldtp import *
from ldtputils import *

__author__ = "Nguyen Thanh Trung <nguyenthanh.trung@nomovok.com"
__client_path__ = "../../client/gst-ipcam-client"

__stream_type_h264__ = "H264 stream"
__stream_type_jpeg__ = "Jpeg stream"
__stream_type_mpeg4__ = "MPEG4 stream"
__main_window_title__ = "Gst Ipcam Client"
__connection_dialog_title__ = "Connection"
__error_dialog_title__ = "Error"

class gisMcClient:
	''' a class to manage an instance of a client '''
	client_path = __client_path__
	STATE_DISCONNECTED = 0
	STATE_CONNECTING = 1
	STATE_PLAYING = 2
	STATE_PAUSED = 3
	state_names = { STATE_DISCONNECTED: "disconnected", STATE_CONNECTING: "connecting", STATE_PLAYING: "playing", STATE_PAUSED: "paused" }
	def __init__(self, id):
		''' init the object '''
		global __client_path__
		#self.client_path = __client_path__
		self.set_id(id)
		self.url = ""
		self.stream_type = ""
		self.current_state = gisMcClient.STATE_DISCONNECTED
		try:
			ret = onwindowcreate(self.error_dialog_title, self.on_connection_error)
			if ret == 1:
				# ok
				pass
		except LdtpExecutionError, e:
			self.process_ldtp_exception(e)
			pass
		#print "Error dialog name is ", self.error_dialog_title

	def launch(self):
		''' launch the client if it has not running : there is not any client with same id is running '''
		#print "Main window short title is ", self.main_window_short_title
		if self.is_running():
			#print "already existed !"
			return
		#print "Ok, we need to launch it, the command is ", self.client_path, " --id ", self.id
		#launchapp(self.client_path, ['--id', str(self.id)])
		cmd = gisMcClient.client_path + " --id " + str(self.id) + " > /dev/null 2>&1 &"
		os.system(cmd)
		#print "Done"

	def set_id(self, id):
		''' set id for the client. This's should not be called out side __init__() function '''
		global __main_window_title__, __connection_dialog_title__, __error_dialog_title__
		self.id = id;
		self.main_window_title = self.create_window_title(__main_window_title__)
		self.main_window_short_title = "frm" + self.main_window_title.replace(" ", "")
		self.connection_dialog_title = self.create_window_title(__connection_dialog_title__)
		self.error_dialog_title = self.create_window_title(__error_dialog_title__)
		#self.error_dialog_title = "dlgError"

	def create_window_title(self, title):
		''' create the window title with client id '''
		return title + " - " + str(self.id)

	def connect(self, url, stream_type = None):
		''' connect to given url '''
		self.url = url
		self.stream_type = stream_type
		try:
			#print "click connect button"
			click(self.main_window_title, "btnConnect")
			#print "now, input value"
			settextvalue(self.connection_dialog_title, "txt0", self.url)
			#settextvalue(self.connection_dialog_title, "txt0", self.stream_type)
			click(self.connection_dialog_title, "btnConnect")
			#ret = waittillguiexist(self.error_dialog_title)
			#if ret == 1:
			#	self.on_connection_error()
			self.set_state(gisMcClient.STATE_CONNECTING)
		except LdtpExecutionError, e:
			self.process_ldtp_exception(e)
			pass

	def disconnect(self):
		''' disconnect the client '''
		#if self.is_button_ready("btnDisconnect"):
		try:
			click(self.main_window_title, "btnDisconnect")
			self.set_state(gisMcClient.STATE_DISCONNECTED)
		except LdtpExecutionError, e:
			self.process_ldtp_exception(e)
			pass

	def play(self):
		''' resume the playing state of the client '''
		#if self.is_button_ready("btnResume"):
		try:
			#print "Ok, we can resume"
			click(self.main_window_title, "btnResume")
			self.set_state(gisMcClient.STATE_CONNECTING)
		except LdtpExecutionError, e:
			self.process_ldtp_exception(e)
			pass

	def pause(self):
		''' pause the playing client '''
		#if self.is_button_ready("btnPause"):
		try:
			click(self.main_window_title, "btnPause")
			self.set_state(gisMcClient.STATE_PAUSED)
		except LdtpExecutionError, e:
			self.process_ldtp_exception(e)
			pass

	def quit(self):
		''' quit the client '''
		#if self.is_running():
		try:
			#print "quiting ...", self.main_window_title
			click(self.main_window_title, "btnQuit")
		except LdtpExecutionError, e:
			self.process_ldtp_exception(e)
			pass

	def set_client_path(cls, path):
		''' set the client path. This's the class method and should be called before any gisMcClient object construction '''
		gisMcClient.client_path = path
	set_client_path = classmethod(set_client_path)

	def get_current_state(self, check = False):
		''' get the current state of the client: unused right now '''
		if check == True:
			self.check_state()
		return self.current_state

	def set_state(self, state):
		''' set the state to the client: useless right now '''
		if state >= gisMcClient.STATE_DISCONNECTED and state <= gisMcClient.STATE_PAUSED:
			self.current_state = state

	def is_running(self):
		''' check if there's a client with the same id is running '''
		try:
			getwindowsize(self.main_window_title)
			return True
		except LdtpExecutionError, e:
			self.process_ldtp_exception(e)
			return False

	def on_connection_error(self):
		''' this function will be called when there's the connection error when using connection dialog '''
		try:
			click(self.error_dialog_title, "btnOK")
			#self.set_state(gisMcClient.STATE_DISCONNECTED)
			#print "Can not connect to: ", self.url
			waittillguinotexist(self.error_dialog_title)
			self.check_state()
		except LdtpExecutionError, e:
			self.process_ldtp_exception(e)
			pass

	def is_button_ready(self, btnObjectName, window = None):
		''' check if we can perfom click action on a button inside window '''
		if window == None:
			window = self.main_window_title
		return btnObjectName in getobjectlist(window) and stateenabled(window, btnObjectName)

	def check_state(self):
		remap("gst-ipcam-client", self.main_window_title)
		if not self.is_running() or self.is_button_ready("btnConnect"):
			self.set_state(gisMcClient.STATE_DISCONNECTED)
			return
		if self.is_button_ready("btnPause"):
			self.set_state(gisMcClient.STATE_PLAYING)
			return
		if self.is_button_ready("btnResume"):
			self.set_state(gisMcClient.STATE_PAUSED)
			return
		self.set_state(gisMcClient.STATE_CONNECTING)

	def validate_client_path(cls):
		''' validate if the client path is valid '''
		if os.path.isfile(gisMcClient.client_path):
			return True
		return False
	validate_client_path = classmethod(validate_client_path)

	def get_client_path(cls):
		''' get current client path '''
		return gisMcClient.client_path
	get_client_path = classmethod(get_client_path)

	def process_ldtp_exception(self, e):
		if str(e) == 'Server aborted':
			try:
				ret = onwindowcreate(self.error_dialog_title, self.on_connection_error)
				if ret == 1:
					# ok
					pass
			except:
				pass

