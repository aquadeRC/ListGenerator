project(LG_GUI VERSION 0.1 LANGUAGES CXX)

qt_add_qml_module(${PROJECT_NAME}
    STATIC
    URI LG_GUI
       QML_FILES AppTheme.qml
		QML_FILES AppToolBar.qml
		QML_FILES ComboField.qml
		QML_FILES Combo.qml
		QML_FILES Footer.qml
		QML_FILES IconButton.qml
		QML_FILES LeftPanel.qml
		QML_FILES ThemeSwitch.qml
		QML_FILES ArchitektModel.qml
		QML_FILES InwestorModel.qml
		QML_FILES InwestycjaModel.qml
		QML_FILES UrzadModel.qml
		QML_FILES Ustawienia.qml
	RESOURCES icons/ksiezyc.svg icons/sloneczko.svg icons/ustawienia.svg
	QML_FILES FileEdit.qml
	QML_FILES RoundedButton.qml
	QML_FILES RoundedTextField.qml
	QML_FILES FieldText.qml
	QML_FILES ComboField2.qml
	QML_FILES ComboN.qml
	QML_FILES

)

add_custom_target(copy_Icons_data
        COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
	"${CMAKE_CURRENT_SOURCE_DIR}/icons/"
	"${CMAKE_CURRENT_BINARY_DIR}/icons"
    COMMENT "Copying users data"
)
add_dependencies(${PROJECT_NAME} copy_Icons_data)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Quick)