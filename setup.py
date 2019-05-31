# © Leo Tamminen. Do not use without explicit permission.
# shophorn@protonmail.com

# Codetest program for job application
# Python c extension setup.

from distutils.core import setup, Extension

sources = [
	"src/codetest.c",
	"src/vector_functions.c"
]

module = Extension('codetest', sources)

setup(
	name = 'codetest',
   	ext_modules = [module]
)