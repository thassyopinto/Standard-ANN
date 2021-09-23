/*
 * ANN_NeuralNetwork.hpp
 *
 * Revision: October 2018
 * Author: Thassyo Pinto - thassyo@ieee.org
 */

#ifndef ANN_NEURALNETWORK_HPP_
#define ANN_NEURALNETWORK_HPP_

// Standard libraries
#include <vector>
#include <fstream>

// Local libraries
#include "Misc_Random.hpp"
#include "ANN_Neuron.hpp"
#include "ANN_Connection.hpp"

// An artificial neural network class..
template<typename Neuron_t = Neuron, typename Connection_t = Connection>
class NeuralNetwork{
  public:
    // Initializes the network with the supplied number of neurons.
    NeuralNetwork(size_t numberOfNeurons = 0){
      for(int i = 0; i < numberOfNeurons; i++){
        this->addNeuron();
      }
    }

    // Sets the minimum weight that a connection may get due to randomization or mutation.
    void setMinWeight(double minWeight){
      _minWeight = minWeight;
    }

    // Sets the maximum weight that a connection may get due to randomization or mutation.
    void setMaxWeight(double maxWeight){
      _maxWeight = maxWeight;
    }

    // Returns a reference to the vector of neurons of this network.
    std::vector<Neuron_t>& getNeurons(){
      return _neurons;
    }

    // Returns a reference to the vector of connections of this network.
    std::vector<Connection_t>& getConnections(){
      return _connections;
    }

    // Sets the activation value of the indicated neuron.
    void setValue(size_t neuronIndex, double value){
      _neurons[neuronIndex].setValue(value);
    }

    // Returns the activation value of the indicated neuron.
    double getValue(size_t neuronIndex){
      if(neuronIndex >= _neurons.size()){
        std::cerr << "Index out of bounds! Index: " << neuronIndex << " size: " << _neurons.size() << std::endl;
      }
      return _neurons[neuronIndex].getValue();
    }

    // Sets the bias of the indicated neuron.
    void setBias(size_t neuronIndex, double bias){
      _neurons[neuronIndex].setBias(bias);
    }

    // Returns the bias of the indicated neuron.
    double getBias(size_t neuronIndex){
      if(neuronIndex >= _neurons.size()){
        std::cerr << "Index out of bounds! Index: " << neuronIndex << " size: " << _neurons.size() << std::endl;
      }
      return _neurons[neuronIndex].getBias();
    }

    // Sets the amount of incoming potential of the indicated neuron.
    void setIncoming(size_t neuronIndex, double incoming){
      _neurons[neuronIndex].setIncoming(incoming);
    }

    // Adds a neuron to this network.
    void addNeuron(){
      _neurons.push_back(Neuron());
    }

    // Adds a connection between the two indicated neurons.
    void addConnection(size_t sourceIndex, size_t targetIndex, double weight = 0.0){
      _connections.push_back(Connection(sourceIndex, targetIndex, weight));
    }


    // Performs one step of network activation.
    void step(){
      for(int i = 0; i < _neurons.size(); i++){
        this->setIncoming(i, 0.0);
      }
      for(int i = 0; i < _neurons.size(); i++){
        for(int j = 0; j < _connections.size(); j++){
          if(_connections[j].getTarget() == i){
            _neurons[i].updateIncoming(this->getValue(_connections[j].getSource()) * _connections[j].getWeight());
          }
        }
      }
      for(int i = 0; i < _neurons.size(); i++){
        _neurons[i].propagate();
      }
    }

    // Assigns random weights to each connection.
    void randomize(){
      for(int i = 0; i < _neurons.size(); i++){
        this->setValue(i, randDouble(0.0, 1.0));
        this->setBias(i, randDouble(_minWeight, _maxWeight));
        for(int j = 0; j < _neurons.size(); j++){
          this->addConnection(i, j, randDouble(_minWeight, _maxWeight));
        }
      }
    }

    // Writes the current activation of the network to the output stream.
    void logActivation(std::ofstream& activationFile){
      if(activationFile.is_open()){
        activationFile << this->getValue(0);
        for(int i = 1; i < _neurons.size(); i++){
          activationFile << " " << this->getValue(i);
        }
        activationFile << "\n";
      }
    }

    // Resets all neurons in the network.
    void reset(){
      for(int i = 0; i < _neurons.size(); i++){
        _neurons[i].reset();
      }
    }

    void run(size_t numberOfSteps, std::string activationFileName = ""){
      std::ofstream actFile;
      if(activationFileName != "") actFile.open(activationFileName);
      for(int i = 0; i < numberOfSteps; i++){
        this->step();
        this->logActivation(actFile);
      }
      actFile.close();
    }

  protected:
    //Vectors containing neurons and connections.
    std::vector<Neuron_t> _neurons;
    std::vector<Connection_t> _connections;

    // Weight constraints.
    double _minWeight;
    double _maxWeight;
};

// Convenience function for writing network connections to a file-stream.
std::ostream& operator<<(std::ostream& is, NeuralNetwork<Neuron, Connection>& obj){
  std::vector<Neuron> neurons = obj.getNeurons();
  std::vector<Connection> connections = obj.getConnections();

  //Write neurons
  is << neurons.size() << " ";
  is << connections.size() << " ";
  for(size_t i=0; i<neurons.size(); ++i){
    is << neurons[i].getBias() << " ";
  }

  //Write connections
  for(size_t i=0; i<connections.size(); ++i){
    is << connections[i].getSource() << " " << connections[i].getTarget() << " "<< connections[i].getWeight()<< " ";
  }
  return is;
}

#endif /* ANN_NEURALNETWORK_HPP_ */
