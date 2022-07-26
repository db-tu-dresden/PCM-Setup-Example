#include <chrono>
#include <vector>
#include <iostream>
#include <fstream>
#include <random>
#include <cstdlib>
#include <cstdio>

//include the important headers for pcm
#include "pcm.h"

//these files are located in the algorithm directory and contain the code that is benchmarked
#include "isEven.h"
#include "codegenIsEven.h"

using namespace pcm;


int main(int argc, char ** argv) {
	using baseInType = uint64_t;//only integral types possible
	using baseOutType = bool;
	
	//for overall cpu time
	auto start = std::chrono::steady_clock::now();
	

	//Initialize PCM
	PCM *pcm = PCM::getInstance();
	//and check for errors
	auto error_code = pcm->program();
	if(error_code != PCM::Success) {
		std::cerr << "PCM couldn't start" << std::endl;
        std::cerr << "Error code: " << error_code << std::endl;
        std::cerr << "Try to execute 'sudo modprobe msr' and execute this program with root privigeges";
        return 1;
	}


	// set default values and gather user inputs
	long low = 10, high = 420, step = 10;
	int loops = 10;
	std::string filename = "result/is_even.csv"; // see NOTE below
	if(argc == 2) {
		filename = argv[1];
	}
	if(argc == 6) {
		low   = atol(argv[1]);
		high  = atol(argv[2]);
		step  = atol(argv[3]);
		loops = atoi(argv[4]);
		filename = argv[5];
	}	

	//open file for benchmark results
	std::ofstream output_file;
	//NOTE: There is no check weather the file already exists or not 
	// -> the file is created or overwritten no matter what (exception: the program (which needs root-rights to be able 
	//to read performance counters) has not the permission to write the file)
	output_file.open(filename);
	//print header to file (edit or remove if neccessary)
	output_file << "count, cycles0, l2hitRatio0, cycles1, l2hitRatio1" << std::endl;
	output_file.flush();


	//alloc memory
	baseInType* in_array    = static_cast<baseInType*> (aligned_alloc(sizeof(baseInType) , sizeof(baseInType)  * high));	
	baseOutType* out_array0 = static_cast<baseOutType*>(aligned_alloc(sizeof(baseOutType), sizeof(baseOutType) * high));
	baseOutType* out_array1 = static_cast<baseOutType*>(aligned_alloc(sizeof(baseOutType), sizeof(baseOutType) * high));
	for(baseInType i = 0; i < high; ++i) {
		in_array[i] = i;
	}
	
	
	//declare varibles to hold the performance counter states
	SystemCounterState before_sstate, after_sstate;
	
	//the actuall performance measuremnts below
	for(uint64_t count = low; count <= high; count += step){
		
		std::cout << "Run with count = " << count << std::endl;			
		
		//create state of the performance counters before 
    	before_sstate = getSystemCounterState();
		for(int i = 0; i < loops; ++i) {
			//replace loop by code to benchmark
			for(uint64_t j = 0; j <= count; ++j){
				out_array0[j] = isEven::simpleIsEven(in_array[j]);
			}
		}
		//create state of the performance counters after 
		after_sstate = getSystemCounterState();
		auto s_cycles  = getCycles(before_sstate, after_sstate);
        auto s_l2ratio = getL2CacheHitRatio(before_sstate, after_sstate);
        

		//create state of the performance counters before 
    	before_sstate = getSystemCounterState();
		for(int i = 0; i < loops; ++i) {
			//replace loop by code to benchmark
			for(uint64_t j = 0; j <= count; ++j){
				out_array1[j] = isEven::codegenIsEven(in_array[j]);
			}
		}
		//create state of the performance counters after 
		after_sstate = getSystemCounterState();
		//use before_sstate and after_sstate benchmark to get desired performance counter values
		auto c_cycles  = getCycles(before_sstate, after_sstate);
        auto c_l2ratio = getL2CacheHitRatio(before_sstate, after_sstate);
        //additional functions for additional countras can be found in the documentation under
		//		Classes > ClassMemebers > Related Functions
		//		file://<path to repo>/html/functions_rela.html
        

		output_file << count << ", " <<  s_cycles << ", " <<  s_l2ratio 
							   << ", " <<  c_cycles << ", " << c_l2ratio 
							   << "\n";
		output_file.flush();
	}

	//clean up
	free(in_array);
	free(out_array0);
    free(out_array1);
   	pcm->cleanup();
   	
   	
	output_file.close();
	std::cout << "The results were written to '" << filename << "'(relative to your current working directory)" << std::endl;
	
	auto end = std::chrono::steady_clock::now();
	auto time = std::chrono::duration_cast<std::chrono::seconds>(end - start);	
	std::cout << "The Benchmark ran for " << time.count() << "second(s)" << std::endl;
	return 0;
}
