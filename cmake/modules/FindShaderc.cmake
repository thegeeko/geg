if(DEFINED ENV{VULKAN_SDK})
	# use shaderc from vulkan sdk if avaliabe
	if(WIN32)
		if (CMAKE_BUILD_TYPE MATCHES "Debug")
			set(GEG_SHADERC_LIB "$ENV{VULKAN_SDK}/Lib/shaderc_combinedd.lib")
		else()
			set(GEG_SHADERC_LIB "$ENV{VULKAN_SDK}/Lib/shaderc_combined.lib")
		endif()
	else()
		# on unix based systems
		set(GEG_SHADERC_LIB "$ENV{VULKAN_SDK}/lib/libshaderc_combinedd.a")
	endif()
else()
	message(STATUS "VulkanSdk not found looking into the system libraries")
	if(WIN32)
		message(FATAL_ERROR "You have to install VulkanSDK and add the env VULKAN_SDK")
	else()
		# in unix based systems check if the lib is installed to the system
		find_library(GEG_SHADERC_LIB shaderc_combined REQUIRED)
	endif()
endif()

message(STATUS "found shaderc in the path ${GEG_SHADERC_LIB}")

add_library(shaderc STATIC IMPORTED)
set_property(TARGET shaderc PROPERTY IMPORTED_LOCATION ${GEG_SHADERC_LIB})
