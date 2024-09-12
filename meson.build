project('chemskr', 'cpp',
	default_options: ['warning_level=2', 'werror=false', 'cpp_std=c++23'])

flex  = find_program('flex',  required: true)
bison = find_program('bison', required: true)

lexer_src = custom_target('gen-lexer',
	input: 'src/chemskr/lexer.l',
	output: 'flexer.cpp',
	build_by_default: true,
	command: [flex, '--prefix=chemskr', '-o', '@OUTPUT@', '@INPUT@'])

parser_src = custom_target('gen-parser',
	input: 'src/chemskr/parser.y',
	output: ['formula.cpp', 'formula.h'],
	build_by_default: true,
	command: [bison, '@INPUT@', '--defines=@OUTPUT1@', '--output=@OUTPUT0@'],
	install_dir: get_option('includedir'),
	install: true
)

chemskr_sources = [
	'src/chemskr/ASTNode.cpp',
	'src/chemskr/Chemskr.cpp',
	'src/chemskr/Globals.cpp',
	'src/chemskr/Lexer.cpp',
	'src/chemskr/Nuclear.cpp',
	'src/chemskr/NuclideMasses.cpp',
	'src/chemskr/Parser.cpp',
	'src/chemskr/StringSet.cpp',
	'src/chemskr/Utils.cpp',
	lexer_src[0],
	parser_src[0]
]

inc_dirs = include_directories('include')

if build_machine.system() == 'windows'
	cpp_args = ['-Dfileno=_fileno', '-D_POSIX_SOURCE']
	add_project_arguments(cpp_args, language: 'cpp')
endif

chemskr_lib = static_library('chemskr', chemskr_sources,
	override_options: 'unity=off',
	include_directories: inc_dirs)

chemskr = declare_dependency(
	sources: [parser_src[1]],
	include_directories: inc_dirs,
	link_with: chemskr_lib
)
