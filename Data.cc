#include "Data.hh"

#include <iostream>
#include <fstream>
#include <cassert>
#include <stdexcept>
#include <cmath>

using namespace std;

Data::Data(const std::string& filename) {
  ifstream file(filename);

  // check if file opened correctly
  if (!file.is_open()) {
    throw std::runtime_error("Error could not open file with name: " +
                             filename);
  }

  int size = -1;
  file >> size;

  // read in data from file: m_siz+1 bin edges
  for (int i = 0; i < size + 1; ++i) {
    double edge;
    file >> edge;
    m_bins.push_back(edge);
  }
  // read in data from file: m_siz bin contents
  for (int i = 0; i < size; ++i) {
    double entries;
    file >> entries;
    m_data.push_back(entries);
  }
  // read in errors from file: m_siz bin errors
  for (int i = 0; i < size; ++i) {
    double errors;
    file >> errors;
    m_errors.push_back(errors);
  }
 

  // done! close the file
  file.close();

  assertSizes();
};

void Data::assertSizes() { assert(m_data.size() + 1 == m_bins.size()); }

int Data::checkCompatibility(const Data& in, int n) {
  int j = 0;
  for (int i = 0; i < size(); ++i) {
    double diff = abs(measurement(i)-in.measurement(i));
    double err = sqrt(pow((error(i)),2)+pow((in.error(i)),2));
    if (n*err < diff) {
      j += 1;
    }
  }
  return j;
};

double background(double x) {
  double f_x = 0.005 - 0.00001 * x + 0.08 * exp(-0.015 * x);
  return f_x;
}

double Data::chisquared_ndf(int parameters) {
  double chisum = 0;
  for (int i = 0; i < size(); i++) {
    double delta = (measurement(i) - background(binCenter(i)));
    double chi = pow((delta/error(i)),2);
    chisum += chi;
  }
  double chi2_ndf = chisum / (size() - parameters);
  return chi2_ndf;
}
