#MiniCL provides a small subset of OpenCL

INCLUDE_DIRECTORIES(
	${BULLET_PHYSICS_SOURCE_DIR}/src
	${VECTOR_MATH_INCLUDE}
)

SET(MiniCL_SRCS
	MiniCL.cpp
	MiniCLTaskScheduler.cpp
	MiniCLTask/MiniCLTask.cpp
)

SET(Root_HDRS
	MiniCLTaskScheduler.h
	cl.h
	cl_gl.h
	cl_platform.h
	cl_MiniCL_Defs.h
)

SET(MiniCLTask_HDRS
	MiniCLTask/MiniCLTask.h
)

SET(MiniCL_HDRS
	${Root_HDRS}
	${MiniCLTask_HDRS}
)

ADD_LIBRARY(MiniCL ${MiniCL_SRCS} ${MiniCL_HDRS} )
SET_TARGET_PROPERTIES(MiniCL PROPERTIES VERSION ${BULLET_VERSION})
SET_TARGET_PROPERTIES(MiniCL PROPERTIES SOVERSION ${BULLET_VERSION})


IF (BUILD_SHARED_LIBS)
	TARGET_LINK_LIBRARIES(MiniCL BulletMultiThreaded BulletDynamics BulletCollision)
ENDIF (BUILD_SHARED_LIBS)
