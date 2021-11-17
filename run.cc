#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include <cmath>

#include "Data.hh"

// generic function comparing two values of some type T used later for int and
// double
template <class T>
bool testEqual(const std::string& name, T expected, T real) {
  if (expected != real) {
    std::cout << "(" << name << ": <" << expected << ">"
              << ", " << real << ") ";
    return false;
  }
  return true;
}

bool testReadingSize() {
  std::cout << "testReadingSize...";
  Data datA("testA");
  return testEqual<int>("size", 1, datA.size());
}

bool testReadingMeasurement() {
  std::cout << "testReadingMeasurement...";
  Data datA("testA");
  return testEqual("measurement", 10., datA.measurement(0));
}

bool testReadingBinEdges() {
  std::cout << "testReadingBinEdges...";
  Data datA("testA");
  return testEqual("bin low", 0., datA.binLow(0)) &&
         testEqual("bin high", 1., datA.binHigh(0));
}

bool testReadingErrors() {
  std::cout << "testReadingErrors...";
  Data datA("testA");
  return testEqual("error", 2., datA.error(0));
}

bool testCopyConstructor() {
  std::cout << "testCopyConstructor...";
  Data a("testA");
  Data b(a);
  Data c = a;  // equivalent to Data c(a)
  return testEqual("measurement", 10., b.measurement(0)) &&
         testEqual("measurement", 10., c.measurement(0));
}

void runTests() {
  std::cout << "running tests...\n";
  std::vector<std::function<bool()> > tests(
      {testReadingSize, testReadingMeasurement, testReadingBinEdges,
       testReadingErrors, testCopyConstructor});
  for (auto test : tests)
    std::cout << (test() ? " ok" : " FAILED!") << std::endl;
}

/*

Data calcAverage(const Data& in1, const Data& in2) {
  Data averages;
  for (int i = 0; i < in1.size(); ++i) {
    double w1 = 1/pow(in1.error(i),2);
    double w2 = 1/pow(in2.error(i),2);
    double y = (w1 * in1.measurement(i) + w2 * in2.measurement(i))/(w1+w2);
    averages.push_back(y);
    std::cout << "Average" << i << ": " << y << std::endl;
  }
  return averages;
}

*/

int main() {
  using namespace std;

  cout << "******************************************************" << endl;
  runTests();
  cout << "******************************************************" << endl;
  // create an object which holds data of experiment A
  Data datA("exp_A");
  // create an object which holds data of experiment B
  Data datB("exp_B");
  // create an object which holds data of experiment C
  Data datC("exp_C");
  // create an object which holds data of experiment D
  Data datD("exp_D");

  // here is the data from experiment A
  cout << "bin 27: from " << datA.binLow(27) << " to " << datA.binHigh(27)
       << endl;
  cout << "measurement of experiment A in bin 27: " << datA.measurement(27)
       << endl;
  cout << "measurement of experiment B in bin 27: " << datB.measurement(27)
       << endl;
  cout << "measurement of experiment C in bin 27: " << datC.measurement(27)
       << endl;
  cout << "measurement of experiment D in bin 27: " << datD.measurement(27)
       << endl;  
       
       
  // Check if A and B are compatible
  cout << "|A-B| = " << sqrt(pow((datA.measurement(27)-datB.measurement(27)),2))
       << endl;
  cout << "Error in A " << datA.error(27)
       << endl;
  cout << "Error in B " << datB.error(27)
       << endl;
  cout << "Resulting Error: " << sqrt(pow((datA.error(27)),2)+pow((datB.error(27)),2))
       << endl;
       
  //cout << "first test of compatibility function: " << //datA.checkCompatibility(datB,1)
  //     << endl;
       
  //First test of calcAverage
  Data sum12 = datA + datB;
  cout << "sum12 (sum of datA and datB) in bin 27: " << sum12.measurement(27)
     << endl;
       
  //Output deviations between datasets
  cout << "Checking compatibility of datasets for 2 sigma: " << endl << endl;
  cout << "A and B: " << datA.checkCompatibility(datB,2) << endl; 
  cout << "A and C: " << datA.checkCompatibility(datC,2) << endl; 
  cout << "A and D: " << datA.checkCompatibility(datD,2) << endl; 
  cout << "B and C: " << datB.checkCompatibility(datC,2) << endl; 
  cout << "B and D: " << datB.checkCompatibility(datD,2) << endl; 
  cout << "C and D: " << datC.checkCompatibility(datD,2) << endl << endl; 
  
  cout << "Checking compatibility of datasets for 3 sigma: " << endl << endl;
  cout << "A and B: " << datA.checkCompatibility(datB,3) << endl; 
  cout << "A and C: " << datA.checkCompatibility(datC,3) << endl; 
  cout << "A and D: " << datA.checkCompatibility(datD,3) << endl; 
  cout << "B and C: " << datB.checkCompatibility(datC,3) << endl; 
  cout << "B and D: " << datB.checkCompatibility(datD,3) << endl; 
  cout << "C and D: " << datC.checkCompatibility(datD,3) << endl << endl; 
       
  //Output compatibility with background
  cout << "Calculating Chisquared for datasets with background: " << endl << endl;
  cout << "Chisquared/ndf for A: " << datA.chisquared_ndf(4) << endl;     
  cout << "Chisquared/ndf for B: " << datB.chisquared_ndf(4) << endl; 
  cout << "Chisquared/ndf for C: " << datC.chisquared_ndf(4) << endl; 
  cout << "Chisquared/ndf for D: " << datD.chisquared_ndf(4) << endl << endl;   
  
  //Combining the datasets
  Data sumAB = datA + datB;
  Data sumCD = datC + datD;
  Data sumall = sumAB + sumCD;
  cout << "Test of sumall: " << sumall.error(27) << " and " << sumall.binLow(27) << endl;
  //Data sumall = datA + datB + datC + datD; 
  
  cout << "Chisquared for combined datasets with background: " << 
  sumall.chisquared_ndf(4) << endl << endl;

  return 0;
}
