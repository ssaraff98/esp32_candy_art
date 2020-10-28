set(srcs "main.c"
		 "include/color_sensor.c"
		 "include/stepper_motor.c"
		 )

idf_component_register(SRCS ${srcs} INCLUDE_DIRS ".")