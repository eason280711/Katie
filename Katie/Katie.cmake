add_library(Katie SHARED
    Katie/src/Katie/Core/Application.cpp
    Katie/src/Katie/Core/Application.h
    Katie/src/Katie/Core/Core.h
    Katie/src/Katie/Core/EntryPoint.h
      Katie/src/Katie/Event/ApplicationEvent.h
      Katie/src/Katie/Event/Event.h
      Katie/src/Katie/Event/KeyEvent.h
      Katie/src/Katie/Event/MouseEvent.h
    Katie/src/Katie/Log/Log.cpp
    Katie/src/Katie/Log/Log.h
  Katie/src/Katie.h
  Katie/src/ktpch.cpp
  Katie/src/ktpch.h
)

target_link_libraries(Katie vsg::vsg)

if (vsgXchange_FOUND)
    target_compile_definitions(Katie PRIVATE vsgXchange_FOUND)
    target_link_libraries(Katie vsgXchange::vsgXchange)
endif()

# if (VULKAN_SDK)
#     set(ENV{VULKAN_SDK} ${VULKAN_SDK})
# endif()
# find_package(vsg 0.4.0 REQUIRED)
# find_package(vsgXchange QUIET)

set_target_properties(Katie PROPERTIES
  OUTPUT_NAME Katie
)

target_include_directories(Katie PRIVATE
  C:/dev/Katie/Katie/src
  C:/dev/Katie/Katie/vendor/spdlog/include
)

target_compile_definitions(Katie PRIVATE
  KT_PLATFORM_WINDOWS
  KT_BUILD_DLL
  $<$<CONFIG:Debug>:KT_DEBUG>
  $<$<CONFIG:Release>:KT_RELEASE>
  $<$<CONFIG:Dist>:KT_DIST>
)

target_link_directories(Katie PRIVATE
)
