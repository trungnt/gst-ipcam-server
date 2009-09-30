#!/usr/bin/env python

import sys, os
from gis_mc_clients_manager import gisMcClientsMng
from gis_mc_client import gisMcClient

__author__ = "Nguyen Thanh Trung <nguyenthanh.trung@nomovok.com"
command_list = [ "list", "add", "remove", "connect", "disconnect", "pause", "resume", "exit", "help" ]
manager = gisMcClientsMng()

def show_intro():
	''' show the introduction and usage '''
	introduction = """Gst Ipcam Server: multiple simultaneous clients
Avaiable commands:
	- help						show this screen
	- list						list all running clients
	- add number_of_clients				to add number_of_clients clients
	- remove [client ids]				quit clients with given ids in the list
	- remove all					quit all maanged clients
	- connect url [client ids]			connect given clients to given url
	- disconnect [client ids]			disconnect clients with given ids in the list
	- pause [client ids]				pause connected clients with given ids
	- resume [client ids]				resume paused clients with given ids
	- exit						exit the program
For examples:
	add 3						# launch 3 client
	remove 1					# remove the client with id = 1
	connect rtsp://localhost:8554/jpg 2 3		# connect the client 2, 3 to rtsp://localhost:8554/jpg
	...
"""
	print introduction

def get_client_ids(command_args, start_pos):
	''' process the command args and get client ids, start from start_pos '''
	ids = []
	for i in range(len(command_args) - start_pos):
		try:
			id = int(command_args[i + start_pos])
			if id > 0:
				ids.append(id)
		except:
			pass
	if len(ids) <= 0:
		print "Wrong ids list format ! (must be integer)."
	return ids

def process_input(cmd):
	''' process input command and perform needed actions '''
	global command_list, manager
	command_args = cmd.split(" ")
	command = command_args[0]
	if command not in command_list:
		print "Sorry, I don't understand what you want."
		return -1
	if command == "exit":
		manager.remove_clients()
		return 0
	elif command == "list":
		managed = manager.list_clients()
		if len(managed) > 0:
			print "List of managed client ids:", str(managed)
		else:
			print "There isn't any managed client. Use add command to add some"
		#for id in managed:
		#	print "id: ", id, ", state: ", gisMcClient.state_names[manager.get_client(id).get_current_state(True)]
		return 1
	elif command == "add":
		if len(command_args) < 2:
			print "add command require a parameter"
			return -1
		number_of_clients = int(command_args[1])
		if number_of_clients <= 0:
			print "Please give an positive integer for add command"
			return -1
		manager.add_clients(number_of_clients)
		return 1
	elif command == "remove":
		if len(command_args) < 2:
			print "remove command require client id(s) to removed"
			return -1
		if (command_args[1] == "all"):
			manager.remove_clients()
			return 1
		ids = get_client_ids(command_args, 1)
		manager.remove_clients(ids)
		return 1
	elif command == "connect":
		if len(command_args) < 3:
			print "connect command require at least 2 parameters"
			return -1
		url = command_args[1]
		ids = get_client_ids(command_args, 2)
		manager.connect(ids, url)
		return 1
	elif command == "disconnect":
		if len(command_args) < 2:
			print "disconnect command require at least a parameters"
			return -1
		ids = get_client_ids(command_args, 1)
		manager.disconnect(ids)
		return 1
	elif command == "pause":
		if len(command_args) < 2:
			print "pause command require client ids to pause"
			return -1
		ids = get_client_ids(command_args, 1)
		manager.pause(ids)
		return 1
	elif command == "resume":
		if len(command_args) < 2:
			print "resume command require client ids to resume"
			return -1
		ids = get_client_ids(command_args, 1)
		manager.play(ids)
		return 1
	elif command == "help":
		show_intro()
		return 1
	else:
		print "For now, i can not do the work from command: ", command
		return 1
	return 1
		
def check_client_path(base_dir, client_name = 'gis-ipcam-client', cli_paths = [], checked_dirs = []):
	''' check and list the client with given name in given basedir '''
	if base_dir in checked_dirs:
		return
	checked_dirs.append(base_dir)
	path = os.path.join(base_dir, client_name)
	if os.path.isfile(path):
		cli_paths.append(path)
	for subdir in os.listdir(base_dir):
		if os.path.isdir(subdir):
			subdir = os.path.abspath(subdir)
			check_client_path(subdir, client_name, cli_paths, checked_dirs)

def find_file(file_name, path_list):
	''' find all client with file_name in given path_list '''
	client_paths = []
	paths = path_list.split(',')
	for path in paths:
		check_client_path(path, file_name, client_paths)
	return client_paths

def on_client_not_found(path_list = ".,..,../client", client_name = "gst-ipcam-client"):
	''' this function will be called in the case that our program can not search any client in default path '''
	print "Can not find the client, please give me something to search:"
	new_path_list = raw_input("Directories to search: [" + path_list + "]). If ok, input Y, else, give me new paths:")
	if new_path_list == "Y" or new_path_list == "y":
		pass
	else:
		path_list = new_path_list
	new_client_name = raw_input("Name of the client is " + client_name + ". If ok, input Y, else give me new name:")
	if new_client_name == "Y" or new_client_name == "y":
		pass
	else:
		client_name = new_client_name
	client_paths = find_file(client_name, path_list)
	if len(client_paths) <= 0:
		on_client_not_found(path_list, client_name)
	else:
		gisMcClient.set_client_path(client_paths[0])

if __name__ == "__main__":
	if gisMcClient.validate_client_path() == False:
		dirs_to_search = str.join(",", [".", "..", "../client"])
		client_name = "gst-ipcam-client"
		client_paths = find_file(client_name, dirs_to_search)
		if len(client_paths) > 0:
			gisMcClient.set_client_path(client_paths[0])
		else:
			on_client_not_found(dirs_to_search, client_name)
	show_intro()
	while (1):
		command = raw_input("Give me a command:\n")
		process_input(command)
		if command == "exit":
			sys.exit(0)
