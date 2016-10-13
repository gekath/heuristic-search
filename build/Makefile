all: debug

print-%: ; @echo $*=$($*)

# List headers here
HEADERS = \
	generic_defs/single_goal_test.h \
	generic_defs/non_goal_heuristic.h \
	generic_defs/permutation_hash_function.h \
	domains/tile_puzzle/tile_puzzle_state.h \
	domains/tile_puzzle/tile_puzzle_transitions.h \
	domains/tile_puzzle/tile_manhattan_distance.h \
	domains/map_pathfinding/map_location.h \
	domains/map_pathfinding/map_pathfinding_transitions.h \
	domains/map_pathfinding/map_manhattan_distance.h \
	domains/map_pathfinding/map_octile_distance.h \
	domains/map_pathfinding/map_loc_hash_function.h \
	domains/map_pathfinding/map_utils.h \
	utils/floating_point_utils.h \
	algorithms/best_first_search/node_table.h \
	algorithms/best_first_search/open_closed_list.h \
	algorithms/best_first_search/a_star.h \
	utils/string_utils.h \
	utils/combinatorics.h

# Don't need object files for these. HORRIBLE HACK for now.
ABS_H_FILES = \
	../src/generic_defs/transition_system.h \
	../src/generic_defs/goal_test_function.h \
	../src/generic_defs/state_hash_function.h \
	../src/generic_defs/heuristic.h \
	../src/generic_defs/search_engine.h \
	../src/algorithms/best_first_search/best_first_search.h

# Builds list of sources
SOURCES = $(HEADERS:%.h=%.cpp)

# Defines location of the object files
OBJ_LOC = ../obj

# Defines the objects for the debug build
OBJ_DEBUG_SUFFIX = debug.o
OBJS_DEBUG = $(SOURCES:%.cpp=$(OBJ_LOC)/%.$(OBJ_DEBUG_SUFFIX))

# Defines the objects for the release build
OBJ_RELEASE_SUFFIX = release.o
OBJS_RELEASE = $(SOURCES:%.cpp=$(OBJ_LOC)/%.$(OBJ_RELEASE_SUFFIX)) 

# Libraries
LIBS =

# Excecutable names for testers
TILE_TARGET = ../bin/tiles

EMPTY_MAP_TARGET = ../bin/empty_map

STARCRAFT_MAP_TARGET = ../bin/starcraft_map

# Defines the compiler and the compiler flags
CC = g++
CXXFLAGS = -Wall -pedantic -ansi -Werror -Wno-unused-function -std=c++11
CXXRELEASEFLAGS = -O3 $(CXXFLAGS)
CXXDEBUGFLAGS = -g $(CXXFLAGS)

# Builds object files
$(OBJS_DEBUG): $(OBJ_LOC)/%.$(OBJ_DEBUG_SUFFIX) : ../src/%.cpp ../src/%.h $(ABS_H_FILES)
	@mkdir -p $(@D)
	$(CC) $(CXXDEBUGFLAGS) -c $< -o $@
	
$(TILE_TARGET): $(OBJS_DEBUG) ../src/experiments/tile_experiments.cpp
	$(CC) $(CXXDEBUGFLAGS) -o $(TILE_TARGET) ../src/experiments/tile_experiments.cpp $(OBJS_DEBUG) $(LIBS)
	
$(EMPTY_MAP_TARGET): $(OBJS_DEBUG) ../src/experiments/empty_grid_experiments.cpp
	$(CC) $(CXXDEBUGFLAGS) -o $(EMPTY_MAP_TARGET) ../src/experiments/empty_grid_experiments.cpp $(OBJS_DEBUG) $(LIBS)
	
$(STARCRAFT_MAP_TARGET): $(OBJS_DEBUG) ../src/experiments/starcraft_map_experiments.cpp
	$(CC) $(CXXDEBUGFLAGS) -o $(STARCRAFT_MAP_TARGET) ../src/experiments/starcraft_map_experiments.cpp $(OBJS_DEBUG) $(LIBS)

debug: $(TILE_TARGET) $(EMPTY_MAP_TARGET) $(STARCRAFT_MAP_TARGET)

clean:
	rm -f $(OBJS_DEBUG) $(OBJS_RELEASE) $(TILE_TARGET) $(EMPTY_MAP_TARGET) $(STARCRAFT_MAP_TARGET)