#
# (C) 2014 KATO Takeshi
#

VERSION = '0.0.1.0'
APPNAME = 'microtree'

top = '.'
out = 'build'

def options(x):
	x.load('compiler_cxx')

def configure(x):
	x.load('compiler_cxx flex bison')
	x.env.append_value('LIB', 'fl')

def build(x):
	x.recurse('source')
