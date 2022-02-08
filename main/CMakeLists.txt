file(GLOB_RECURSE srcs app/src/*.c oneNET/*.c)
idf_component_register(SRCS "main.c" ../componnects/app/src/app_mqtt_code.c
                    INCLUDE_DIRS "." ../componnects/app/include          
                    )
