add_executable(Sandbox
  Sandbox/src/Sandbox.cpp
)

add_dependencies(Sandbox
  Katie
)
set_target_properties(Sandbox PROPERTIES
  OUTPUT_NAME Sandbox
)


target_include_directories(Sandbox PRIVATE
  C:/dev/Katie/Katie/vendor/spdlog/include
  C:/dev/Katie/Katie/src
)

target_compile_definitions(Sandbox PRIVATE
  KT_PLATFORM_WINDOWS
  _UNICODE
  UNICODE
  $<$<CONFIG:Debug>:KT_DEBUG>
  $<$<CONFIG:Release>:KT_RELEASE>
  $<$<CONFIG:Dist>:KT_DIST>
)
target_link_directories(Sandbox PRIVATE
)
target_link_libraries(Sandbox
  Katie
  vsg::vsg
)
if (vsgXchange_FOUND)
    target_compile_definitions(Sandbox PRIVATE vsgXchange_FOUND)
    target_link_libraries(Sandbox vsgXchange::vsgXchange)
endif()

