add_library(MatrixAnalysis SHARED 
    src/MatrixAnalysis.cpp
    include/MatrixAnalysis.h
)

target_include_directories(MatrixAnalysis PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_features(MatrixAnalysis PUBLIC cxx_std_20)

target_compile_definitions(MatrixAnalysis PRIVATE MATRIX_EXPORTS)
install(TARGETS MatrixAnalysis
    EXPORT MatrixAnalysisTargets
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(DIRECTORY include/
    DESTINATION include
)