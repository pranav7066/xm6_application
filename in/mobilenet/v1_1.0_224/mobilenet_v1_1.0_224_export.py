"""
To append aux node to already frozen graph
"""
import sys
sys.path.append('D:/CEVA/CDNN_latest/frameworks/DeepLearning/CDNN/Trunk/CDNNBin/exe')

import tensorflow as tf
from tensorflow_export_edited import export_graph
from tensorflow.python.platform import gfile


def load_graph(frozen_graph_filename):
	# We load the protobuf file from the disk and parse it to retrieve the 
	# unserialized graph_def
	with tf.gfile.GFile(frozen_graph_filename, "rb") as f:
		graph_def = tf.GraphDef()
		graph_def.ParseFromString(f.read())

	# Then, we import the graph_def into a new Graph and returns it 
	with tf.Graph().as_default() as graph:
		tf.import_graph_def(graph_def, name="")
	return graph


graph = load_graph("mobilenet_v1_1.0_224_frozen_old.pb")
with tf.Session(graph=graph) as sess:
	export_graph(sess, "mobilenet_v1_1.0_224_frozen.pb", "MobilenetV1/Predictions/Softmax")

