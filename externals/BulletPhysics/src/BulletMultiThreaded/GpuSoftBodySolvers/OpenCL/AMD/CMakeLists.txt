
INCLUDE_DIRECTORIES(
	${BULLET_PHYSICS_SOURCE_DIR}/src
	${AMD_OPENCL_INCLUDES}
)

ADD_DEFINITIONS(-DUSE_AMD_OPENCL)
ADD_DEFINITIONS(-DCL_PLATFORM_AMD)



SET(BulletSoftBodyOpenCLSolvers_SRCS
	../btSoftBodySolver_OpenCL.cpp
	../btSoftBodySolver_OpenCLSIMDAware.cpp
	../btSoftBodySolverOutputCLtoGL.cpp
)

SET(BulletSoftBodyOpenCLSolvers_HDRS
	../btSoftBodySolver_OpenCL.h
	../btSoftBodySolver_OpenCLSIMDAware.h
	../../Shared/btSoftBodySolverData.h
	../btSoftBodySolverVertexData_OpenCL.h
	../btSoftBodySolverTriangleData_OpenCL.h
	../btSoftBodySolverLinkData_OpenCL.h
	../btSoftBodySolverLinkData_OpenCLSIMDAware.h
	../btSoftBodySolverBuffer_OpenCL.h
	../btSoftBodySolverVertexBuffer_OpenGL.h
	../btSoftBodySolverOutputCLtoGL.h
)




ADD_LIBRARY(BulletSoftBodySolvers_OpenCL_AMD
	${BulletSoftBodyOpenCLSolvers_SRCS} 
	${BulletSoftBodyOpenCLSolvers_HDRS} 
)

SET_TARGET_PROPERTIES(BulletSoftBodySolvers_OpenCL_AMD PROPERTIES VERSION ${BULLET_VERSION})
SET_TARGET_PROPERTIES(BulletSoftBodySolvers_OpenCL_AMD PROPERTIES SOVERSION ${BULLET_VERSION})
IF (BUILD_SHARED_LIBS)
	TARGET_LINK_LIBRARIES(BulletSoftBodySolvers_OpenCL_AMD BulletSoftBody)
ENDIF (BUILD_SHARED_LIBS)
