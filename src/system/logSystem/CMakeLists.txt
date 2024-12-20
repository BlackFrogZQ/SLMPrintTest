aux_source_directory(. DIR_SYSTEM_SRCS)
add_library(systemLogLib ${DIR_SYSTEM_SRCS})

add_library(debugLib debugLog.cpp)

target_link_libraries(systemLogLib
                        PRIVATE
						debug debugLib
						)