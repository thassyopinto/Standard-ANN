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

// An artificial neural network class.
template<typename Neuron_t = Neuron, typename Connection_t = Connection>
class NeuralNetwork{
  public:
    // Builds a fully connected network with the supplied number of neurons.
    NeuralNetwork(size_t numberOfNeurons = 0){
      // Creates neurons.
      for(int i = 0; i < numberOfNeurons; i++){
        this->addNeuron();
      }
      // Creates connections.
      for(int i = 0; i < _neurons.size(); i++){
        for(int j = 0; j < _neurons.size(); j++){
          this->addConnection(i, j, 0.0);
        }
      }
      // Adds the index of all connections.
      for(int i = 0; i < _connections.size(); i++){
        this->addIncoming(this->getTarget(i), i);
        this->addOutgoing(this->getSource(i), i);
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

    // Updates the current incoming potential of the indicated neuron.
    void updateIncoming(size_t neuronIndex, double update){
      _neurons[neuronIndex].updateIncoming(update);
    }

    // Updates the activation value of the indicated neuron.
    void propagateNeuron(size_t neuronIndex){
      _neurons[neuronIndex].propagate();
    }

    // Resets the state of the indicated neuron.
    void resetNeuron(size_t neuronIndex){
      _neurons[neuronIndex].reset();
    }

    // Adds the index of an incoming connection to the indicated neuron.
    void addIncoming(size_t neuronIndex, size_t incomingIndex){
      _neurons[neuronIndex].addIncoming(incomingIndex);
    }

    // Adds the index of an outgoing connection to the indicated neuron.
    void addOutgoing(size_t neuronIndex, size_t outgoingIndex){
      _neurons[neuronIndex].addOutgoing(outgoingIndex);
    }

    // Sets the weight of the indicated connection.
    void setWeight(size_t connectionIndex, double weight){
        _connections[connectionIndex].setWeight(weight);
    }

    // Returns the weight of the indicated connection.
    double getWeight(size_t connectionIndex){
      if(connectionIndex >= _connections.size()){
        std::cerr << "Index out of bounds! Index: " << connectionIndex << " size: " << _connections.size() << std::endl;
      }
      return _connections[connectionIndex].getWeight();
    }

    // Returns the source of the indicated connection.
    size_t getSource(size_t connectionIndex){
      if(connectionIndex >= _connections.size()){
        std::cerr << "Index out of bounds! Index: " << connectionIndex << " size: " << _connections.size() << std::endl;
      }
      return _connections[connectionIndex].getSource();
    }

    // Returns the target of the indicated connection.
    size_t getTarget(size_t connectionIndex){
      if(connectionIndex >= _connections.size()){
        std::cerr << "Index out of bounds! Index: " << connectionIndex << " size: " << _connections.size() << std::endl;
      }
      return _connections[connectionIndex].getTarget();
    }

    // Adds a neuron to this network.
    void addNeuron(){
      _neurons.push_back(Neuron());
    }

    // Adds a connection between the two indicated neurons.
    void addConnection(size_t sourceIndex, size_t targetIndex, double weight = 0.0){
      _connections.push_back(Connection(sourceIndex, targetIndex, weight));
    }

    // Performs one update of network activation.
    void update(){
      // Resets the incoming values of neurons to 0.
      for(int i = 0; i < _neurons.size(); i++){
        this->setIncoming(i, 0.0);
      }
      // Updates the incoming values of all neurons by iterating over all their incoming connections.
      for(int i = 0; i < _neurons.size(); i++){
        std::vector<size_t> incomingConnections = _neurons[i].getIncomingIndices();
        for(int j = 0; j < incomingConnections.size(); j++){
          double updateValue = this->getValue(this->getSource(incomingConnections[j])) * this->getWeight(incomingConnections[j]);
          this->updateIncoming(i, updateValue);
        }
      }
      // Propagates the incoming value to become the current activation of that neuron.
      for(int i = 0; i < _neurons.size(); i++){
        this->propagateNeuron(i);
      }
    }

    // Randomizes the network.
    void randomize(){
      // Assigns each neuron, uniform randomly, a bias in [_minWeight, _maxWeight]
      for(int i = 0; i < _neurons.size(); i++){
        this->setBias(i, randDouble(_minWeight, _maxWeight));
      }
      // Assigns each connection, uniform randomly, a weight in [_minWeight, _maxWeight]
      for(int i = 0; i < _connections.size(); i++){
        this->setWeight(i, randDouble(_minWeight, _maxWeight));
      }
    }

    // Initializes the network with specific values and weights.
    void initialize(double initValue, double initWeight){
      for(int i = 0; i < _neurons.size(); i++){
        this->setValue(i, initValue);
      }
      for(int i = 0; i < _connections.size(); i++){
        this->setWeight(i, initWeight);
      }
    }

    // Resets all neurons in the network.
    void reset(){
      for(int i = 0; i < _neurons.size(); i++){
        this->resetNeuron(i);
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

    // Run the neural network for a given number of updates.
    void run(size_t numberOfUpdates, std::string activationFileName = ""){
      std::ofstream actFile;
      if(activationFileName != "") actFile.open(activationFileName);
      for(int i = 0; i < numberOfUpdates; i++){
        this->update();
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

  is << neurons.size() << " ";
  is << connections.size() << " ";

  // Write neurons to file.
  for(size_t i=0; i<neurons.size(); ++i){
    is << neurons[i].getBias() << " ";
  }

  // Write connections to file.
  for(size_t i=0; i<connections.size(); ++i){
    is << connections[i].getSource() << " " << connections[i].getTarget() << " "<< connections[i].getWeight()<< " ";
  }
  return is;
}

#endif /* ANN_NEURALNETWORK_HPP_ */
