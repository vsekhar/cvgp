usrdir = "examples"

### END USER MODIFIABLES ###

lib_target  = 'vgp'
subdirs = Split("""
src
src/detail
src/library
src/util
""")
subdirs.append(usrdir)

Import('env')
env.Append(CPPPATH = ['include', '/usr/include/python3.1'])
env.Append(LIBS = ['boost_python3'])
env.Append(CCFLAGS = ['-Wall', '-fmessage-length=0'])

globpatterns = [s+'/*.cpp' for s in subdirs]
filelist = map(Glob, globpatterns)
files = reduce(lambda x,y:x+y, filelist)

lib = env.SharedLibrary(target = lib_target, source = files)
Default(lib)

