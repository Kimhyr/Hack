add_rules('mode.debug', 'mode.release')

set_warnings('everything')
set_toolchains('clang')
set_languages('c++23')

target('test')
	set_kind('binary')
	add_files('source/*.cpp', 'source/*.cppm')
