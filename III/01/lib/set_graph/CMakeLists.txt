add_library(set_graph src/SetGraph.cpp)
target_include_directories(set_graph PUBLIC include)
target_link_libraries(set_graph PUBLIC i_graph)

