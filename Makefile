INCLUDES= \
  src/Fl_Flow/Fl_Helper.h \
  src/Fl_Flow/Fl_Transform.h \
  src/Fl_Flow/Fl_Instruction.h \
  src/Fl_Flow/Fl_State.h \
  src/Fl_Flow/Fl_Flow.h

CXXFLAGS=-Wall -std=c++98
CXX=c++

all:
	mkdir -p include/FL
	cat $(INCLUDES) > include/FL/Fl_Flow.H
	$(CXX) -osample $(CXXFLAGS) `fltk-config --cxxflags` -Iinclude src/sample/*.cpp `fltk-config --ldflags`

clean:
	rm -f sample
	rm -r -f include
