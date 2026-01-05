project(Void.INC)

set(CMAKE_CXX_STANDARD 17)

include_directories("C:/SFML-3.0.0+32-Bit/include")

link_directories("C:/SFML-3.0.0+32-Bit/lib")

add_executable (
	Void.INC "Void_INC.cpp")

target_link_libraries(Void.INC sfml-graphics sfml-window sfml-system sfml-audio sfml-network)