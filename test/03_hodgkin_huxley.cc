#include "../src/tall.h"

#include <iostream>

int main(int argc, char** argv) {

    // Create a model with 100 variables (for example)
    tall::registry r(100);

    // Add variables for membrane potential (V), gating variables (n, m, h)
    auto V = tall::var(r, "V");     // Membrane potential
    auto n = tall::var(r, "n");     // Potassium activation
    auto m = tall::var(r, "m");     // Sodium activation
    auto h = tall::var(r, "h");     // Sodium inactivation

    // Add parameters: conductances, reversal potentials, capacitance, etc.
    auto gNa  = param(r, "gNa");     // Sodium conductance (mS/cm^2)
    auto gK   = param(r, "gK");      // Potassium conductance (mS/cm^2)
    auto gL   = param(r, "gL");      // Leak conductance (mS/cm^2)
    auto ENa  = param(r, "ENa");     // Sodium reversal (mV)
    auto EK   = param(r, "EK");      // Potassium reversal (mV)
    auto EL   = param(r, "EL");      // Leak reversal (mV)
    auto Cm   = param(r, "Cm");      // Membrane capacitance (uF/cm^2)
    auto Iext = param(r, "Iext");    // External current (uA/cm^2)

    // Assemble a single HH neuron
    auto hh_neuron = tall::assemble(r, {V, n, m, h, gNa, gK, gL, ENa, EK, EL, Cm, Iext});

    // Assemble a network of HH neurons (e.g., 1000 neurons)
    auto hh_network = tall::assemble(r, {hh_neuron}, 1000);

    // Print the model for verification
    tall::print(r);

    return 0;
}