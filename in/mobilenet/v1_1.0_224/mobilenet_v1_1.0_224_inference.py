import tensorflow as tf # Default graph is initialized when the library is imported
import os
from tensorflow.python.platform import gfile
import numpy as np
import scipy
from scipy import misc
import struct
import argparse
import cv2

def inferDumpLog(log, inf, dump):
	with tf.Graph().as_default() as graph: # Set default graph as graph
		with tf.Session() as sess:
			# Load the graph in graph_def
			print("load graph")

			# We load the protobuf file from the disk and parse it to retrive the unserialized graph_drf
			model_filename = 'mobilenet_v1_1.0_224_frozen.pb'
			with gfile.FastGFile(model_filename,'rb') as f:
				print("Load Image...")
				# Read the image & get statstics
				image = scipy.misc.imread('input_image.jpg')
				image = image.astype(float)
				resize_image = cv2.resize(image, (224, 224)) / 255.0
				#resize_image = misc.imresize(image,[224, 224]) / 255.0
				resize_image = np.reshape(resize_image, [1, 224, 224, 3])
				# Set mobilenet_v1 graph to the default graph
				graph_def = tf.GraphDef()
				graph_def.ParseFromString(f.read())
				sess.graph.as_default()

				# Import a graph_def into the current default Graph (In this case, the weights are (typically) embedded in the graph)
				tf.import_graph_def(
				graph_def,
				input_map=None,
				return_elements=None,
				name="",
				op_dict=None,
				producer_op_list=None
				)

				# INFERENCE Here
				l_input = graph.get_tensor_by_name('input:0') # Input Tensor
				l_output = graph.get_tensor_by_name('MobilenetV1/Predictions/Softmax:0') # Output Tensor

				#initialize_all_variables
				tf.global_variables_initializer()
				
				if inf:
					# Run mobilenet_v1 model on single image
					Session_out = sess.run( l_output, feed_dict= {l_input:resize_image})
					top5 = tf.nn.top_k(Session_out, 5)
					print("Top5 Probablities...\n")
					ind = sess.run(top5).indices
					val = sess.run(top5).values
					for i in range(0, 5):
						with open('imagenet1000_clsid_to_human.txt') as f:
							for line in f:
								if str(ind[0][i]-1)+':' in line:
									print(line.strip() + "  " + str(val[0][i]))
									break
				if dump:
					# To dump each layers' output
					i = 0
					for op in sess.graph.get_operations():
						# ignore "NoOp, SaveV2 and Aux"
						if (op.type=="NoOp" or op.type == "SaveV2" or op.name == "AuxiliaryData"):
							print ("Skipping %s" % op.name)
							continue
						orig_t = sess.graph.get_tensor_by_name(op.name + ":0")
						if orig_t.get_shape().ndims == 4:
							t = tf.transpose(orig_t, [0, 3, 1, 2])
						else:
							t = orig_t
						# ignore nodes without dimensions
						if (type(t.get_shape().ndims) != int):
							print ("Skipping %s" % op.name)
							continue
						if t is not None:
							t = t.eval(session = sess, feed_dict={l_input:resize_image})
							opName = str(i) + '_' + op.name
							opName = opName.replace("/", "_")
							print("Dumping ", opName)
							if not os.path.isdir("Dump"):
								os.makedirs("Dump/")
							with open('Dump/'+'{}'.format(opName),'wb') as file:
								for val in t.flatten():
									file.write(struct.pack('f', val))
							i += 1
					print("\nDone...")
			if log:
				print("Dumping log...\n")
				#log to visualize the graph in tensorboard
				with gfile.FastGFile(model_filename, 'rb') as f:
					graph_def = tf.GraphDef()
					graph_def.ParseFromString(f.read())
					g_in = tf.import_graph_def(graph_def)
				if not os.path.isdir("log"):
					os.makedirs("log/")
				train_writer = tf.summary.FileWriter("log/")
				train_writer.add_graph(sess.graph)
				print("Done...")
		
if __name__ == '__main__':
	parser = argparse.ArgumentParser()
	parser.add_argument("--log", type=bool, default=False, help="Dump log to visualize the graph in tensorboard. To visualize run this in cmd: tensorboard --logdir=logs/")
	parser.add_argument("--inf", type=bool, default=True, help="To run the inference (Default)")
	parser.add_argument("--dump", type=bool, default=False, help="To Dump layerwise output")
	args = parser.parse_args()
	inferDumpLog(args.log, args.inf, args.dump)