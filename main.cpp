/*
 * Main.cpp
 *
 * Revision: October 2018
 * Author: Thassyo Pinto - thassyo@ieee.org
 *
 * This is the main file for the Standard ANN.
 */

// Standard libraries
#include <sstream>

// Local libraries
#include "ANN_NeuralNetwork.hpp"

int main(int argc, char* argv[]) {

    seed();

    typedef NeuralNetwork<> ind_t;

    ind_t myNetwork (10);

    myNetwork.setMinWeight(-1.0);

    myNetwork.setMaxWeight(1.0);

    myNetwork.reset();

    myNetwork.randomize();

    std::ofstream networkFile("standard_ANN.csv");
    networkFile << myNetwork << "\n";
    networkFile.close();

    myNetwork.run(50, "standard_ANN_activation.csv");

    return 0;
}
