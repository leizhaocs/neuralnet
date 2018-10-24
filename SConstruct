#!/usr/bin/python

import os
import glob

env = Environment(ENV = os.environ)

env.Append(CPPPATH = ['src', 'src/data', 'src/nsga-ii', 'include'])
env.Append(CPPFLAGS = ['-std=c++1z', '-fopenmp', '-O3'])
env.Append(LINKFLAGS = ['-fopenmp'])

VariantDir('obj', 'src', duplicate = 0)
src_files = glob.glob('src/*.cpp')
src_files += glob.glob('src/data/*.cpp')
src_files += glob.glob('src/nsga-ii/*.cpp')

for i in range(len(src_files)):
  src_files[i] = src_files[i].replace('src', 'obj')

env.Program('nn', src_files, LIBS=['seal'], LIBPATH='lib')
