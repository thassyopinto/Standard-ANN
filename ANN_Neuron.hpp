/*
 * ANN_Neuron.hpp
 *
 * Revision: October 2018
 * Author: Thassyo Pinto - thassyo@ieee.org
 */

#ifndef ANN_NEURON_HPP_
#define ANN_NEURON_HPP_

// Standard libraries
#include <cmath>

// A neuron class for neural networks.
class Neuron{
  public:
    // Constructor. Creates a new neuron with the indicated number of incoming connection.
    // The current and new activations are initialized to zero.
    Neuron():
      _value(0),
      _incoming(0),
      _bias(0),
      _lambda(5.0){
    }

    // Return the current activation value of this neuron.
    double getValue(){
      return _value;
    }

    // Sets the current value.
    void setValue(double value){
      _value = value;
    }

    // Returns the current amount of incoming potential.
    const double& getIncoming(){
      return _incoming;
    }

    // Sets the amount of incoming potential.
    void setIncoming(double incoming){
      _incoming = incoming;
    }

    // Adds the supplied value to the current incoming potential.
    void updateIncoming(double update){
      _incoming += update;
    }

    // Returns the bias of this neuron.
    double getBias(){
      return _bias;
    }

    // Sets the bias of this neuron.
    void setBias(double bias){
      _bias = bias;
    }

    // Adds the index of an incoming connection.
    void addIncoming(size_t incoming){
      _incomingIndices.push_back(incoming);
    }

    // Adds the index of an outgoing connection.
    void addOutgoing(size_t outgoing){
      _outgoingIndices.push_back(outgoing);
    }

    // Returns the vector of incoming connection indices.
    const std::vector<size_t>& getIncomingIndices(){
      return _incomingIndices;
    }

    // Returns the vector of outgoing connection indices.
    const std::vector<size_t>& getOutgoingIndices(){
      return _outgoingIndices;
    }

    // Resets the state of this neuron.
    void reset(){
      _value = 0;
      _incoming = 0;
    }

    // Updates the activation value based on the incoming potential and the bias.
    void propagate(){
        _value = std::tanh(_incoming + _bias) * _lambda;
    }

  protected:
    // ANN attributes
    std::vector<size_t> _incomingIndices;
    std::vector<size_t> _outgoingIndices;
    double _value;
    double _incoming;
    double _bias;
    double _lambda;
};

#endif /* ANN_NEURON_HPP_ */
