add_rules('mode.debug', 'mode.release')

set_warnings('everything')
set_toolchains('clang')
set_languages('c++23')

add_includedirs('.')

if is_mode('debug') then
	set_symbols('debug')
	set_optimize('none')
else
	set_symbols('none')
end

target('hack')
	set_kind('binary')
	add_files('hack/**.cppm', 'hack/**.cpp')

target('u')
	set_kind('static')
	add_files('u/**.cppm', 'u/**.cpp')

target('test')
	set_kind('binary')
	add_files('test/**.cpp')
