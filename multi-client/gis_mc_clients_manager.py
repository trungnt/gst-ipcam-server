#!/usr/bin/env python

from ldtp import *
from ldtputils import *
from gis_mc_client import gisMcClient

__author__ = "Nguyen Thanh Trung <nguyenthanh.trung@nomovok.com"
__main_window_short_title__ = "frmGstIpcamClient"

class gisMcClientsMng:
	''' a class to managed a list of client '''
	def __init__(self, gui_mode = False):
		# list of actived clients
		self.clients = {}
		self.max_id = 0
		# list of removed ids
		self.removed = []
		self.gui_mode = gui_mode
		if self.gui_mode == True:
			# list of selected ids
			self.selected = []
			self.clients_store = gtk.ListStore(int, str, str)
			self.selected_store = gtk.ListStore(int, str, str)

	def next_id(self):
		''' get the next suitable id to add to our client list '''
		if len(self.removed) > 0:
			id = self.removed.pop(0)
			return id
		return (self.max_id + 1)

	def has_client(self, id):
		''' check if there's a client with same id '''
		return self.clients.has_key(id)

	def add_client(self, id = None):
		''' add a client to list, do not launch if it already launch '''
		if id == None:
			id = self.next_id()
		#print "id = ", id
		client = gisMcClient(id)
		# should check if client can be created here
		self.clients[id] = client
		#print "going to launch it"
		client.launch()
		if self.gui_mode == True:
			print " append then "
			self.clients_store.append([id, "Gst Ipcam Client - " + str(id), "no state"])
		if self.max_id < id:
			self.max_id = id

	def add_clients(self, count):
		''' add $count clients to list '''
		#print "Going to add ", count, "client(s)"
		for i in range(count):
			#print "Number ", i
			self.add_client()

	def append_clients(self, ids):
		''' append a list of clients with given id to list, launch if the client is not running '''
		if not isinstance(ids, list):
			ids = [ids]
		for id in ids:
			self.add_client(id)

	def remove_client(self, id):
		''' remove and quit the client with given id '''
		#print "Removing client with id ", id
		#client = self.clients.pop(id, None)
		if not self.has_client(id):
			#print "How come, no client with id =", id
			#print str(self.clients)
			return
		client = self.clients[id]
		client.quit()
		del self.clients[id]
		self.removed.append(id)
		self.removed.sort()
		if self.gui_mode == True:
			self.remove_from_store(self.clients_store, id)
			if id in self.selected:
				self.deselect(id)
	
	def remove_clients(self, ids = None):
		''' remove a list of clients. If ids == None, then remove all clients '''
		if ids == None:
			#remove all clients
			ids = self.list_clients()
		if not isinstance(ids, list):
			ids = [ids]

		for id in ids:
			self.remove_client(id)

	def list_clients(self):
		''' list managed client ids '''
		return self.clients.keys()

	def get_client(self, id):
		''' get the client with given id. If there's not any client with same id, return None '''
		if not self.has_client(id):
			return None
		return self.clients[id]

	def list_unmanaged_clients(self):
		''' This will search all current running client and list what are not under controlled of this class '''
		global __main_window_short_title__
		prefix = __main_window_short_title__
		unmanaged = []
		for title in getwindowlist():
			if prefix in title:
				id = int(title.split("-")[1].strip())
				if not self.has_client(id):
					unmanaged.append(id)
		return unmanaged

	def connect(self, ids, url, stream_type = None):
		''' connect given clients to given url. We do not use stream_type now, it's deprecated '''
		if not isinstance(ids, list):
			ids = [ids]
		for id in ids:
			self.get_client(id).connect(url, stream_type)

	def disconnect(self, ids):
		''' disconnect given clients '''
		if not isinstance(ids, list):
			ids = [ids]
		for id in ids:
			self.get_client(id).disconnect()

	def play(self, ids):
		''' resume given clients if they are paused '''
		if not isinstance(ids, list):
			ids = [ids]
		for id in ids:
			self.get_client(id).play()

	def pause(self, ids):
		''' paused given clients if they are playing '''
		if not isinstance(ids, list):
			ids = [ids]
		for id in ids:
			self.get_client(id).pause()

	def select(self, id):
		''' for gui mode, useless now '''
		if self.gui_mode == True:
			self.selected.append(id)
			self.selected_store.append([id, str(id), str(self.get_client(id).get_current_state)])

	def unselect(self, id):
		''' for gui mode, useless now '''
		if self.gui_mode == True:
			self.selected.remove(id)
			self.remove_from_store(self.selected_store, id)

	def remove_from_store(self, store, id):
		''' remove a client row from a store
		This can be the all client store or selected clients store
		for gui mode, useless now '''
		if self.gui_mode == True:
			iter = store.get_iter_first()
			while iter:
				row = store[iter]
				if row[0] == id:
					store.remove(iter)
					return
				iter = store.iter_next(iter)
		
	def set_state_for_row(self, store, id, state):
		''' for gui mode, useless now '''
		if self.gui_mode == True:
			for row in store:
				if row[0] == id:
					row[2] = str(state)

	def get_clients_model(self):
		''' for gui mode, useless now '''
		if self.gui_mode == True:
			return self.clients_store
		return None

	def get_selected_model(self):
		''' for gui mode, useless now '''
		if self.gui_mode == True:
			return self.selected_store
		return None


