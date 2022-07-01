# Include headers
include_directories(${WaterSimulation_SOURCE_DIR}/libconfig/inc)

# Output library
add_library(libconfig	libconfig.c
						grammar.c
						scanctx.c
						scanner.c
						strbuf.c
						strvec.c
						util.c)