add_library(list_graph src/ListGraph.cpp)
target_include_directories(list_graph PUBLIC include)
target_link_libraries(list_graph PUBLIC i_graph)

