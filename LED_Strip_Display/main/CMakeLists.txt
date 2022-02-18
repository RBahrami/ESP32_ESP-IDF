FILE(GLOB_RECURSE app_sources ${PROJECT_DIR}/main/*.*)

idf_component_register(	SRCS ${app_sources}
						INCLUDE_DIRS "." "Libraries"
					  )