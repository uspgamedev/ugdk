INCLUDE_DIRECTORIES( 
	${BULLET_PHYSICS_SOURCE_DIR}/src
	${BULLET_PHYSICS_SOURCE_DIR}/Extras/Serialize/BulletFileLoader
)

ADD_LIBRARY(
BulletWorldImporter 
btBulletWorldImporter.cpp 
btBulletWorldImporter.h 
btWorldImporter.cpp 
btWorldImporter.h 
)

SET_TARGET_PROPERTIES(BulletWorldImporter PROPERTIES VERSION ${BULLET_VERSION})
SET_TARGET_PROPERTIES(BulletWorldImporter PROPERTIES SOVERSION ${BULLET_VERSION})

IF (BUILD_SHARED_LIBS)
        TARGET_LINK_LIBRARIES(BulletWorldImporter BulletDynamics BulletCollision BulletFileLoader LinearMath)
ENDIF (BUILD_SHARED_LIBS)
