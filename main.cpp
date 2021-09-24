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
#include "Misc_Random.hpp"

double VAL_MIN = 0.0;
double VAL_MAX = 1.0;
double WGT_MIN = -1.0;
double WGT_MAX = 1.0;

int main(int argc, char* argv[]) {

    seed();

    typedef NeuralNetwork<> ann_t;

    ann_t myNetwork(10);

    myNetwork.setMinWeight(WGT_MIN);
    myNetwork.setMaxWeight(WGT_MAX);
    myNetwork.reset();
    myNetwork.initialize(randDouble(VAL_MIN, VAL_MAX), 0.0);
    myNetwork.randomize();

    std::ofstream networkFile("standard_ANN.csv");
    networkFile << myNetwork << "\n";
    networkFile.close();

    myNetwork.run(50, "standard_ANN_activation.csv");

    return 0;
}
