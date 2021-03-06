file(GLOB_RECURSE TESTVOXEL_HEADER_FILES
	"*.h"
	"*.hpp"
)
file(GLOB_RECURSE TESTVOXEL_SOURCE_FILES
	"*.c"
	"*.cpp"
	"*.rc"
)
set(TESTVOXEL_SOURCES
	${TESTVOXEL_HEADER_FILES}
	${TESTVOXEL_SOURCE_FILES}
)
set(TESTVOXEL_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/Tests/TestVoxel/")

if(ACID_BUILD_RELEASE AND WIN32)
	add_executable(TestVoxel WIN32 ${TESTVOXEL_SOURCES})
else()
	add_executable(TestVoxel ${TESTVOXEL_SOURCES})
endif()

set_target_properties(TestVoxel PROPERTIES
	POSITION_INDEPENDENT_CODE ON
	FOLDER "Acid"
)

add_dependencies(TestVoxel Acid)

target_include_directories(TestVoxel PUBLIC ${ACID_INCLUDE_DIR} ${TESTVOXEL_INCLUDE_DIR})
target_link_libraries(TestVoxel PUBLIC Acid)

if(MSVC AND "${CMAKE_BUILD_TYPE}" STREQUAL "Release")
	set_target_properties(TestVoxel PROPERTIES LINK_FLAGS "/subsystem:windows /ENTRY:mainCRTStartup")
elseif(UNIX AND APPLE)
	set_target_properties(TestVoxel PROPERTIES
		MACOSX_BUNDLE_BUNDLE_NAME "Test Voxel"
		MACOSX_BUNDLE_SHORT_VERSION_STRING ${ACID_VERSION}
		MACOSX_BUNDLE_LONG_VERSION_STRING ${ACID_VERSION}
		MACOSX_BUNDLE_INFO_PLIST "${PROJECT_SOURCE_DIR}/Scripts/MacOSXBundleInfo.plist.in"
	)
endif()

# Install
if(ACID_INSTALL)
	install(DIRECTORY .
		DESTINATION include
		FILES_MATCHING PATTERN "*.h"
		PATTERN "Private" EXCLUDE
	)

	install(TARGETS TestVoxel
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	)
endif()