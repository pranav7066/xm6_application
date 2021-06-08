
set(InternalDependencies "${XM_TOOLS_LIBS_PATH}/crtn.o" "--start-group" "CDNNTarget" "CDNNManager" "CDNNAppUtility" "CDNNKernelModuleCommon" "CDNNDevice" "CDNNPCKernelModule" "CDNNUtility" "CDNNInfrastructure" "CDNNNeuProSDevice" "CDNNXM6Device" "CDNNXM6KernelModule")

add_compile_definitions(${HARDWARE_FLAGS})
add_compile_definitions(${PRINT_LEVEL_FLAGS})

set(directories "${XM_TOOLS_LIBS_PATH}"
"${CMAKE_BINARY_DIR}/CDNNBin/Lib/XM6/")

set(ExternalDependenciesPaths "${XM_TOOLS_LIBS_PATH}/libc.a"
"${XM_TOOLS_LIBS_PATH}/libc++.a"
"${XM_TOOLS_LIBS_PATH}/libc++abi.a"
"${XM_TOOLS_LIBS_PATH}/libcompiler-rt.a"
"${CMAKE_SOURCE_DIR}/../../Installs/install_xm6/lib/libcsl_xm6_elf.a"
"${CMAKE_SOURCE_DIR}/../../Installs/install_xm6/lib/libplatform_mgr.a"
"${CMAKE_SOURCE_DIR}/../../Installs/install_xm6/lib/libNeuProRuntime.a"
"${CMAKE_SOURCE_DIR}/../../Installs/install_xm6/lib/libneupro_service.a"
)
list(APPEND ExternalDependenciesPaths "--end-group")

include_directories(
"${CMAKE_CURRENT_SOURCE_DIR}/hdr"
"${CMAKE_SOURCE_DIR}/../../Installs/install_generic/include/"
"${CMAKE_SOURCE_DIR}/CDNNTools/CDNNAppUtility/hdr"
"${CMAKE_CURRENT_SOURCE_DIR}/../CDNNInterface/External"
"${CMAKE_CURRENT_SOURCE_DIR}/../CDNNTools/CDNNUtility/Include/"

"${CMAKE_CURRENT_SOURCE_DIR}/../CDNNTools/CDNNAppUtility/3rdparty/libjpeg/include"
"${CMAKE_CURRENT_SOURCE_DIR}/../CDNNTools/CDNNAppUtility/3rdparty/stbImage" 

"${CMAKE_CURRENT_SOURCE_DIR}/../../CDNNManager/Interface/Include/"
"${CMAKE_SOURCE_DIR}/../../Installs/install_generic/Accelerators/Common"
"${CMAKE_SOURCE_DIR}/../../Installs/install_generic/Accelerators/NeuProS/NeuProSDriver/include"
)

set (folder cdnn_xm6_app)
createExe(${folder} ${InternalDependencies} ${directories} ${ExternalDependenciesPaths} ${ExternalDependencies} ${InternalDependencies} ${LINKER_FLAGS})

# create ouput directory -- todo: remove it , it's diplicated
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/out)
