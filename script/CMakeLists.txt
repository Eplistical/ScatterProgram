CMAKE_MINIMUM_REQUIRED(VERSION 3.1.0)

ADD_SUBDIRECTORY(aux)

FILE(GLOB PYSCRIPT *.py)
FILE(GLOB SHSCRIPT *.sh)
INSTALL(FILES ${PYSCRIPT} ${SHSCRIPT} DESTINATION bin 
        PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ 
                    GROUP_EXECUTE GROUP_READ 
                    WORLD_READ WORLD_EXECUTE
        )
