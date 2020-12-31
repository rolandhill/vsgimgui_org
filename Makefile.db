GLSLC = $(VULKAN_SDK)/bin/glslc

CXXFILES = main.cpp\
           VSGImGui.cpp\
           VSGImGuiEventHandler.cpp\


CXXFILES += \
        imgui.cpp\
        imgui_demo.cpp\
        imgui_draw.cpp\
        imgui_impl_vulkan.cpp\
        imgui_tables.cpp\
        imgui_widgets.cpp\

CXXFLAGS = -g3 -ggdb -std=c++17 -I$(VULKAN_SDK)/include -I./ -I./imgui

SPV_FILES = $(addsuffix .spv, $(wildcard *.frag) $(wildcard *.vert) $(wildcard *.geom))

OBJS = $(addprefix .build/,$(CXXFILES:.cpp=.o))

LIBS = -lvsgd -lvulkan

BUILD_DIR = .build

EXEC = vsgimgui

$(EXEC): $(BUILD_DIR) $(OBJS) $(SPV_FILES)
	$(CXX) $(LDFLAGS) $(OBJS) $(LIBS) -o $@

%.vert.spv: %.vert
	$(GLSLC) $< -o $@

%.frag.spv: %.frag
	$(GLSLC) $< -o $@

%.geom.spv: %.geom
	$(GLSLC) $< -o $@

clean:
	rm -f $(OBJS)

clobber: clean
	rm -f $(EXEC) $(SPV_FILES)

$(BUILD_DIR):
	mkdir -p .build

$(BUILD_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

VPATH += imgui
