release_env = Environment(variant = 'release')
release_env.Append(CCFLAGS=['-O3', '-march=pentium4'])

debug_env = Environment(variant = 'debug')
debug_env.Append(CCFLAGS=['-g','-O0'])
debug_env.Append(CPPDEFINES=['_DEBUG'])

SConscript('SConscript', variant_dir="Release", exports={'env':release_env}, duplicate=0)
SConscript('SConscript', variant_dir="Debug", exports={'env':debug_env}, duplicate=0)

