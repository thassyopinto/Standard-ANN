This is a Standard ANN library.

**Activation Function**

<img src="https://render.githubusercontent.com/render/math?math=a^{t%2B1}_{i}=\sigma {(\displaystyle \sum_{j=1}^{n} w_{ij} a^{t}_{j})}">

Possible activation functions:
- linear: identity, except that it truncates values to lie in [-1, 1]
- sin: calculates the sine-wave (f(x)=sin(x))
- gaussian: calculates a gaussian (f(x)=e^(-x*x)), scaled to lie in [-1, 1]
- sigmoid: calculates the sigmoid (f(x)=tanh(x*lambda))

<p align="center">
  <img src="standard_ANN.csv.png" />
</p>
