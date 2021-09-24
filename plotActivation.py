#!/usr/bin/python

import sys
import numpy as np 
import matplotlib.pyplot as plt

def toNumpyMatrix(matrix):
   height = len(matrix)
   width = len(matrix[0])
   numpyMatrix = np.zeros((width, height))
   for i in range(height):
      for j in range(width):
         numpyMatrix[j][i] = matrix[i][j]
   return numpyMatrix

def readFile(filename):
   inputFile = open(filename, 'r')
   matrix = []

   inputFile.seek(0)
   for line in inputFile:
      line = line.strip()
      numbers  = line.split()
      row = []
      for number in numbers:
         row.append(float(number))
      matrix.append(row)
   inputFile.close()

   return matrix

for arg in sys.argv[1:]:
    matrix = readFile(arg)
    numpyMatrix = toNumpyMatrix(matrix)
    numpyMatrix.transpose()
    plt.imshow(numpyMatrix, cmap=plt.cm.gray, aspect='auto', interpolation='nearest')
    plt.savefig(arg + ".pdf")
    plt.clf()
