add_executable(publisher publisher.cpp)
target_link_libraries(publisher eeros)

add_executable(subscriber subscriber.cpp)
target_link_libraries(subscriber eeros)