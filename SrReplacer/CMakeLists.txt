file(GLOB SRC_FILES *.h *.cpp)

add_executable(SrReplacer ${SRC_FILES})
target_link_libraries(SrReplacer jsoncpp ofstd oflog dcmdata dcmimage dcmimgle dcmsr)