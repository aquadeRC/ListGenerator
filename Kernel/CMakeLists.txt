cmake_minimum_required(VERSION 3.16)
project(Kernel VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTORCC ON)

#add_subdirectory(ModeleDanych)
qt_add_qml_module(${PROJECT_NAME}
    STATIC
    URI Kernel
    SOURCES kernel.h kernel.cpp 
	KernelCommon.h 
	DataWrapperManager.h 
	DataWrapperManager.cpp
		SOURCES 
		ModeleDanych/UrzadDataModel.h
		ModeleDanych/ArchitektDataModel.h
		ModeleDanych/UrzadDataModel.cpp
		ModeleDanych/ArchitektDataModel.cpp
		DataTypes/UrzadData.h
		DataTypes/ArchitektData.h
		DataWrappers/ArchitektDataWrapper.h 
		DataWrappers/ArchitektDataWrapper.cpp
		DataWrappers/IDataWrapper.h
		SOURCES ModeleDanych/AbstractAppModel.h
		SOURCES DataWrappers/UrzadDataWrapper.h DataWrappers/UrzadDataWrapper.cpp

)
qt_add_resources(${PROJECT_NAME} "data"
        PREFIX "/data"
	FILES
	Data/ArchitektData.json
	Data/UrzedyData.json
    )

target_link_libraries(${PROJECT_NAME}
    PRIVATE Qt6::Quick
    )
