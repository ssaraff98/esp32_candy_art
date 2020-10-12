set(srcs "main.c"

		 "include/color_sensor.c"
		 )

idf_component_register(SRCS ${srcs} INCLUDE_DIRS ".")