
CXX = g++

EXE = calc
IMGUI_DIR = ./imgui
IMPLOT_DIR = ./implot
SOURCES = main.cpp
SOURCES += $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_draw.cpp $(IMGUI_DIR)/imgui_tables.cpp $(IMGUI_DIR)/imgui_widgets.cpp
SOURCES += $(IMGUI_DIR)/backends/imgui_impl_glfw.cpp $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp
SOURCES += $(IMPLOT_DIR)/implot.cpp $(IMPLOT_DIR)/implot_items.cpp
SOURCES += algo.c bank.c helpers.c
OBJS = $(addsuffix .o, $(basename $(notdir $(SOURCES))))
UNAME_S := $(shell uname -s)
LINUX_GL_LIBS = -lGL

CFLAGS = -stc=c11 -Wall -Wformat -Wextra -Werror #-g

CXXFLAGS = -std=c++17
CXXFLAGS += -I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends -I$(IMPLOT_DIR)
CXXFLAGS += -Wall -Wformat #-g
LIBS =

ifeq ($(UNAME_S), Linux)
	LIBS += $(LINUX_GL_LIBS) `pkg-config --static --libs glfw3`

	CXXFLAGS += `pkg-config --cflags glfw3`
endif

ifeq ($(UNAME_S), Darwin)
	LIBS += -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
	LIBS += -L/usr/local/lib #-L/opt/local/lib -L/opt/homebrew/lib

	LIBS += -lglfw

	CXXFLAGS += -I/usr/local/include -I/opt/local/include -I/opt/homebrew/include
endif

%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:$(IMGUI_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:$(IMGUI_DIR)/backends/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:$(IMPLOT_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:$(IMPLOT_DIR)/backends/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:%.c
	gcc -c -o $@ $<

all: $(EXE)

$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

clean:
	rm -f $(OBJS)
	rm -rf manual

fclean: clean
	rm -f $(EXE)
	make -f back.make clean

test:
	make -f back.make test

gcov_report:
	make -f back.make gcov_report

install: $(EXE)
	install -m 777 $(EXE) ~/Desktop

uninstall:
	rm -rf ~/Desktop/$(EXE)

dist: $(EXE)
	tar -czf SmartCalc_v1.0.tgz $(EXE) 

dvi:
	makeinfo documentation.texi --html
	open ./manual/index.html
