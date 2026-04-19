cmake_minimum_required(VERSION 3.16)
project(Kernel VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTORCC ON)
find_package(Qt6 REQUIRED COMPONENTS NetworkAuth)

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
		ModeleDanych/DecyzjeModel.h
		ModeleDanych/DecyzjeModel.cpp

		DataTypes/UrzadData.h
		DataTypes/ArchitektData.h
		DataWrappers/ArchitektDataWrapper.h 
		DataWrappers/ArchitektDataWrapper.cpp
		DataWrappers/IDataWrapper.h
		SOURCES ModeleDanych/AbstractAppModel.h
		SOURCES DataWrappers/UrzadDataWrapper.h DataWrappers/UrzadDataWrapper.cpp
		SOURCES
		SOURCES googlesso.h googlesso.cpp
		SOURCES DataTypes/CredentialsData.h
		SOURCES ModeleDanych/ProjektModel.h ModeleDanych/ProjektModel.cpp
		SOURCES ModeleDanych/InwestorzyModel.h ModeleDanych/InwestorzyModel.cpp
		SOURCES GDocUpdateShema.h
		QML_FILES Data/Settings.json
		SOURCES Ustawienia.h Ustawienia.cpp
		SOURCES
		SOURCES ModeleDanych/PracowniaModel.h ModeleDanych/PracowniaModel.cpp

)
qt_add_resources(${PROJECT_NAME} "data"
        PREFIX "/data"
	FILES
	Data/ArchitektData.json
	Data/UrzedyData.json
    )



target_link_libraries(${PROJECT_NAME}
    PRIVATE Qt6::Quick Qt6::NetworkAuth
    )

add_custom_target(copy_data
        COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
	"${CMAKE_CURRENT_SOURCE_DIR}/Data/"
	"${CMAKE_CURRENT_BINARY_DIR}/Data"
    COMMENT "Copying users data"
)
add_dependencies(${PROJECT_NAME} copy_data)

