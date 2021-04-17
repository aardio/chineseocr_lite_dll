set(OnnxRuntime_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/include")
include_directories(${OnnxRuntime_INCLUDE_DIRS})

if (APPLE)
    message("配置macOS OnnxRuntime 路径: ${CMAKE_CURRENT_LIST_DIR}/macos")
    link_directories(${CMAKE_CURRENT_LIST_DIR}/macos)
    set(OnnxRuntime_LIBS onnxruntime_session onnxruntime_optimizer onnxruntime_providers onnxruntime_util onnxruntime_framework onnxruntime_graph onnxruntime_common onnxruntime_mlas onnxruntime_flatbuffers onnx onnx_proto protobuf-lite re2 flatbuffers nsync_cpp)
elseif (WIN32)
    if(CMAKE_CL_64)
        message("配置Windows OnnxRuntime x64 路径: ${CMAKE_CURRENT_LIST_DIR}/windows-x64")
        link_directories(${CMAKE_CURRENT_LIST_DIR}/windows-x64)
    else()
        message("配置Windows OnnxRuntime x86 路径: ${CMAKE_CURRENT_LIST_DIR}/windows-x86")
        link_directories(${CMAKE_CURRENT_LIST_DIR}/windows-x86)
    endif()
    set(OnnxRuntime_LIBS onnxruntime_session onnxruntime_optimizer onnxruntime_providers onnxruntime_util onnxruntime_framework onnxruntime_graph onnxruntime_common onnxruntime_mlas onnxruntime_flatbuffers onnx onnx_proto libprotobuf-lite re2 flatbuffers)
elseif (UNIX)
    message("配置Linux OnnxRuntime 路径: ${CMAKE_CURRENT_LIST_DIR}/linux")
    link_directories(${CMAKE_CURRENT_LIST_DIR}/linux)
    set(OnnxRuntime_LIBS onnxruntime_session onnxruntime_optimizer onnxruntime_providers onnxruntime_util onnxruntime_framework onnxruntime_graph onnxruntime_common onnxruntime_mlas onnxruntime_flatbuffers onnx onnx_proto protobuf-lite re2 flatbuffers nsync_cpp)
endif ()

set(OnnxRuntime_FOUND TRUE)
