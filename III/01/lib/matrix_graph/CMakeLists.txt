add_library(matrix_graph src/MatrixGraph.cpp)
target_include_directories(matrix_graph PUBLIC include)
target_link_libraries(matrix_graph PUBLIC i_graph)
