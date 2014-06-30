# -*- coding: utf-8 -*-
# import sqlite3
import MySQLdb
import MySQLdb.cursors
import json
import datetime
import cv2
import os
from common import match


def str_dict(tup):
	return	tuple( dict( (key, str(value))  for (key, value) in dic.items()  ) for dic in tup )

def str_oneDict(dic):
	return	dict( (key, str(value) ) for (key, value) in dic.items()  )


class Database:

	
	def __init__(self):
		# print "Database instantiated"
		# conn = sqlite3.connect("smoky.db")
		self.user_id = 0
		self.user_role = "user"
		self.db = None
		self.cursor = None

		print "connecting to database ..."
		self.db = MySQLdb.connect( host = "localhost", user ="root", passwd = "", cursorclass=MySQLdb.cursors.DictCursor, use_unicode=False, charset="utf8" )
		self.db.autocommit(1)

		cur = self.db.cursor();

		#construct if first run
		with open('smoky_db.sql', 'r') as content_file:
			sql_struct = content_file.read()

		for sql in sql_struct.split(";"): 
			cur = self.db.cursor();
			if not sql.strip() == '':
				cur.execute(sql)
			cur.close()

		self.db = MySQLdb.connect( host = "localhost", user ="root", passwd = "", db = "smoky_db", cursorclass=MySQLdb.cursors.DictCursor, use_unicode=False, charset="utf8" )
		self.db.autocommit(1)

			
	
	def getNextPersonId(self):
		cur = self.db.cursor();
		next_id_query = """SELECT id FROM persons ORDER BY id DESC LIMIT 1"""
		cur.execute( next_id_query )
		if ( cur.rowcount ):
			row = cur.fetchone()
			return row["id"] + 1
		else:
			return 1

	def getNextFaceId(self):
		cur = self.db.cursor();
		next_id_query = """SELECT id FROM faces ORDER BY id DESC LIMIT 1"""
		cur.execute( next_id_query )
		if ( cur.rowcount ):
			row = cur.fetchone()
			return row["id"] + 1
		else:
			return 1

	def get_threshold(self, method):
		cur = self.db.cursor();
		# get_rec_method_query = """SELECT option_value FROM options WHERE option_name = 'rec_method'"""
		# cur.execute( get_rec_method_query )
		# row = cur.fetchone()

		# method = "LBPHFace_threshold";
		# if ( row["option_value"] == 0):
		# 	method = "eigenFace_threshold"

		# if ( row["option_value"] == 1):
		# 	method = "fisherFace_threshold"

		method_str = "LBPHFace_threshold";
		if ( method == 0):
			method_str = "eigenFace_threshold"

		if ( method == 1):
			method_str = "fisherFace_threshold"


		get_threshold_query = """SELECT option_value FROM options WHERE option_name = '%s'""" % method

		cur.execute( get_threshold_query )
		row = cur.fetchone()

		print "threshold: %s" % row["option_value"]
		return row["option_value"]

	def get_actual_method(self):
		cur = self.db.cursor();
		get_rec_method_query = """SELECT option_value FROM options WHERE option_name = 'rec_method'"""
		cur.execute( get_rec_method_query )
		row = cur.fetchone()

		return row["option_value"]

	def add_face(self, person_id):
		cur = self.db.cursor();

		face_id = self.getNextFaceId();

		query = """INSERT INTO faces (id, persons_id) VALUES (%d, %d)""" % ( face_id, person_id )

		# print query
		cur.execute( query )
		return face_id

	def search(self, i):
		cur = self.db.cursor();

		persons_per_page = 20;
		dep = ( i["page"] - 1 ) * persons_per_page;

		firstName_str, lastName_str, sexe_str, profession_str, address_str, email_str, phone_str, birthCity_str, birthday_str, personId_str = '1=1', '1=1', '1=1', '1=1', '1=1', '1=1', '1=1', '1=1', '1=1', '1=1'

		if i["name"]:
			firstName = match( i["name"], "(([A-Z]? *)*[a-z]+)*" )
			lastName  = match( i["name"], "[A-Z]{2}[A-Z ]*" )

			if (firstName):
				firstName_str = "firstName SOUNDS LIKE '%s'" % ( '%' + firstName + '%' )

			if (lastName):
				lastName_str = "lastName SOUNDS LIKE '%s'" % ( '%' + lastName + '%' )


		if i["sexe"] == 0 or i["sexe"] == 1:
			sexe_str = 'sexe = %s' % i["sexe"]

		if i["profession"]:
			profession_str = 'profession LIKE %s' % ("'%" + i["profession"] + "%'")

		if i['personId']:
			personId_str = "id LIKE '%s'" % ( str( i['personId'] )  + '%')

		if i["address"]:
			address_str = 'address LIKE %s' % ("'%" + i["address"] + "%'")

		if i["email"]:
			email_str = 'email LIKE %s' % ("'%" + i["email"] + "%'")

		if i["phone"]:
			phone_str = 'phone LIKE %s' % ("'%" + i["phone"] + "%'")

		if i["birthCity"]:
			birthCity_str = 'birthCity LIKE %s' % ("'%" + i["birthCity"] + "%'")

		if i["birthday"]:
			birthday_str = 'birthday LIKE %s' % ("'%" + i["birthday"] + "%'")


		query = """SELECT firstName, lastName, id as personId
				FROM persons
				WHERE %s AND %s AND %s AND %s AND %s AND %s AND %s AND %s AND %s AND %s
				LIMIT %s, %s
				""" % (firstName_str, lastName_str, sexe_str, profession_str, personId_str, address_str, email_str, phone_str, birthday_str, birthday_str, dep, persons_per_page )

		cur.execute( query )
		self.log( "9", "")
		rows = cur.fetchall()

		images = ''
		for row in rows:
			images += self.get_default_face( row["personId"], (48,48) )
			images += "<smoky-image>"

		return str_dict( rows ), images

	def addPersonInfo(self, i):
		cur = self.db.cursor();

		nextId = self.getNextPersonId()
		query = """INSERT INTO persons (id, firstName, lastName, birthday, address, sexe, email, birthCity, phone, profession)
				VALUES (%d, '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s')""" % (nextId, i["firstName"], i["lastName"], i["birthday"],i["address"], i["sexe"], i["email"], i["birthCity"], i["phone"], i["profession"])

		cur.execute( query )
		self.log( "6", nextId )
		return nextId

	def updatePerson(self, i):
		cur = self.db.cursor();

		query = """UPDATE persons
		SET firstName = '%s',
		lastName   = '%s',
		birthday   = '%s',
		address    = '%s',
		sexe       = '%s',
		email      = '%s',
		birthCity  = '%s',
		phone      = '%s',
		profession = '%s'
		WHERE id = %d""" % ( i["firstName"], i["lastName"], i["birthday"], i["address"], i["sexe"], i["email"], i["birthCity"], i["phone"], i["profession"], int( i["personId"] ) )

		self.log("8", i["personId"] )
		cur.execute( query )

	def saveOptions(self, params):
		cur = self.db.cursor();

		query = ''
		for (k,v) in params.items():
			query += """UPDATE options SET option_value = %s WHERE option_name = '%s';""" % (v,k)

		cur.execute( query )

	# def saveUsers(self, users):

	# 	cur = self.db.cursor();
	# 	query = 'TRUNCATE TABLE users;'

	# 	for user in users:
	# 		query += """INSERT INTO users (person_id, username, password, role)
	# 				VALUES('%s', '%s', '%s', '%s');""" % (user["person_id"], user["username"], user["password"], user["role"] )

	# 	cur.execute( query )

	def setRecResult(self, stat, eigen_ratio, fisher_ratio, lbph_ratio):
		cur = self.db.cursor();
		query = ''

		if stat < 0:
			field = "unrec"
			if eigen_ratio:
				query += "UPDATE options SET option_value = option_value + 1 WHERE option_name = 'eigenFace_%s';" % field
				query += "UPDATE options SET option_value = option_value - 1 WHERE option_name = 'eigenFace_ign';"
			if fisher_ratio:
				query += "UPDATE options SET option_value = option_value + 1 WHERE option_name = 'fisherFace_%s';" % field
				query += "UPDATE options SET option_value = option_value - 1 WHERE option_name = 'fisherFace_ign';"
			if lbph_ratio:
				query += "UPDATE options SET option_value = option_value + 1 WHERE option_name = 'LBPHFace_%s';" % field
				query += "UPDATE options SET option_value = option_value - 1 WHERE option_name = 'LBPHFace_ign';"
		elif stat == 0:
			field = "ign"
			query += "UPDATE options SET option_value = option_value + 1 WHERE option_name LIKE '%Face_ign';"
		else:
			field = "rec"
			if eigen_ratio:
				query += "UPDATE options SET option_value = option_value + 1 WHERE option_name = 'eigenFace_%s';" % field
				query += "UPDATE options SET option_value = option_value - 1 WHERE option_name = 'eigenFace_ign';"
			if fisher_ratio:
				query += "UPDATE options SET option_value = option_value + 1 WHERE option_name = 'fisherFace_%s';" % field
				query += "UPDATE options SET option_value = option_value - 1 WHERE option_name = 'fisherFace_ign';"
			if lbph_ratio:
				query += "UPDATE options SET option_value = option_value + 1 WHERE option_name = 'LBPHFace_%s';" % field
				query += "UPDATE options SET option_value = option_value - 1 WHERE option_name = 'LBPHFace_ign';"


		print query;
		cur.execute( query )



	def log(self, op_id, op_data):
		cur = self.db.cursor();
		now_str = str( datetime.datetime.now().strftime('%Y/%m/%d %H:%M:%S') )

		query = """ INSERT INTO logs (user_id, op_id, op_data, op_date)
				VALUES(%s, %s, '%s', NOW()) """ % (self.user_id, op_id, op_data)
		
		cur.execute( query )


	def get_name(self, personId):
		cur = self.db.cursor();

		query = """SELECT firstName, lastName
		FROM persons
		WHERE id = %s LIMIT 1""" % (personId)

		cur.execute( query )

		if (cur.rowcount):
			row = cur.fetchone()
			return ( row['firstName'], row['lastName'] )
		else:
			return ("","")

	def isAdmin(self, userId):
		cur = self.db.cursor();

		query = """SELECT role
		FROM users
		WHERE id = %s LIMIT 1""" % (user_id)

		cur.execute( query )

		if not cur.rowcount:
			return false

		row = cur.fetchone()
		if row['role'] == "admin":
			return true
		else:
			return false

	def get_default_face(self, personId, (width, height) ):
		# print "GETTING default face for %d" % personId
		cur = self.db.cursor();
		query = """SELECT id
		FROM faces
		WHERE persons_id = %s 
		ORDER BY id ASC LIMIT 1""" % personId

		cur.execute( query )

		if cur.rowcount:
			row = cur.fetchone()


			file_path = os.path.join( 'db', str(personId), str(row['id'])+'.color.png' )
			# print file_path
			def_path = os.path.join( 'db', 'default.png' )
			if not os.path.exists( file_path ):
				file_path = def_path;

			img = cv2.imread( file_path ); # size 96 x 96
			img_resized = cv2.resize( img, (width, height) )
			_, buf = cv2.imencode( '.png', img_resized )


			return buf.tostring(order=None)
		else:
			print "NO default face for user: %d" % personId
			return ""


	def get(self, personId):
		cur = self.db.cursor();

		query = """SELECT *, YEAR(birthday) as year, MONTH(birthday) as month, DAY(birthday) as day
		FROM persons
		WHERE id = %d LIMIT 1""" % int(personId)

		cur.execute( query )
		row = cur.fetchone()

		if (cur.rowcount > 0):
			self.log( "11", row["id"])

			return ( str_oneDict(row) )
		else:
			return {}

	def read_faces(self):
		cur = self.db.cursor();
		new_faces, new_labels = [], [] # no faces for now

		query = """SELECT * FROM faces"""

		cur.execute( query )
		rows = cur.fetchall()

		folder_path = "db"
		for row in rows:
			face_path = os.path.join( folder_path, str(row["persons_id"]), "%s.gray.png" % str(row["id"]) )
			if not os.path.exists( face_path ):
				return

			new_faces.append( cv2.imread( face_path, cv2.IMREAD_GRAYSCALE ) )
			new_labels.append( row["persons_id"] )

		return new_faces, new_labels


	def auth(self, username, password, personId):
		cur = self.db.cursor();

		query_users = "SELECT count(*) as nbUsers FROM users"
		cur.execute( query_users )
		row = cur.fetchone()

		if row["nbUsers"] == 0:
			print "nbUsers = 0"
			return {"id": -2, "role": "admin", "firstName": "admin"} # temp admin account

		query = """SELECT users.id, persons.firstName, users.role
		FROM users
		JOIN persons
		ON users.person_id = persons.id
		WHERE username = '%s' AND password = MD5('%s') OR person_id = %d LIMIT 1""" % (username, password, personId)
		
		cur.execute( query )

		if cur.rowcount:
			row = cur.fetchone()
			print "userfound %s %s %d" % (username, password, personId)
			return str_oneDict( row )
		
		# print "wrong credentials"
		return {"id": 0, "role": 0, "firstName": 0}
		 

	def getStats(self):
		cur = self.db.cursor();

		query = """SELECT option_name, option_value
				FROM options WHERE option_name LIKE '%rec' OR option_name LIKE '%unrec' OR option_name LIKE '%ign' """

		cur.execute( query )
		rows = cur.fetchall()

		stats = dict( (dic['option_name'],dic['option_value']) for dic in rows)

		query = """SELECT count(id) as personsCount
				FROM persons"""

		cur.execute( query )
		row = cur.fetchone()

		stats["personsCount"] = row["personsCount"]
		
		return stats

	def get_params(self):
		cur = self.db.cursor();

		query = """SELECT option_name, option_value
				FROM options"""

		cur.execute( query )
		rows = cur.fetchall()

		params = dict( (dic['option_name'],dic['option_value']) for dic in rows) 
		return params

	def get_users(self):
		cur = self.db.cursor();

		query = """SELECT id, person_id, username, role
				FROM users"""

		cur.execute( query )
		rows = cur.fetchall()

		return str_dict( rows )

		"""
        op["9"] = "Recherche par image";
        op["10"] = "Consulation";

		"""
	def sendUser(self, userId,username, password, personId, role ):
		cur = self.db.cursor();

		# print (userId, username, password, personId, role )
		if userId == '*':
			query = """INSERT INTO users (username, password, person_id, role)
				VALUES('%s', MD5('%s'), %d, '%s');
				
				INSERT INTO logs (user_id, op_id, op_data, op_date)
				VALUES('%s', '3', LAST_INSERT_ID(), NOW() )

				""" % (username, password, personId, role, self.user_id )
		else:
			updatePassStr = ''
			if len(password) > 3:
				updatePassStr = ", password = MD5('%s')" % password

			query = """UPDATE users
			SET  username = '%s' %s, person_id = %d, role = '%s'
			WHERE id = %d""" % (username, updatePassStr, personId, role, int(userId) )
		
			self.log( "4", userId )

		cur.execute( query )


	def deleteUser(self, userId):

		if userId != self.user_id:
			cur = self.db.cursor();
			query = """DELETE FROM users WHERE id = %d""" % int( userId )
			cur.execute( query )

			self.log( "5", userId )


	def get_log(self, username, op_id, op_data, op_date, page):

		cur = self.db.cursor();

		logs_per_page = 20;
		dep = ( page - 1 ) * logs_per_page;

		username_str, op_id_str, op_data_str, op_date_str = '1=1', '1=1', '1=1', '1=1'

		if username:
			username_str = 'user_id IN ( SELECT id FROM users WHERE username LIKE  %s )' % ( "'%" + username + "%'" )

		if op_id:
			op_id_str = 'op_id = %s' % op_id

		if op_data:
			op_data_str = 'op_data LIKE %s' % ("'%" + op_data + "%'")

		if op_date:
			op_date_str = "op_date LIKE '%s'" % ( str(op_date)  + '%')

		query = """SELECT users.username, logs.op_id, logs.op_data, logs.op_date
				FROM logs
				JOIN users ON users.id = logs.user_id
				WHERE %s AND %s AND %s AND %s
				ORDER BY op_date DESC
				LIMIT %s, %s""" % (username_str, op_id_str, op_data_str, op_date_str, dep, logs_per_page )

		cur.execute( query )
		rows = cur.fetchall()


		return str_dict( rows )


