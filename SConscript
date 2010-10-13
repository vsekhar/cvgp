# add additional user code [sub]directories to this list, relative to the repository root

usrdirs = Split("""
usr
""")

### END USER MODIFIABLES ###

lib_target  = 'vgp'
subdirs = Split("""
src
src/detail
src/library
src/python
src/util
""")
subdirs.append(usrdirs)

Import('env')
env.Append(CPPPATH = ['include', '/usr/include/python3.1'])
env.Append(LIBS = ['boost_python3'])
env.Append(CCFLAGS = ['-Wall', '-fmessage-length=0'])

globpatterns = [s+'/*.cpp' for s in subdirs]
filelist = map(Glob, globpatterns)
files = reduce(lambda x,y:x+y, filelist)

lib = env.SharedLibrary(target = lib_target, source = files)
Default(lib)

