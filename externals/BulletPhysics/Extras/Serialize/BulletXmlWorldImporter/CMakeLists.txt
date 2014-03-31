INCLUDE_DIRECTORIES( 
	${BULLET_PHYSICS_SOURCE_DIR}/src
	${BULLET_PHYSICS_SOURCE_DIR}/Extras/Serialize/BulletFileLoader
	${BULLET_PHYSICS_SOURCE_DIR}/Extras/Serialize/BulletWorldImporter
)

ADD_LIBRARY(
	BulletXmlWorldImporter 
	btBulletXmlWorldImporter.cpp 
	btBulletXmlWorldImporter.h 
	string_split.cpp
	string_split.h
	tinyxml.cpp
	tinyxml.h
	tinystr.cpp
	tinystr.h
	tinyxmlerror.cpp
	tinyxmlparser.cpp
)

SET_TARGET_PROPERTIES(BulletXmlWorldImporter  PROPERTIES VERSION ${BULLET_VERSION})
SET_TARGET_PROPERTIES(BulletXmlWorldImporter PROPERTIES SOVERSION ${BULLET_VERSION})

IF (BUILD_SHARED_LIBS)
        TARGET_LINK_LIBRARIES(BulletXmlWorldImporter BulletWorldImporter BulletDynamics BulletCollision BulletFileLoader LinearMath)
ENDIF (BUILD_SHARED_LIBS)
