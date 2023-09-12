project('chemskr', 'cpp',
	default_options: ['warning_level=2', 'werror=false', 'cpp_std=c++20'])

flex  = find_program('flex',  required: true)
bison = find_program('bison', required: true)

lgen = generator(flex,
	output: '@BASENAME@.cpp',
	arguments: ['--prefix=chemskr', '-o', '@OUTPUT@', '@INPUT@'])

lfiles = lgen.process('src/chemskr/lexer.l')

pgen = generator(bison,
	output: ['@BASENAME@.cpp', '@BASENAME@.h'],
	arguments: ['@INPUT@', '--defines=@OUTPUT1@', '--output=@OUTPUT0@'])

pfiles = pgen.process('src/chemskr/parser.y')

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
]

sl = static_library('chemskr', chemskr_sources,
	lfiles,
	pfiles,
	override_options: 'unity=off',
	include_directories: include_directories('include'))
