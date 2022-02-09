
set(include_dirs app/include oneNET oneNET/cipher)
file(GLOB_RECURSE srcs app/src/*.c oneNET/*.c   oneNET/cipher/*.c)

idf_component_register(SRCS ${srcs} 
                    INCLUDE_DIRS ${include_dirs}
                    REQUIRES mbedtls nvs_flash protocol_examples_common mqtt 
                     )
