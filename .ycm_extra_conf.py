#If the path of current project include any chinese word, then YCM can not find the ".ycm_extra_conf.py" file in current path. 

import os
import ycm_core
 
flags = [ 
    '-Wno-deprecated-declarations', 
    '-Wno-disabled-macro-expansion', 
    '-Wno-c++98-compat', 
    '-Wno-c++98-compat-pedantic', 
    '-Wno-global-constructors', 
    '-Wno-exit-time-destructors', 
    '-Wno-missing-prototypes', 
    '-Wno-padded',

    '-Wunused',
    '-Wuninitialized',
    '-Wall',
    '-Wextra',
    '-Wpointer-arith',
    '-Wshadow',
    '-Wfloat-equal', 

    '-x', 'c++',
    '-std=gnu++11',

    '-isystem', r'E:/ProgramFiles/GNU Tools ARM Embedded/5.2 2015q4/arm-none-eabi/include',
    '-isystem', r'E:/ProgramFiles/GNU Tools ARM Embedded/5.2 2015q4/arm-none-eabi/include/c++/5.2.1',
    '-isystem', r'E:/ProgramFiles/GNU Tools ARM Embedded/5.2 2015q4/arm-none-eabi/include/c++/5.2.1/arm-none-eabi/armv7-m',
    '-isystem', r'E:/ProgramFiles/GNU Tools ARM Embedded/5.2 2015q4/arm-none-eabi/include/c++/5.2.1/backward',
    '-isystem', r'E:/ProgramFiles/GNU Tools ARM Embedded/5.2 2015q4/lib/gcc/arm-none-eabi/5.2.1/include',
    '-isystem', r'E:/ProgramFiles/GNU Tools ARM Embedded/5.2 2015q4/lib/gcc/arm-none-eabi/5.2.1/include-fixed',

    '-I', r'.',
    '-I', r'../scm_stm32f0xx',
    '-I', r'../include',
    '-I', r'../../stm32f051_stdPeriph_lib/system/include',
    '-I', r'../../stm32f051_stdPeriph_lib/system/include/arm',
    '-I', r'../../stm32f051_stdPeriph_lib/system/include/cmsis',
    '-I', r'../../stm32f051_stdPeriph_lib/system/include/cortexm',
    '-I', r'../../stm32f051_stdPeriph_lib/system/include/diag',
    '-I', r'../../stm32f051_stdPeriph_lib/system/include/stm32f0-stdperiph/',

    '-D__GNUC__',
    '-DDEBUG',
    '-DHSE_VALUE=8000000',
    '-DOS_USE_TRACE_SEMIHOSTING_DEBUG',
    '-DSTM32F051',
    '-DTRACE',
    '-DUSE_FULL_ASSERT',
    '-DUSE_STDPERIPH_DRIVER',
] 
compilation_database_folder = '' 
if compilation_database_folder: 
  database = ycm_core.CompilationDatabase( compilation_database_folder ) 
else: 
  database = None 
SOURCE_EXTENSIONS = [ '.cpp', '.cxx', '.cc', '.c', '.m', '.mm' ] 
def DirectoryOfThisScript(): 
  return os.path.dirname( os.path.abspath( __file__ ) ) 
def MakeRelativePathsInFlagsAbsolute( flags, working_directory ): 
  if not working_directory: 
    return list( flags ) 
  new_flags = [] 
  make_next_absolute = False 
  path_flags = [ '-isystem', '-I', '-iquote', '--sysroot=' ] 
  for flag in flags: 
    new_flag = flag 
    if make_next_absolute: 
      make_next_absolute = False 
      if not flag.startswith( '/' ): 
        new_flag = os.path.join( working_directory, flag ) 
    for path_flag in path_flags: 
      if flag == path_flag: 
        make_next_absolute = True 
        break 
      if flag.startswith( path_flag ): 
        path = flag[ len( path_flag ): ] 
        new_flag = path_flag + os.path.join( working_directory, path ) 
        break 
    if new_flag: 
      new_flags.append( new_flag ) 
  return new_flags 
def IsHeaderFile( filename ): 
  extension = os.path.splitext( filename )[ 1 ] 
  return extension in [ '.h', '.hxx', '.hpp', '.hh' ] 
def GetCompilationInfoForFile( filename ): 
  if IsHeaderFile( filename ): 
    basename = os.path.splitext( filename )[ 0 ] 
    for extension in SOURCE_EXTENSIONS: 
      replacement_file = basename + extension 
      if os.path.exists( replacement_file ): 
        compilation_info = database.GetCompilationInfoForFile(          replacement_file )
        if compilation_info.compiler_flags_: 
          return compilation_info 
    return None 
  return database.GetCompilationInfoForFile( filename ) 
def FlagsForFile( filename, **kwargs ): 
  if database: 
    compilation_info = GetCompilationInfoForFile( filename ) 
    if not compilation_info: 
      return None 
    final_flags = MakeRelativePathsInFlagsAbsolute( 
      compilation_info.compiler_flags_, 
      compilation_info.compiler_working_dir_ ) 
  else: 
    relative_to = DirectoryOfThisScript() 
    final_flags = MakeRelativePathsInFlagsAbsolute( flags, relative_to ) 
  return { 
    'flags': final_flags, 
    'do_cache': True 
  }
