SUBDIRS(  BulletSoftBody BulletCollision BulletDynamics LinearMath )

IF(BUILD_MULTITHREADING)
	SUBDIRS(MiniCL BulletMultiThreaded)
ENDIF()