set(srcs "main.c"
		 "include/color_sensor.c"
		 "include/servo_motor.c"
		 "include/stepper_motor.c"
		 "include/wifi_config.c"
		 "include/myserver_config.c"
		 "include/mytask.c"
		 )

idf_component_register(SRCS ${srcs} INCLUDE_DIRS ".")