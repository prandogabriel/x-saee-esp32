set(COMPONENT_SRCS
	"app_main.c"
	"shadow_demo_main.c"
	"shadow_demo_helpers.c"
	)

set(COMPONENT_ADD_INCLUDEDIRS
	"."
	"${CMAKE_CURRENT_LIST_DIR}"
	"${PROJECT_DIR}/../esp-idf-v5.0.2/components/esp-aws-iot/libraries/common/logging/"
	)

idf_component_register(SRCS "${COMPONENT_SRCS}"
					   INCLUDE_DIRS ${COMPONENT_ADD_INCLUDEDIRS}
					  )
