IDIR := inc
BINDIR := bin
SOURCE_DIR := src

CC := gcc
CXX := g++
CFLAGS := -I$(IDIR)
CXXFLAGS := -I$(IDIR)

ODIR := obj
C_ODIR := $(ODIR)/c
CXX_ODIR := $(ODIR)/cxx

DEPS_C := $(wildcard $(IDIR)/*.h)
DEPS_CXX := $(wildcard $(IDIR)/*.hpp)

C_SOURCES := $(shell find $(SOURCE_DIR) -type f -name '*.c' -execdir basename '{}' ';')
CXX_SOURCES := $(shell find $(SOURCE_DIR) -type f -name '*.cpp' -execdir echo '{}' ';')

OBJS := $(addprefix $(C_ODIR)/,$(C_SOURCES:%.c=%.o)) $(addprefix $(CXX_ODIR)/,$(CXX_SOURCES:%.cpp=%.o))

$(C_ODIR)/%.o: $(SOURCE_DIR)/%.c $(DEPS_C)
	$(CC) -c -o $@ $< $(CFLAGS)

$(CXX_ODIR)/%.o: $(SOURCE_DIR)/%.cpp $(DEPS_CXX)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

$(BINDIR)/app: $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o
