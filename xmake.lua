add_rules('mode.debug', 'mode.release')

set_warnings('everything')
set_toolchains('clang')
set_languages('c++23')

add_cxxflags('-Wno-c++98-compat', '-Wno-pre-c++20-compat', '-fimplicit-modules', '-fimplicit-module-maps', '-stdlib=libc++')

add_includedirs('.')

if is_mode('debug') then
	set_symbols('debug')
	set_optimize('none')
else
	set_symbols('none')
end

target('hack')
	set_kind('binary')
	add_files('source/**.cppm', 'source/**.cpp')
