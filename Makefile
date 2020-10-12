PROJECT_NAME := 49022_Project
#EXTRA_COMPONENT_DIRS = ${CMAKE_SOURCE_DIR}/components
include $(IDF_PATH)/make/project.mk
include sdkconfig
export IDF_PATH
