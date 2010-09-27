lib_target  = 'hello'
subdirs = Split("""
src
src/detail
src/util
""")

DEBUG = True

env = Environment()
env.Append(CPPPATH = ['include', '/usr/include/python3.1'])
env.Append(LIBS = ['boost_python3'])
env.Append(CCFLAGS = ['-Wall', '-fmessage-length=0'])
if DEBUG:
	env.Append(CCFLAGS=['-g','-O0'])
	env.Append(CPPDEFINES=['_DEBUG'])
else:
	env.Append(CCFLAGS=['-O'])

globpatterns = [s+'/*.cpp' for s in subdirs]
filelist = map(Glob, globpatterns)
files = reduce(lambda x,y:x+y, filelist)

lib = env.SharedLibrary(target = lib_target, source = files)
Default(lib)

