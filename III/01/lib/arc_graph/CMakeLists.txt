add_library(arc_graph src/ArcGraph.cpp)
target_include_directories(arc_graph PUBLIC include)
target_link_libraries(arc_graph PUBLIC i_graph)

