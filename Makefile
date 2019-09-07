CXX = g++
# For debugging use -Og -g
# For going fast use -O3
CXXFLAGS = -std=c++14 -Wall -D_GLIBCXX_DEBUG -D_LIBCXX_DEBUG_PEDANTIC -Og -g

# what folders
SRCDIR = src/
OBJDIR = obj/
BINDIR = ./

# File names
SOURCEFILES := $(notdir $(wildcard $(SRCDIR)*.cc))
SOURCES = $(addprefix $(SRCDIR), $(SOURCEFILES))
OBJECTS = $(addprefix $(OBJDIR), $(SOURCEFILES:.cc=.o))
EXEC = socks

DEPDIR := .d
$(shell mkdir -p $(DEPDIR) >/dev/null)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td

COMPILE.cc = $(CXX) $(DEPFLAGS) $(CXXFLAGS) $(INCLUDE_FLAGS) -c
POSTCOMPILE = @mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d && touch $@

all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CXX) $(OBJECTS) $(LINKER_FLAGS) -o $(BINDIR)$@


$(OBJDIR)%.o: $(SRCDIR)%.cc
$(OBJDIR)%.o: $(SRCDIR)%.cc $(DEPDIR)/%.d
	$(COMPILE.cc) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)

# To remove generated files
# This purposely does not remove the binary output
clean: 
	rm -f $(OBJDIR)*.o $(SRCDIR)*.gch

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

.PHONY: all clean

include $(wildcard $(patsubst %,$(DEPDIR)/%.d,$(basename $(SOURCEFILES))))
