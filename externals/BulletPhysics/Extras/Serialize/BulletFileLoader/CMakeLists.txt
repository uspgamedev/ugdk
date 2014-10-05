INCLUDE_DIRECTORIES( 
	${BULLET_PHYSICS_SOURCE_DIR}/src  
)

SET(BulletFileLoader_SRCS
bChunk.cpp
bDNA.cpp
bFile.cpp
btBulletFile.cpp
)

SET(BulletFileLoader_HDRS
bChunk.h
bCommon.h
bDefines.h
bDNA.h
bFile.h
btBulletFile.h
)

ADD_LIBRARY(BulletFileLoader ${BulletFileLoader_SRCS} ${BulletFileLoader_HDRS})

IF (BUILD_SHARED_LIBS)
        TARGET_LINK_LIBRARIES(BulletFileLoader LinearMath)
ENDIF (BUILD_SHARED_LIBS)

SET_TARGET_PROPERTIES(BulletFileLoader PROPERTIES VERSION ${BULLET_VERSION}) 
SET_TARGET_PROPERTIES(BulletFileLoader PROPERTIES SOVERSION ${BULLET_VERSION})
