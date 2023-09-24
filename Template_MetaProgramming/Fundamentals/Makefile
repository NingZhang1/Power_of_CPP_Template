CXX := g++
OMP = -fopenmp
OPTLEVEL = -O2
# CXX := icpc
# OMP = -qopenmp
# ICPC_OPT_REPORT := -qopt-report -qopt-report-phase=vec 
CXX_WARNING_OPTIONS := -Wall -Wextra -Wno-unused-variable -Wno-unused-function $(ICPC_OPT_REPORT) 

# CXX_STANDATD = c++11
# CXX_STANDATD = c++14
CXX_STANDATD = c++17

CXXFLAGS := -std=$(CXX_STANDATD) $(OPTLEVEL)  $(OMP) $(CXX_WARNING_OPTIONS) 
LDFALGS := -pthread $(OMP) -lpthread -lm -ldl

DEBUGGING = -g
# DEBUGGING =
CXXFLAGS := $(CXXFLAGS) -I$(LIB_DIR) -I./ $(DEBUGGING) -fPIC

.PHONY: clean

# APP 

Unexpected_Template_Paramter: Unexpected_Template_Paramter.o 
	$(CXX) -std=$(CXX_STANDATD) Unexpected_Template_Paramter.o -L. $(LDFLAGS) $(DEBUGGING) -o Unexpected_Template_Paramter.exe $(LDFALGS) 

Unexpected_Template_Paramter.o: Unexpected_Template_Paramter.cpp 
	$(CXX) $(CXXFLAGS)  -c -o Unexpected_Template_Paramter.o Unexpected_Template_Paramter.cpp 

clean:
	rm *.o