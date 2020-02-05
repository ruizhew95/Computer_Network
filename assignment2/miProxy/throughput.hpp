#pragma once
//Returns current throughput given all inputs to formula

//Assumes chunkSize given in BYTES
class ThroughputCalculator {
  public:
  ThroughputCalculator(double a): alpha(a){};

	double Add(double time, int chunkSize){
		//From bytes to Kbps
		int chunkKb = chunkSize / 125000; 
		double T_new = double(chunkKb) / time; 

		//EWMA avg formula from spec
		 T_cur = alpha * T_new + (1 - alpha) * T_cur; 
		return T_cur;
  }

  /* Returns the current estimated throughput */
  double Current(){ return T_cur; }

  private:
  double alpha;
  double T_cur = 0;
};
