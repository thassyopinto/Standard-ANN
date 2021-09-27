#!/usr/bin/python

import sys
import numpy as np
import matplotlib.pyplot as plt
import math

numbersPerNeuron = 1
numbersPerConnection = 3

ymin = -1.2
ymax = 1.2
xmin = -1.2
xmax = 1.2
scale = 2
width = (xmax - xmin)*scale
height = (ymax - ymin)*scale
xmargin = (xmax - xmin - 1)/8

inhibColor = [1,0,0]
excitColor = [0,0,1]

arrowDist = 0.15
neuronRadius = 0.1
arrowWidth = 0.03
arrowLength = 0.05

class NetworkParser:
   def __init__(self, split_line):
      self.current_index = 0
      self.split_line = split_line

   def next_number(self):
      i = self.current_index
      self.current_index += 1
      return self.split_line[i]


def readFile(filename):
   neurons = []
   connections = []

   inputFile = open(filename, 'r')

   for line in inputFile:
      line = line.strip()
      numbers  = line.split()

   np = NetworkParser(numbers)

   nbOfNeurons = int(np.next_number())
   nbOfConnections = int(np.next_number())

   for i in range(nbOfNeurons):
      neuron = []
      for i in range(numbersPerNeuron):
         neuron.append(float(np.next_number()))
      neurons.append(neuron)

   for i in range(nbOfConnections):
      connection = []
      connection.append(int(np.next_number()))
      connection.append(int(np.next_number()))
      connection.append(float(np.next_number()))
      connections.append(connection)

   inputFile.close()

   return neurons, connections

def plotNetwork(neurons, connections):
   ax = plt.axes()
   ax.axes.xaxis.set_visible(False)
   ax.axes.yaxis.set_visible(False)

   positions = [(0,0)] * len(neurons)
   angle = 0.0
   angleUpdate = 2 * np.pi / len(neurons)

   for neuronIndex in range(len(neurons)):
      neuron = neurons[neuronIndex]
      bias = neuron[0]
      x = np.sin(angle)
      y = np.cos(angle)
      angle = angle + angleUpdate
      positions[neuronIndex] = (x,y)

      #Draw the neuron outline
      circ = plt.Circle((x,y), radius=neuronRadius, facecolor=[1,1,1], edgecolor=[0,0,0], fill=True, linewidth=1, zorder=2)
      ax.add_patch(circ)

      #Determine color based on bias
      if bias < 0.0:
         currentColor = inhibColor
      elif bias > 0.0:
         currentColor = excitColor
      else: continue

      #Determine size based on bias
      w = ((0.8*abs(bias))+1)*0.05

      #Draw the bias
      if w > 0.05:
          circ = plt.Circle((x,y), radius=w, color=currentColor, fill=True, zorder=3)
          ax.add_patch(circ)


   for connectionIndex in range(len(connections)):
      connection = connections[connectionIndex]
      source, target, weight = connection
      x1, y1 = positions[source]
      x2, y2 = positions[target]

      #Determine color based on weight
      if weight < 0.0:
         currentColor = inhibColor
      elif weight > 0.0:
         currentColor = excitColor
      else: continue

      #Determine line weight based on weight
      w = (2*abs(weight))+0.2

      #If the target equals the source, draw a circle; otherwise draw an arrow
      if source == target:
         x_offset = neuronRadius
         y_offset = neuronRadius
         if x1 < 0: x_offset *= -1
         if y1 < 0: y_offset *= -1
         circ = plt.Circle((x1+x_offset, y1+y_offset), radius=neuronRadius, color=currentColor, fill=False, linewidth=w, zorder=1)
         ax.add_patch(circ)
      else:
         xdiff = x2 - x1
         ydiff = y2 - y1
         length = math.sqrt(xdiff*xdiff + ydiff*ydiff)
         xratio = xdiff/length
         yratio = ydiff/length
         xdiff -= xratio*arrowDist
         ydiff -= yratio*arrowDist
         arr = plt.arrow(x1, y1, xdiff, ydiff, head_width=arrowWidth, head_length=arrowLength, color=currentColor, linewidth=w, zorder=1)
         ax.add_patch(arr)



i=0
for arg in sys.argv[1:]:
    plt.figure(i, figsize=(width,height))
    neurons, connections = readFile(arg)
    plotNetwork(neurons, connections)
    plt.ylim([ymin, ymax])
    plt.xlim([xmin, xmax])
    plt.savefig(arg + ".png")
    plt.clf()
    i+=1
