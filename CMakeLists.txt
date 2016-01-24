cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(SegToolExample)
find_package(MITK REQUIRED)

# Check that MITK has been build with Qt support
if(NOT MITK_USE_QT)
  message(SEND_ERROR "MITK needs to be built with MITK_USE_QT set to ON")
endif()

#include_directories(
#  ${CMAKE_CURRENT_SOURCE_DIR}/../../Examples/Tutorial/Step6
#  ${CMAKE_CURRENT_SOURCE_DIR}/../../Examples/Tutorial/Step7
#  )

mitk_create_executable(
  ${PROJECT_NAME} DEPENDS MitkQtWidgetsExt
  PACKAGE_DEPENDS ITK|ITKCurvatureFlow+ITKRegionGrowing
  )
