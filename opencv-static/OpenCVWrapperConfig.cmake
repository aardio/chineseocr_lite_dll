if (WIN32)
    if (CMAKE_CL_64)
        message("配置WINDOWS OpenCV x64 路径: ${CMAKE_CURRENT_LIST_DIR}/windows-x64")
        set(OpenCV_DIR "${CMAKE_CURRENT_LIST_DIR}/windows-x64")
    else ()
        message("配置WINDOWS OpenCV x86 路径: ${CMAKE_CURRENT_LIST_DIR}/windows-x86")
        set(OpenCV_DIR "${CMAKE_CURRENT_LIST_DIR}/windows-x86")
    endif ()
elseif (APPLE)
    message("配置macOS OpenCV 路径: ${CMAKE_CURRENT_LIST_DIR}/macos/share/OpenCV")
    set(OpenCV_DIR "${CMAKE_CURRENT_LIST_DIR}/macos/share/OpenCV")
elseif (UNIX)
    message("配置Linux OpenCV 路径: ${CMAKE_CURRENT_LIST_DIR}/linux/share/OpenCV")
    set(OpenCV_DIR "${CMAKE_CURRENT_LIST_DIR}/linux/share/OpenCV")
endif ()
