function(fwl_add_executable targetname src depends libs)
	add_executable(${targetname} ${src})
	add_dependencies(${targetname} ${depends})
	target_link_libraries(${targetname} ${libs})
endfunction()


