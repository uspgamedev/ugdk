INCLUDE_DIRECTORIES(
	${BULLET_PHYSICS_SOURCE_DIR}/src
	${VECTOR_MATH_INCLUDE}
)

SET(BulletMultiThreaded_SRCS
	SpuFakeDma.cpp
	SpuLibspe2Support.cpp
	btThreadSupportInterface.cpp
	Win32ThreadSupport.cpp
	PosixThreadSupport.cpp
	SequentialThreadSupport.cpp
	SpuSampleTaskProcess.cpp
	SpuCollisionObjectWrapper.cpp 
	SpuCollisionTaskProcess.cpp
	SpuGatheringCollisionDispatcher.cpp
	SpuContactManifoldCollisionAlgorithm.cpp
	btParallelConstraintSolver.cpp
	
	#SPURS_PEGatherScatterTask/SpuPEGatherScatterTask.cpp
	#SpuPEGatherScatterTaskProcess.cpp

	SpuNarrowPhaseCollisionTask/boxBoxDistance.cpp
	SpuNarrowPhaseCollisionTask/SpuContactResult.cpp
	SpuNarrowPhaseCollisionTask/SpuMinkowskiPenetrationDepthSolver.cpp
	SpuNarrowPhaseCollisionTask/SpuGatheringCollisionTask.cpp
	SpuNarrowPhaseCollisionTask/SpuCollisionShapes.cpp

	#Some GPU related stuff, mainly CUDA and perhaps OpenCL
	btGpu3DGridBroadphase.cpp
)

SET(Root_HDRS
	PlatformDefinitions.h
	PpuAddressSpace.h
	SpuFakeDma.h
	SpuDoubleBuffer.h
	SpuLibspe2Support.h
	btThreadSupportInterface.h
	Win32ThreadSupport.h
	PosixThreadSupport.h
	SequentialThreadSupport.h
	SpuSampleTaskProcess.h
	SpuCollisionObjectWrapper.cpp 
	SpuCollisionObjectWrapper.h 
	SpuCollisionTaskProcess.h
	SpuGatheringCollisionDispatcher.h
	SpuContactManifoldCollisionAlgorithm.h
	btParallelConstraintSolver.h

	#SPURS_PEGatherScatterTask/SpuPEGatherScatterTask.h
	#SpuPEGatherScatterTaskProcess.h

	#Some GPU related stuff, mainly CUDA and perhaps OpenCL
	btGpu3DGridBroadphase.h
	btGpu3DGridBroadphaseSharedCode.h
	btGpu3DGridBroadphaseSharedDefs.h
	btGpu3DGridBroadphaseSharedTypes.h
	btGpuDefines.h
	btGpuUtilsSharedCode.h
	btGpuUtilsSharedDefs.h
)

SET(SpuNarrowPhaseCollisionTask_HDRS
	SpuNarrowPhaseCollisionTask/Box.h
	SpuNarrowPhaseCollisionTask/boxBoxDistance.h
	SpuNarrowPhaseCollisionTask/SpuContactResult.h
	SpuNarrowPhaseCollisionTask/SpuMinkowskiPenetrationDepthSolver.h
	SpuNarrowPhaseCollisionTask/SpuConvexPenetrationDepthSolver.h
	SpuNarrowPhaseCollisionTask/SpuPreferredPenetrationDirections.h
	SpuNarrowPhaseCollisionTask/SpuGatheringCollisionTask.h
	SpuNarrowPhaseCollisionTask/SpuCollisionShapes.h
)

SET(BulletMultiThreaded_HDRS
	${Root_HDRS}
	${SpuNarrowPhaseCollisionTask_HDRS}
)

ADD_LIBRARY(BulletMultiThreaded ${BulletMultiThreaded_SRCS} ${BulletMultiThreaded_HDRS})
SET_TARGET_PROPERTIES(BulletMultiThreaded PROPERTIES VERSION ${BULLET_VERSION})
SET_TARGET_PROPERTIES(BulletMultiThreaded PROPERTIES SOVERSION ${BULLET_VERSION})


SUBDIRS(GpuSoftBodySolvers)


IF (BUILD_SHARED_LIBS)
	IF (UNIX)
		TARGET_LINK_LIBRARIES(BulletMultiThreaded BulletDynamics BulletCollision pthread)
	ELSE()
		TARGET_LINK_LIBRARIES(BulletMultiThreaded BulletDynamics BulletCollision)
	ENDIF()
ENDIF (BUILD_SHARED_LIBS)
