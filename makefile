#
# Cross Platform Makefile
# Compatible with MSYS2/MINGW, Ubuntu 14.04.1 and Mac OS X
#
# You will need GLFW (http://www.glfw.org):
# Linux:
#   apt-get install libglfw-dev
# Mac OS X:
#   brew install glfw
# MSYS2:
#   pacman -S --noconfirm --needed mingw-w64-x86_64-toolchain mingw-w64-x86_64-glfw
#

#CXX = g++
#CXX = clang++

EXE = simple_chacha20_app
IMGUI_DIR = .
SOURCES = main.cpp
SOURCES += $(IMGUI_DIR)/imgui/imgui.cpp $(IMGUI_DIR)/imgui/imgui_demo.cpp $(IMGUI_DIR)/imgui/imgui_draw.cpp $(IMGUI_DIR)/imgui/imgui_tables.cpp $(IMGUI_DIR)/imgui/imgui_widgets.cpp
SOURCES += $(IMGUI_DIR)/backends/imgui_impl_glfw.cpp $(IMGUI_DIR)/backends/imgui_impl_opengl2.cpp
SOURCES += $(IMGUI_DIR)/implot/implot.cpp $(IMGUI_DIR)/implot/implot_items.cpp $(IMGUI_DIR)/implot/implot_demo.cpp  

#SOURCES += $(IMGUI_DIR)/NormalDistribution/normaldistribution.c $(IMGUI_DIR)/NormalDistribution/data.c 

OBJS = $(addsuffix .o, $(basename $(notdir $(SOURCES))))
UNAME_S := $(shell uname -s)

CXXFLAGS = -lm -lrt -std=c++11 -I$(IMGUI_DIR)/eqapp -I$(IMGUI_DIR)/imgui -I$(IMGUI_DIR)/backends -I$(IMGUI_DIR)/implot   
CXXFLAGS += -g -Wall -Wformat
LIBS =  -lpthread -lm -lcrypt -lcrypto -lssl -lrt -lncurses

##--------------------------------------------------------------------- -lwiringPi -lwiringPiDev
## BUILD FLAGS PER PLATFORM
##---------------------------------------------------------------------

ifeq ($(UNAME_S), Linux) #LINUX
	ECHO_MESSAGE = "Linux"
	LIBS += -lGL `pkg-config --static --libs glfw3`

	CXXFLAGS += `pkg-config --cflags glfw3`
	CFLAGS = $(CXXFLAGS)
endif


##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------

%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:$(IMGUI_DIR)/imgui/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:$(IMGUI_DIR)/backends/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:$(IMGUI_DIR)/implot/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<



#%.o:$(IMGUI_DIR)/NormalDistribution/%.c
#	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: $(EXE)
	@echo Build complete for $(ECHO_MESSAGE)

$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

clean:
	rm -f $(EXE) $(OBJS)

