################################################################################
# this is used for compile the user defined oc demo
################################################################################

#if you have defined multiple demos by yourself, modify this file to compile the chosen demo.

#example for hello world
	#example for hello world	 
	ifeq ($(CONFIG_USER_DEMO), "hello_world_demo")	
		user_demo_src  = ${wildcard $(TARGET_DIR)/Demos/hello_world_demo/*.c}
		user_demo_inc = -I $(TARGET_DIR)/Demos/hello_world_demo
		user_demo_defs = -D CONFIG_HELLO_WORLD_ENABLE=1
	endif

#example for nb lwm2m
	#example for oc_nb_lwm2m_light
	ifeq ($(CONFIG_USER_DEMO), "oc_nb_lwm2m_light")	
		user_demo_src  = ${wildcard $(TARGET_DIR)/Demos/oc_nb_lwm2m_light/*.c}
		user_demo_inc = -I $(TARGET_DIR)/Demos/oc_nb_lwm2m_light
		user_hardware_src = ${wildcard $(TARGET_DIR)/Hardware/E53_SC1/*.c} 
		user_hardware_inc = -I ${wildcard $(TARGET_DIR)/Hardware/E53_SC1}
		user_demo_defs = -D CONFIG_OC_STREELIGHT_DEMO_ENABLE=1
		
	endif

	#example for oc_nb_lwm2m_agriculture
	ifeq ($(CONFIG_USER_DEMO), "oc_nb_lwm2m_agriculture")	
		user_demo_src  = ${wildcard $(TARGET_DIR)/Demos/oc_nb_lwm2m_agriculture/*.c}
		user_demo_inc = -I $(TARGET_DIR)/Demos/oc_nb_lwm2m_agriculture
		user_hardware_src = ${wildcard $(TARGET_DIR)/Hardware/E53_IA1/*.c} 
		user_hardware_inc = -I ${wildcard $(TARGET_DIR)/Hardware/E53_IA1}
		user_demo_defs = -D CONFIG_OC_ARRICULTURE_DEMO_ENABLE=1
		
	endif

	#example for oc_nb_lwm2m_gps
	ifeq ($(CONFIG_USER_DEMO), "oc_nb_lwm2m_gps")	
		user_demo_src  = ${wildcard $(TARGET_DIR)/Demos/oc_nb_lwm2m_gps/*.c}
		user_demo_inc = -I $(TARGET_DIR)/Demos/oc_nb_lwm2m_gps
		user_hardware_src = ${wildcard $(TARGET_DIR)/Hardware/E53_ST1/*.c} 
		user_hardware_inc = -I ${wildcard $(TARGET_DIR)/Hardware/E53_ST1}
		user_demo_defs = -D CONFIG_OC_TRACK_DEMO_ENABLE=1
		
	endif

	#example for oc_nb_lwm2m_smoke
	ifeq ($(CONFIG_USER_DEMO), "oc_nb_lwm2m_smoke")	
		user_demo_src  = ${wildcard $(TARGET_DIR)/Demos/oc_nb_lwm2m_smoke/*.c}
		user_demo_inc = -I $(TARGET_DIR)/Demos/oc_nb_lwm2m_smoke
		user_hardware_src = ${wildcard $(TARGET_DIR)/Hardware/E53_SF1/*.c} 
		user_hardware_inc = -I ${wildcard $(TARGET_DIR)/Hardware/E53_SF1}
		user_demo_defs = -D CONFIG_OC_SMOKE_DEMO_ENABLE=1
	endif

	#example for oc_nb_lwm2m_manhole_cover
	ifeq ($(CONFIG_USER_DEMO), "oc_nb_lwm2m_manhole_cover")	
		user_demo_src  = ${wildcard $(TARGET_DIR)/Demos/oc_nb_lwm2m_manhole_cover/*.c}
		user_demo_inc = -I $(TARGET_DIR)/Demos/oc_nb_lwm2m_manhole_cover
		user_hardware_src = ${wildcard $(TARGET_DIR)/Hardware/E53_SC2/*.c} 
		user_hardware_inc = -I ${wildcard $(TARGET_DIR)/Hardware/E53_SC2}
		user_demo_defs = -D CONFIG_OC_MANHOLE_COVER_DEMO_ENABLE=1
	endif

	#example for oc_nb_lwm2m_infrared
	ifeq ($(CONFIG_USER_DEMO), "oc_nb_lwm2m_infrared")	
		user_demo_src  = ${wildcard $(TARGET_DIR)/Demos/oc_nb_lwm2m_infrared/*.c}
		user_demo_inc = -I $(TARGET_DIR)/Demos/oc_nb_lwm2m_infrared
		user_hardware_src = ${wildcard $(TARGET_DIR)/Hardware/E53_IS1/*.c} 
		user_hardware_inc = -I ${wildcard $(TARGET_DIR)/Hardware/E53_IS1}
		user_demo_defs = -D CONFIG_OC_INFRARED_DEMO_ENABLE=1
		
	endif

#example for wifi lwm2m
	#example for oc_wifi_lwm2m_light
	ifeq ($(CONFIG_USER_DEMO), "oc_wifi_lwm2m_light")	
		user_demo_src  = ${wildcard $(TARGET_DIR)/Demos/oc_wifi_lwm2m_light/*.c}
		user_demo_inc = -I $(TARGET_DIR)/Demos/oc_wifi_lwm2m_light
		user_hardware_src = ${wildcard $(TARGET_DIR)/Hardware/E53_SC1/*.c} 
		user_hardware_inc = -I ${wildcard $(TARGET_DIR)/Hardware/E53_SC1}
		user_demo_defs = -D CONFIG_OC_STREELIGHT_DEMO_ENABLE=1
		
	endif

	#example for oc_wifi_lwm2m_agriculture
	ifeq ($(CONFIG_USER_DEMO), "oc_wifi_lwm2m_agriculture")	
		user_demo_src  = ${wildcard $(TARGET_DIR)/Demos/oc_wifi_lwm2m_agriculture/*.c}
		user_demo_inc = -I $(TARGET_DIR)/Demos/oc_wifi_lwm2m_agriculture
		user_hardware_src = ${wildcard $(TARGET_DIR)/Hardware/E53_IA1/*.c} 
		user_hardware_inc = -I ${wildcard $(TARGET_DIR)/Hardware/E53_IA1}
		user_demo_defs = -D CONFIG_OC_ARRICULTURE_DEMO_ENABLE=1
		
	endif

	#example for oc_wifi_lwm2m_gps
	ifeq ($(CONFIG_USER_DEMO), "oc_wifi_lwm2m_gps")	
		user_demo_src  = ${wildcard $(TARGET_DIR)/Demos/oc_wifi_lwm2m_gps/*.c}
		user_demo_inc = -I $(TARGET_DIR)/Demos/oc_wifi_lwm2m_gps
		user_hardware_src = ${wildcard $(TARGET_DIR)/Hardware/E53_ST1/*.c} 
		user_hardware_inc = -I ${wildcard $(TARGET_DIR)/Hardware/E53_ST1}
		user_demo_defs = -D CONFIG_OC_TRACK_DEMO_ENABLE=1
		
	endif

	#example for oc_wifi_lwm2m_smoke
	ifeq ($(CONFIG_USER_DEMO), "oc_wifi_lwm2m_smoke")	
		user_demo_src  = ${wildcard $(TARGET_DIR)/Demos/oc_wifi_lwm2m_smoke/*.c}
		user_demo_inc = -I $(TARGET_DIR)/Demos/oc_wifi_lwm2m_smoke
		user_hardware_src = ${wildcard $(TARGET_DIR)/Hardware/E53_SF1/*.c} 
		user_hardware_inc = -I ${wildcard $(TARGET_DIR)/Hardware/E53_SF1}
		user_demo_defs = -D CONFIG_OC_SMOKE_DEMO_ENABLE=1
	endif

	#example for oc_wifi_lwm2m_manhole_cover
	ifeq ($(CONFIG_USER_DEMO), "oc_wifi_lwm2m_manhole_cover")	
		user_demo_src  = ${wildcard $(TARGET_DIR)/Demos/oc_wifi_lwm2m_manhole_cover/*.c}
		user_demo_inc = -I $(TARGET_DIR)/Demos/oc_wifi_lwm2m_manhole_cover
		user_hardware_src = ${wildcard $(TARGET_DIR)/Hardware/E53_SC2/*.c} 
		user_hardware_inc = -I ${wildcard $(TARGET_DIR)/Hardware/E53_SC2}
		user_demo_defs = -D CONFIG_OC_MANHOLE_COVER_DEMO_ENABLE=1
	endif

	#example for oc_wifi_lwm2m_infrared
	ifeq ($(CONFIG_USER_DEMO), "oc_wifi_lwm2m_infrared")	
		user_demo_src  = ${wildcard $(TARGET_DIR)/Demos/oc_wifi_lwm2m_infrared/*.c}
		user_demo_inc = -I $(TARGET_DIR)/Demos/oc_wifi_lwm2m_infrared
		user_hardware_src = ${wildcard $(TARGET_DIR)/Hardware/E53_IS1/*.c} 
		user_hardware_inc = -I ${wildcard $(TARGET_DIR)/Hardware/E53_IS1}
		user_demo_defs = -D CONFIG_OC_INFRARED_DEMO_ENABLE=1
		
	endif
	
#example for wifi mqtt
	
	#example for oc_wifi_mqtt_light
	ifeq ($(CONFIG_USER_DEMO), "oc_wifi_mqtt_light")	
		user_demo_src  = ${wildcard $(TARGET_DIR)/Demos/oc_wifi_mqtt_light/*.c}
		user_demo_inc = -I $(TARGET_DIR)/Demos/oc_wifi_mqtt_light
		user_hardware_src = ${wildcard $(TARGET_DIR)/Hardware/E53_SC1/*.c} 
		user_hardware_inc = -I ${wildcard $(TARGET_DIR)/Hardware/E53_SC1}
	endif

	#example for oc_wifi_mqtt_agriculture
	ifeq ($(CONFIG_USER_DEMO), "oc_wifi_mqtt_agriculture")	
		user_demo_src  = ${wildcard $(TARGET_DIR)/Demos/oc_wifi_mqtt_agriculture/*.c}
		user_demo_inc = -I $(TARGET_DIR)/Demos/oc_wifi_mqtt_agriculture
		user_hardware_src = ${wildcard $(TARGET_DIR)/Hardware/E53_IA1/*.c} 
		user_hardware_inc = -I ${wildcard $(TARGET_DIR)/Hardware/E53_IA1}
	endif

	#example for oc_wifi_mqtt_smoke
	ifeq ($(CONFIG_USER_DEMO), "oc_wifi_mqtt_smoke")	
		user_demo_src  = ${wildcard $(TARGET_DIR)/Demos/oc_wifi_mqtt_smoke/*.c}
		user_demo_inc = -I $(TARGET_DIR)/Demos/oc_wifi_mqtt_smoke
		user_hardware_src = ${wildcard $(TARGET_DIR)/Hardware/E53_SF1/*.c} 
		user_hardware_inc = -I ${wildcard $(TARGET_DIR)/Hardware/E53_SF1}
	endif

	#example for oc_wifi_mqtt_infrared
	ifeq ($(CONFIG_USER_DEMO), "oc_wifi_mqtt_infrared")	
		user_demo_src  = ${wildcard $(TARGET_DIR)/Demos/oc_wifi_mqtt_infrared/*.c}
		user_demo_inc = -I $(TARGET_DIR)/Demos/oc_wifi_mqtt_infrared
		user_hardware_src = ${wildcard $(TARGET_DIR)/Hardware/E53_IS1/*.c} 
		user_hardware_inc = -I ${wildcard $(TARGET_DIR)/Hardware/E53_IS1}
	endif

	#example for oc_wifi_mqtt_manhole_cover
	ifeq ($(CONFIG_USER_DEMO), "oc_wifi_mqtt_manhole_cover")	
		user_demo_src  = ${wildcard $(TARGET_DIR)/Demos/oc_wifi_mqtt_manhole_cover/*.c}
		user_demo_inc = -I $(TARGET_DIR)/Demos/oc_wifi_mqtt_manhole_cover
		user_hardware_src = ${wildcard $(TARGET_DIR)/Hardware/E53_SC2/*.c} 
		user_hardware_inc = -I ${wildcard $(TARGET_DIR)/Hardware/E53_SC2}
	endif

	#example for oc_wifi_mqtt_gps
	ifeq ($(CONFIG_USER_DEMO), "oc_wifi_mqtt_gps")	
		user_demo_src  = ${wildcard $(TARGET_DIR)/Demos/oc_wifi_mqtt_gps/*.c}
		user_demo_inc = -I $(TARGET_DIR)/Demos/oc_wifi_mqtt_gps
		user_hardware_src = ${wildcard $(TARGET_DIR)/Hardware/E53_ST1/*.c} 
		user_hardware_inc = -I ${wildcard $(TARGET_DIR)/Hardware/E53_ST1}
	endif

#example for 2g mqtt
	
	#example for oc_2g_mqtt_light
	ifeq ($(CONFIG_USER_DEMO), "oc_2g_mqtt_light")	
		user_demo_src  = ${wildcard $(TARGET_DIR)/Demos/oc_2g_mqtt_light/*.c}
		user_demo_inc = -I $(TARGET_DIR)/Demos/oc_2g_mqtt_light
		user_hardware_src = ${wildcard $(TARGET_DIR)/Hardware/E53_SC1/*.c} 
		user_hardware_inc = -I ${wildcard $(TARGET_DIR)/Hardware/E53_SC1}
	endif

	#example for oc_2g_mqtt_agriculture
	ifeq ($(CONFIG_USER_DEMO), "oc_2g_mqtt_agriculture")	
		user_demo_src  = ${wildcard $(TARGET_DIR)/Demos/oc_2g_mqtt_agriculture/*.c}
		user_demo_inc = -I $(TARGET_DIR)/Demos/oc_2g_mqtt_agriculture
		user_hardware_src = ${wildcard $(TARGET_DIR)/Hardware/E53_IA1/*.c} 
		user_hardware_inc = -I ${wildcard $(TARGET_DIR)/Hardware/E53_IA1}
	endif

	#example for oc_2g_mqtt_smoke
	ifeq ($(CONFIG_USER_DEMO), "oc_2g_mqtt_smoke")	
		user_demo_src  = ${wildcard $(TARGET_DIR)/Demos/oc_2g_mqtt_smoke/*.c}
		user_demo_inc = -I $(TARGET_DIR)/Demos/oc_2g_mqtt_smoke
		user_hardware_src = ${wildcard $(TARGET_DIR)/Hardware/E53_SF1/*.c} 
		user_hardware_inc = -I ${wildcard $(TARGET_DIR)/Hardware/E53_SF1}
	endif

	#example for oc_2g_mqtt_infrared
	ifeq ($(CONFIG_USER_DEMO), "oc_2g_mqtt_infrared")	
		user_demo_src  = ${wildcard $(TARGET_DIR)/Demos/oc_2g_mqtt_infrared/*.c}
		user_demo_inc = -I $(TARGET_DIR)/Demos/oc_2g_mqtt_infrared
		user_hardware_src = ${wildcard $(TARGET_DIR)/Hardware/E53_IS1/*.c} 
		user_hardware_inc = -I ${wildcard $(TARGET_DIR)/Hardware/E53_IS1}
	endif

	#example for oc_2g_mqtt_manhole_cover
	ifeq ($(CONFIG_USER_DEMO), "oc_2g_mqtt_manhole_cover")	
		user_demo_src  = ${wildcard $(TARGET_DIR)/Demos/oc_2g_mqtt_manhole_cover/*.c}
		user_demo_inc = -I $(TARGET_DIR)/Demos/oc_2g_mqtt_manhole_cover
		user_hardware_src = ${wildcard $(TARGET_DIR)/Hardware/E53_SC2/*.c} 
		user_hardware_inc = -I ${wildcard $(TARGET_DIR)/Hardware/E53_SC2}
	endif

	#example for oc_2g_mqtt_gps
	ifeq ($(CONFIG_USER_DEMO), "oc_2g_mqtt_gps")	
		user_demo_src  = ${wildcard $(TARGET_DIR)/Demos/oc_2g_mqtt_gps/*.c}
		user_demo_inc = -I $(TARGET_DIR)/Demos/oc_2g_mqtt_gps
		user_hardware_src = ${wildcard $(TARGET_DIR)/Hardware/E53_ST1/*.c} 
		user_hardware_inc = -I ${wildcard $(TARGET_DIR)/Hardware/E53_ST1}
	endif

#example for cat1 mqtt
	
	#example for oc_cat1_mqtt_light
	ifeq ($(CONFIG_USER_DEMO), "oc_cat1_mqtt_light")	
		user_demo_src  = ${wildcard $(TARGET_DIR)/Demos/oc_cat1_mqtt_light/*.c}
		user_demo_inc = -I $(TARGET_DIR)/Demos/oc_cat1_mqtt_light
		user_hardware_src = ${wildcard $(TARGET_DIR)/Hardware/E53_SC1/*.c} 
		user_hardware_inc = -I ${wildcard $(TARGET_DIR)/Hardware/E53_SC1}
	endif

	#example for oc_cat1_mqtt_agriculture
	ifeq ($(CONFIG_USER_DEMO), "oc_cat1_mqtt_agriculture")	
		user_demo_src  = ${wildcard $(TARGET_DIR)/Demos/oc_cat1_mqtt_agriculture/*.c}
		user_demo_inc = -I $(TARGET_DIR)/Demos/oc_cat1_mqtt_agriculture
		user_hardware_src = ${wildcard $(TARGET_DIR)/Hardware/E53_IA1/*.c} 
		user_hardware_inc = -I ${wildcard $(TARGET_DIR)/Hardware/E53_IA1}
	endif

	#example for oc_cat1_mqtt_smoke
	ifeq ($(CONFIG_USER_DEMO), "oc_cat1_mqtt_smoke")	
		user_demo_src  = ${wildcard $(TARGET_DIR)/Demos/oc_cat1_mqtt_smoke/*.c}
		user_demo_inc = -I $(TARGET_DIR)/Demos/oc_cat1_mqtt_smoke
		user_hardware_src = ${wildcard $(TARGET_DIR)/Hardware/E53_SF1/*.c} 
		user_hardware_inc = -I ${wildcard $(TARGET_DIR)/Hardware/E53_SF1}
	endif

	#example for oc_cat1_mqtt_infrared
	ifeq ($(CONFIG_USER_DEMO), "oc_cat1_mqtt_infrared")	
		user_demo_src  = ${wildcard $(TARGET_DIR)/Demos/oc_cat1_mqtt_infrared/*.c}
		user_demo_inc = -I $(TARGET_DIR)/Demos/oc_cat1_mqtt_infrared
		user_hardware_src = ${wildcard $(TARGET_DIR)/Hardware/E53_IS1/*.c} 
		user_hardware_inc = -I ${wildcard $(TARGET_DIR)/Hardware/E53_IS1}
	endif

	#example for oc_cat1_mqtt_manhole_cover
	ifeq ($(CONFIG_USER_DEMO), "oc_cat1_mqtt_manhole_cover")	
		user_demo_src  = ${wildcard $(TARGET_DIR)/Demos/oc_cat1_mqtt_manhole_cover/*.c}
		user_demo_inc = -I $(TARGET_DIR)/Demos/oc_cat1_mqtt_manhole_cover
		user_hardware_src = ${wildcard $(TARGET_DIR)/Hardware/E53_SC2/*.c} 
		user_hardware_inc = -I ${wildcard $(TARGET_DIR)/Hardware/E53_SC2}
	endif

	#example for oc_cat1_mqtt_gps
	ifeq ($(CONFIG_USER_DEMO), "oc_cat1_mqtt_gps")	
		user_demo_src  = ${wildcard $(TARGET_DIR)/Demos/oc_cat1_mqtt_gps/*.c}
		user_demo_inc = -I $(TARGET_DIR)/Demos/oc_cat1_mqtt_gps
		user_hardware_src = ${wildcard $(TARGET_DIR)/Hardware/E53_ST1/*.c} 
		user_hardware_inc = -I ${wildcard $(TARGET_DIR)/Hardware/E53_ST1}
	endif

	C_SOURCES += $(user_demo_src)
	C_INCLUDES += $(user_demo_inc)
	C_SOURCES += $(user_hardware_src)
	C_INCLUDES += $(user_hardware_inc)
	C_DEFS += $(user_demo_defs)
