#ifndef DATA_HH
#define DATA_HH

#include <vector>
#include <string>
#include <cmath>

class Data {
 public:
  Data(const std::string& filename);

  unsigned int size() const { return m_data.size(); }
  double measurement(int i) const { return m_data[i]; }
  double binCenter(int i) const { return (m_bins[i]+m_bins[i+1])/2; }
  double binLow(int i) const { return m_bins[i]; }
  double binHigh(int i) const { return m_bins[i+1]; }
  double error(int i) const { return m_errors[i]; }
  
  int checkCompatibility(const Data& in, int n);

  double chisquared_ndf(int parameters);
  
  Data operator+(const Data& in) {
    Data sum;
    // sum.m_data = std::vector<double>;
    for (int i = 0; i < in.size(); ++i) {
      double w1 = 1/pow(this->error(i),2);
      double w2 = 1/pow(in.error(i),2);
      double y = (w1 * this->measurement(i) + w2 * in.measurement(i))/(w1+w2);
      sum.m_data.push_back(y);
    }
    for (int i = 0; i < in.size(); ++i) {
      double w1 = 1/pow(this->error(i),2);
      double w2 = 1/pow(in.error(i),2);
      double sigma = 1/sqrt(w1+w2);
      sum.m_errors.push_back(sigma);
    }
    for (int i = 0; i < in.size()+1; ++i) {
      int bin = binLow(i);
      sum.m_bins.push_back(bin);
    }
    return sum;
  }
  

 private:
  Data() {}  // disallow empty data set
  void assertSizes();
  std::vector<double> m_data;
  std::vector<double> m_bins;
  std::vector<double> m_errors;
};

#endif
