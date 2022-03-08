INCLUDE_DIRECTORIES(include)
ADD_LIBRARY(prepmatrix STATIC src/matrix.cpp)

ADD_SUBDIRECTORY(tests)
