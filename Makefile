TARGET	= DrawOpenGLinImGuiWindow
SOURCES	= $(wildcard *.cpp) $(wildcard lib/*.cpp)
HEADERS	= $(wildcard *.h) $(wildcard lib/*.h)
OBJECTS	= $(patsubst %.cpp,%.o,$(SOURCES))
CXXFLAGS	= --std=c++17 -g -Wall -DDEBUG -DX11 -Iinclude
LDLIBS	= -Llib -lglfw3_linux -lGL -lXrandr -lXinerama -lXcursor -lXxf86vm -lXi -lX11 -lpthread -lrt -lm -ldl

.PHONY: clean

$(TARGET): $(OBJECTS)
	$(LINK.cc) $^ $(LOADLIBES) $(LDLIBS) -o $@

$(TARGET).dep: $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) -MM $(SOURCES) > $@

clean:
	-$(RM) $(TARGET) *.o lib/*.o *~ .*~ *.bak *.dep imgui.ini a.out core

-include $(TARGET).dep
