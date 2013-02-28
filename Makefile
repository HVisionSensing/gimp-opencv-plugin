
CXX=g++
GIMPTOOL=gimptool-2.0

GIMP_FLAGS=`$(GIMPTOOL) --cflags`
GIMP_LIBS=`$(GIMPTOOL) --libs`

CV_FLAGS=`pkg-config --cflags opencv`
CV_LIBS=`pkg-config --libs opencv`

RM=rm -f

SOURCES=opencv.cpp cvt_color.cpp
OBJECTS=$(SOURCES:%.cpp=%.o)

all: opencv

opencv: $(OBJECTS)
	$(CXX) $(GIMP_LIBS) $(CV_LIBS) $^ -o $@

%.o: %.cpp
	$(CXX) $(GIMP_FLAGS) $(CV_FLAGS) -c $^

install: opencv
	$(GIMPTOOL) --install-bin opencv

clean:
	$(RM) opencv.o opencv *~
