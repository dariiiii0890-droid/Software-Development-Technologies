add_executable(TestApp main.cpp)
target_link_libraries(TestApp PRIVATE MatrixAnalysis)
add_custom_command(
    TARGET TestApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:MatrixAnalysis>
        $<TARGET_FILE_DIR:TestApp>
)