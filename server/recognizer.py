import cv2
import os
import hashlib
import database as db
import numpy as np

class Recognizer():
	"""Python Recognizer"""
	def __init__(self):
		print "Recognizer instantiated !"


		self.lbphModel = cv2.createLBPHFaceRecognizer();
		self.eigenModel = cv2.createEigenFaceRecognizer();
		self.fisherModel = cv2.createFisherFaceRecognizer();

		self.model_path        = "models"
		self.lbph_model_path   = os.path.join( self.model_path, "lbphModel.xml" )
		self.eigen_model_path  = os.path.join( self.model_path, "eigenModel.xml" )
		self.fisher_model_path = os.path.join( self.model_path, "fisherModel.xml" )

		self.load_models();

		self.new_faces, self.new_labels = [], []

		self.db = db.Database()


	def addPersonFaces(self, faces, person_id):
		print "add [%d] faces" % len( faces )

		for face in faces:
			self.add_face(face, person_id)

	def preprocess(self, color):
		gray = ''
		if len( color ) != 0:
			gray = cv2.cvtColor(color, cv2.COLOR_BGR2GRAY)
			gray = cv2.resize(gray, (48, 48) )
			# gray = cv2.GaussianBlur( gray, (3,3), 0)
		return gray


	def add_face(self, color, person_id):
		if person_id <= 0:
			person_id = 1

		gray = self.preprocess( color );
		# img = cv2.imdecode(buf)
		

		face_id = self.db.add_face( person_id )
		# face_id = hashlib.md5( color ).hexdigest()

		self.new_faces.append( gray );
		self.new_labels.append( person_id )

		#create folder for person_id if does not exists
		folder_path = "db/%d" % person_id
		if not os.path.exists( folder_path ):
			os.makedirs( folder_path )

		color_file_path = os.path.join( folder_path, "%d.color.png" % face_id );
		gray_file_path = os.path.join( folder_path, "%d.gray.png" % face_id );
		if not os.path.exists( color_file_path ) and not os.path.exists( gray_file_path ):
			cv2.imwrite( color_file_path, color, [ cv2.IMWRITE_PNG_COMPRESSION, 0 ] ) # 0 compression level PNG
			cv2.imwrite( gray_file_path, gray, [ cv2.IMWRITE_PNG_COMPRESSION, 0 ])


	def recognize(self, color):

		gray = self.preprocess( color );

		[p_label, p_confidence]           = [-1, 0]
		[eigen_label, eigen_confidence]   = [-1, 0]
		[fisher_label, fisher_confidence] = [-1, 0]
		[lbph_label, lbph_confidence]     = [-1, 0]

		eigen_threashold = self.db.get_threshold("eigenFace_threshold")
		fisher_threashold = self.db.get_threshold("fisherFace_threshold")
		lbph_threashold = self.db.get_threshold("LBPHFace_threshold")

		eigen_ratio, fisher_ratio, lbph_ratio = 0,0,0

		actual_method = self.db.get_actual_method()
		print "actual_method: %s" % actual_method


		if os.path.exists( self.eigen_model_path ):
			[eigen_label, eigen_confidence] = self.eigenModel.predict( gray )
			eigen_ratio = 100 - ( eigen_confidence * 100 ) / int( eigen_threashold )
			print "eigenface said: %d (%d %d percent)" % (eigen_label, eigen_confidence, eigen_ratio)

		if os.path.exists( self.fisher_model_path ):
			[fisher_label, fisher_confidence] = self.fisherModel.predict( gray )
			fisher_ratio = 100 - ( fisher_confidence * 100 ) / int( fisher_threashold )
			print "fisherface said: %d (%d %d percent)" % (fisher_label, fisher_confidence, fisher_ratio)
	
		if os.path.exists( self.lbph_model_path ):
			[lbph_label, lbph_confidence] = self.lbphModel.predict( gray )
			lbph_ratio = 100 - ( lbph_confidence * 100 ) / int( lbph_threashold )
			print "lbph said: %d (%d %d percent)" % (lbph_label, lbph_confidence, lbph_ratio)

		# if actual_method == 0:
		# 	[p_label, p_confidence] = [eigen_label, eigen_confidence]
		# elif actual_method == 1 and os.path.exists( self.fisher_model_path ):
		# 	[p_label, p_confidence] = [fisher_label, fisher_confidence]
		# else:
		# 	[p_label, p_confidence] = [lbph_label, lbph_confidence]

		if ( int( eigen_confidence ) > int( eigen_threashold ) ):
			eigen_ratio = 0
			eigen_label = -1

		if ( int( fisher_confidence ) > int( fisher_threashold ) ):
			fisher_ratio = 0
			fisher_label = -1

		if ( int( lbph_confidence ) > int( lbph_threashold ) ):
			lbph_ratio = 0
			lbph_label = -1

		d = {eigen_label: eigen_ratio/100, fisher_label: fisher_ratio/100, lbph_label: lbph_ratio/100}
		d[eigen_label] += 1
		if os.path.exists( self.fisher_model_path ):
			d[fisher_label] += 1
		d[lbph_label] += 1

		if actual_method == 0:
			d[eigen_label] += 0.5
		elif actual_method == 1 and os.path.exists( self.fisher_model_path ):
			d[fisher_label] += 0.5
		else:
			d[lbph_label] += 0.5

		max_key = 0;
		max_value = -1;

		if d.has_key(-1):
			del d[-1]

		for k,v in d.items():
			if (v > max_value):
				max_key = k
				max_value = v

		if ( max_key > -1 ):
			p_label = max_key

		self.db.log( "10", p_label )

		# if p_label <= -1:
		# 	self.db.setRecResult(0, eigen_ratio, fisher_ratio, lbph_ratio)
		# 	self.db.setRecResult(-1, eigen_ratio, fisher_ratio, lbph_ratio)
		# else:
		# 	self.db.setRecResult(0, eigen_ratio, fisher_ratio, lbph_ratio) 

		print "Decision => %d" % p_label

		return p_label, int( eigen_ratio ), int( fisher_ratio ), int( lbph_ratio )


	def clear_faces(self):
		self.new_labels = []
		self.new_faces = []


	def faces_count(self):
		if len( self.new_labels ) != len( self.new_labels ):
			self.clear_faces();
		return len( self.new_labels )


	def update_models(self):
		print "updating model with %d new Faces" % self.faces_count()
		if self.faces_count() > 0:

			# update LBPH
			if not os.path.exists( self.lbph_model_path ):
				self.lbphModel.train( np.asarray( self.new_faces ), np.asarray( self.new_labels ) )
			else:
				self.lbphModel.update( np.asarray( self.new_faces ), np.asarray( self.new_labels ) )

			# train eigenface and fisherface again
			faces, labels = self.db.read_faces()
			print "%d faces loaded" % len( faces )

			self.new_faces += faces
			self.new_labels += labels
			print "%d faces to train" % len( self.new_faces )
			if len( list(set(self.new_labels)) ) > 1:
				self.fisherModel.train( np.asarray( self.new_faces ), np.asarray( self.new_labels ) )
			
			self.eigenModel.train( np.asarray( self.new_faces ), np.asarray( self.new_labels ) )
			
			self.save_models()


	def save_models(self):
		
		if not os.path.exists( self.model_path ):
			os.makedirs( self.model_path )

		self.lbphModel.save( self.lbph_model_path )
		self.eigenModel.save( self.eigen_model_path )
		if len( list(set(self.new_labels)) ) > 1:
			self.fisherModel.save( self.fisher_model_path )
		self.clear_faces()
	


	def load_models(self):
		if os.path.exists( self.lbph_model_path ):
			self.lbphModel.load( self.lbph_model_path )

		if os.path.exists( self.eigen_model_path ):
			self.eigenModel.load( self.eigen_model_path )

		if os.path.exists( self.fisher_model_path ):
			self.fisherModel.load( self.fisher_model_path )
		
		print "lbph model loaded"