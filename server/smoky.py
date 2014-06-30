#!/usr/bin/env python
# -*- coding: utf-8 -*-

from twisted.internet import reactor, protocol
from twisted.protocols import basic

import re
import string
import json
import time
import random
import os
import hashlib
import cv2
import numpy as np

import recognizer as rec
import database as db
from common import COMMANDS, display_message, validate_file_md5_hash, get_file_md5_hash, read_bytes_from_file, clean_and_split_input, match

COMMANDS = {
            'is': ('is', 'get the first name and last name of person ID'),
            'auth': ('auth', 'Login to the server'),
            'disc': ('disc logout from the server'),
            'log': ('log send log data to the server'),
            'get': ('get', 'Request some data from the server'),
            'getinfo': ('getinfo', 'get infos for search page'),
            'put': ('put', 'Send some data to the server'),
            'hist': ('hist', 'request the history data'),
            'rec': ('rec', 'send a recognition request'),
            'getsettings': ('getsettings', 'load get settings'),
            'saveoptions': ('saveoptions', 'save options'),
            'senduser': ('senduser', 'add or update user'),
            'deleteuser': ('deleteuser', 'delete a user with id'),
            'addperson': ('addperson', 'Add user with faces and infos'),
            'search': ('search', 'search with id or term'),
            'stats': ('stats', 'retrieve stats'),
            'update': ('update', 'update persons info'),
            'refute': ('refute', 'refute recognition decision'),
            'confirm': ('confirm', 'confirm recognition decision')

}





def generate_token():
	return hashlib.sha1(os.urandom(128)).hexdigest();

class Smoky(basic.LineReceiver):
	def __init__(self):
		#self.setRawMode()
		print "Smoky Instantiated ..."
		self.user_data = {"token": "0", "token_time": 0} # token, time
		self.MAX_LENGTH = 1024 * 1024 * 1024 * 1024
		self.delimiter = "<smoky-paquet>"

		self.rec = rec.Recognizer()
		self.db = db.Database()
		

	def _cleanAndSplitInput(self, input):
			input = input.strip()
			input = input.split('<smoky-break>')

			return input

	def transmit(self, query, json_str, raw):
		if not len(raw):
			raw = "nada"
		paquet = query + "<smoky-break>"  + json.dumps(json_str,ensure_ascii=False) + "<smoky-break>";
		display_message( "%s + raw(%d octets)" % (paquet, len(raw)) )
		self.sendLine( paquet + raw )

	def net_error(self, code):
		error_msg = "code : " + str( code ) + ". "
		error_answer = "-1"

		if code == -1: #general error, bad request
			error_msg = error_msg + "bad request"
			self.transmit( 'BAZINGA', {}, '' )

		if code == 0: #auth error
			error_msg = error_msg + "bad credentials OR disconnected"
			self.transmit( 'AUTH', {"token": "0"}, '' ) # bad request

		print( error_msg )

	def lineReceived(self, line):
		# display_message('Received a line from client [%s]' % (self.transport.getPeer().host) )
		data = self._cleanAndSplitInput(line)
		if len(data) == 0 or data == '':
			return 

		
		try:
			token = data[0]
			query = data[1].lower()
			params = json.loads( data[2] )
			raw = data[3];

		
		except Exception:
			print "INCOMPLETE DATA"
			# self.transport.write('Missing query or json_str or Wrong json_str\n')
			# self.transport.write('BAZINGA\n')
			return

		if not query in COMMANDS:

			print "'%s' COMMAND NOT FOUND" % query
			# self.transport.write('Invalid command\n')
			# self.transport.write('BAZINGA\n')
			return

		display_message( 'token = %s\tquery= %s\tparams= %s\traw size= %s\n' % (token, query, params, len(raw) ) )

		self.images = []
		
		if len(raw.strip()):
			if not os.path.exists( "tmp" ):
				os.makedirs( "tmp" )

			
			raw_images = raw.split("<smoky-image>")
			for raw_image in raw_images:
				if len(raw_image.strip()):
					file_path = "tmp/" + str( int ( time.time() * random.random() )) + ".png"

					if not self.file_handler:
						self.file_handler = open(file_path, 'wb')


					# write raw data in a PNG
					self.file_handler.write( raw_image )
					self.file_handler.close()
					self.file_handler = None

					# display_message( "File was successfully transfered and saved" );

					self.images.append( cv2.imread( file_path, cv2.CV_LOAD_IMAGE_COLOR ) );

					os.unlink(file_path)
					# display_message( "'%s' was deleted from tmp" % file_path )




		"""
			PROCESS THE RECEIVED LINE
		"""

		if token == '0':

			if query == "auth" and "username" in params and "password" in params :
				

				personId = -1
				if len( self.images ) > 0:
					personId,_ ,_ ,_ = self.rec.recognize( self.images[0] )
					
				self.db.user_id = 0;
				row = self.db.auth(params['username'], params['password'], personId)
				try:
					self.db.user_id = row["id"]
					self.db.user_role = row["role"]
					self.firstName = row["firstName"]

					# print self.db.user_id
					# print self.db.user_role
					# print self.firstName
				except Exception, e:
					print "auth error"
					return

				
				if self.db.user_id:
					new_token = generate_token();
					self.user_data = { "token" : new_token, "token_time": time.time() } # save token

					# answer
					answer_data = {"token": new_token, "firstName": self.firstName, "role": self.db.user_role, "user_id":self.db.user_id}
					self.transmit( "AUTH", answer_data, '' )
				else:

					self.transmit( 'AUTH', {"token": "0"}, '' ) # bad cred
			else:
				self.transmit( 'AUTH', {"token": "0"}, '' )
				print "bad request token = 0"

		else: #token != 0
			if token == self.user_data["token"] and time.time() < self.user_data["token_time"] + 60 * 30:

				# print "token valid :) reset Time"
				self.user_data["token_time"] = time.time()


				if query == "getsettings" and self.db.user_role == "admin":
					"""
					" query GETSETTINGS
					"""
					
					parameters = self.db.get_params()
					users = self.db.get_users()

					self.transmit("GETSETTINGS", {'parameters': parameters, 'users': users}, '')

				elif query == "search" and "personId" in params and "page" in params and "name" in params and "address" in params and "email" in params and "sexe" in params and "birthday" in params and "birthCity" in params and "phone" in params and "profession" in params:
					items, faces = self.db.search( params );
					self.transmit( 'SEARCH', items, faces )



				elif query == "update" and "personId" in params and "firstName" in params and "lastName" in params and "address" in params and "email" in params and "sexe" in params and "birthday" in params and "birthCity" in params and "phone" in params and "profession" in params:
					self.db.updatePerson( params );




				elif query == "addperson" and "firstName" in params and "lastName" in params and "address" in params and "email" in params and "sexe" in params and "birthday" in params and "birthCity" in params and "phone" in params and "profession" in params and len( self.images ) >= 1:
					newPersonId = self.db.addPersonInfo( params )
					self.rec.addPersonFaces( self.images, newPersonId )
					self.rec.update_models();
					self.transmit( 'PERSONADDED', {}, '')


				elif query == "saveoptions" and self.db.user_role == "admin":
					"""
					" query saveoptions
					"""
					print "SAVEOPTIONS";
					self.db.saveOptions( params )
					# self.db.saveUsers( params["users"] )

				elif query == "refute":
					"""
					" query refute
					"""
					print "REFUTE"
					self.db.setRecResult( -1, params["eigen_ratio"], params["fisher_ratio"], params["lbph_ratio"] )

				elif query == "confirm" and "personId" in params and len( self.images ) >= 1:
					"""
					" query confirm
					"""
					self.db.setRecResult( 1, params["eigen_ratio"], params["fisher_ratio"], params["lbph_ratio"] )

					self.rec.addPersonFaces( self.images, params['personId'] )
					self.rec.update_models();



				elif query == "stats":
					stats = self.db.getStats()
					self.transmit( "STATS", stats, '')

				elif query == "deleteuser" and "userId" in params and self.db.user_role == "admin":
					"""
					" query deleteuser
					"""

					self.db.deleteUser( params["userId"] )


				elif query == "senduser" and "userId" in params and "username" in params and "password" in params and "personId" in params and "role" in params and self.db.user_role == "admin":
					"""
					" query SENDUSER
					"""

					print "senduser"

					self.db.sendUser(params["userId"],params["username"], params["password"], params["personId"], params["role"] );

				elif query == "disc":
					"""
					" query DISC
					"""

					print "disc"

					self.transmit( 'AUTH', {"token": "0"}, '' )
					self.db.user_id = 0;
					self.db.log( 2, "" )
					self.user_data = {"token": "0", "token_time": 0} # token, time




				elif query == "log" and "op_id" in params and "op_data" in params:
					"""
					" query LOG
					"""
					print "log event"
					self.db.log(params["op_id"], params["op_data"]);





				elif query == "hist" and "username" in params and "op_id" in params and "op_date" in params and "page" in params and self.db.user_role == "admin":
					"""
					" query "HIST"
					"""
					logs_json = self.db.get_log(params["username"], params["op_id"], 0, params["op_date"], params["page"] )
					self.transmit("HIST", logs_json, '')




				elif query == "is" and "faceId" in params and "personId" in params:
					"""
					" query "IS"
					"""
					(firstName, lastName) = self.db.get_name( params["personId"] )
					print "%s ==> %s %s" % ( params["personId"], firstName, lastName )

					answer_json = {"faceId": params["faceId"], "firstName": firstName, "lastName": lastName}
					self.transmit("IS", answer_json , '')




				elif ( query == "get" or query == "getinfo") and "personId" in params:
					infos = self.db.get( params["personId"] )
					faceImage = self.db.get_default_face( params["personId"], (96,96) ) ;
					faceImage += "<smoky-image>"
					self.transmit( query.upper(), infos, faceImage ) 



				elif query == "rec" and "faceId" in params and "imageId" in params and len( self.images ) > 0:
					"""
					" query "REC"
					"""
					personId, eigen_ratio, fisher_ratio, lbph_ratio = self.rec.recognize( self.images[0] )
					(firstName, lastName) = self.db.get_name( personId )

					answer_json = {'faceId': params['faceId'], 'imageId': params['imageId'], 'personId': personId, 'eigen_ratio':eigen_ratio, 'fisher_ratio':fisher_ratio, 'lbph_ratio':lbph_ratio, 'firstName': firstName, 'lastName': lastName}
					
					self.db.setRecResult(0, eigen_ratio, fisher_ratio, lbph_ratio)
					self.transmit( "REC", answer_json , '')



			else:
				print "le token n'est plus valid"
				self.transmit( 'AUTH', {"token": "0"}, '' ) # bad cred

	def rawDataReceived(self, data):
		# I'll never receive raw data from here :)
		print "anwi id issiwlen i rawDataReceived ?"

	def connectionMade(self):
		print "Client Connecté"

		self.file_handler = None

	def connectionLost(self, reason):
		self.file_handler = None
		print "Client Deconnecté !\n"





class SmokyFactory(protocol.ServerFactory):
	def __init__(self):
		print "Serveur lancé: attente d'une connexion..."

	def buildProtocol(self, addr):
		print "<{}>".format(addr)
		return Smoky()

host, port = "localhost", 3886
reactor.listenTCP( port, SmokyFactory() )
reactor.run()