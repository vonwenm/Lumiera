# -*- python -*-
##
## Platform.py  -  SCons build: platform configuration and library detection
##

#  Copyright (C)         Lumiera.org
#    2012,               Hermann Vosseler <Ichthyostega@web.de>
#
#  This program is free software; you can redistribute it and/or
#  modify it under the terms of the GNU General Public License as
#  published by the Free Software Foundation; either version 2 of
#  the License, or (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#####################################################################


# NOTE: scons -h for help.
# Read more about the SCons build system at: http://www.scons.org
# Basically, this script just /defines/ the components and how they
# fit together. SCons will derive the necessary build steps.



import os
import sys

from SCons.Script import Exit

from Buildhelper import *
from LumieraEnvironment import *





def configurePlatform(env):
    """ locate required libs.
        setup platform specific options.
        Abort build in case of failure.
    """
    conf = env.Configure()
    # run all configuration checks in the given env
    
    # Perform checks for prerequisites --------------------------------------------
    problems = []
    if not conf.TryAction('pkg-config --version > $TARGET')[0]:
        problems.append('We need pkg-config for including library configurations, exiting.')
    
    if not conf.CheckLibWithHeader('m', 'math.h','C'):
        problems.append('Did not find math.h / libm.')
    
    if not conf.CheckLibWithHeader('dl', 'dlfcn.h', 'C'):
        problems.append('Functions for runtime dynamic loading not available.')
    
    if not conf.CheckLibWithHeader('pthread', 'pthread.h', 'C'):
        problems.append('Did not find the pthread lib or pthread.h.')
    else:
       conf.env.Append(CPPFLAGS = ' -DHAVE_PTHREAD')
       conf.env.Append(CCFLAGS = ' -pthread')
    
    if conf.CheckCHeader('execinfo.h'):
       conf.env.Append(CPPFLAGS = ' -DHAVE_EXECINFO_H')
    
    if conf.CheckCHeader('valgrind/valgrind.h'):
        conf.env.Append(CPPFLAGS = ' -DHAVE_VALGRIND_H')
    else:
        print 'Valgrind not found. The use of Valgrind is optional; building without.'
    
    if not conf.CheckPkgConfig('nobugmt', 201006.1):
        problems.append('Did not find NoBug [http://www.lumiera.org/nobug_manual.html].')
    else:
        conf.env.mergeConf('nobugmt')
    
    if not conf.CheckCXXHeader('tr1/memory'):
        problems.append('We rely on the std::tr1 standard C++ extension for shared_ptr.')
    
    if not conf.CheckCXXHeader('boost/config.hpp'):
        problems.append('We need the C++ boost-libraries.')
    else:
        if not conf.CheckCXXHeader('boost/scoped_ptr.hpp'):
            problems.append('We need boost::scoped_ptr (scoped_ptr.hpp).')
        if not conf.CheckCXXHeader('boost/format.hpp'):
            problems.append('We need boost::format (header).')
        if not conf.CheckLibWithHeader('boost_program_options-mt','boost/program_options.hpp','C++'):
            problems.append('We need boost::program_options (including binary lib for linking).')
        if not conf.CheckLibWithHeader('boost_system-mt','boost/system/error_code.hpp','C++'):
            problems.append('We need the boost::system support library (including binary lib).')
        if not conf.CheckLibWithHeader('boost_filesystem-mt','boost/filesystem.hpp','C++'):
            problems.append('We need the boost::filesystem lib (including binary lib for linking).')
        if not conf.CheckLibWithHeader('boost_regex-mt','boost/regex.hpp','C++'):
            problems.append('We need the boost regular expression lib (incl. binary lib for linking).')
    
    
    if conf.CheckLib(symbol='clock_gettime'):
        print 'Using function clock_gettime() as defined in the C-lib...'
    else:
        if not conf.CheckLib(symbol='clock_gettime', library='rt'):
            problems.append('No library known to provide the clock_gettime() function.')
    
    if not conf.CheckPkgConfig('gavl', 1.0):
        problems.append('Did not find Gmerlin Audio Video Lib [http://gmerlin.sourceforge.net/gavl.html].')
    else:
        conf.env.mergeConf('gavl')
    
    if not conf.CheckPkgConfig('gtkmm-2.4', 2.8):
        problems.append('Unable to configure GTK--')
        
    if not conf.CheckPkgConfig('glibmm-2.4', '2.16'):
        problems.append('Unable to configure Lib glib--')
    
    if not conf.CheckPkgConfig('gthread-2.0', '2.12.4'):
        problems.append('Need gthread support lib for glib-- based thread handling.')
    
    if not conf.CheckPkgConfig('cairomm-1.0', 0.6):
        problems.append('Unable to configure Cairo--')
    
    verGDL = '2.27.1'
    if not conf.CheckPkgConfig('gdl-1.0', verGDL, alias='gdl'):
        print 'No sufficiently recent (>=%s) version of GDL found. Maybe use custom package gdl-lum?' % verGDL
        if not conf.CheckPkgConfig('gdl-lum', verGDL, alias='gdl'):
            problems.append('GNOME Docking Library not found. We either need a sufficiently recent GDL '
                            'version (>=%s), or the custom package "gdl-lum" from Lumiera.org.' % verGDL)
    
    if not conf.CheckPkgConfig('librsvg-2.0', '2.18.1'):
        problems.append('Need rsvg Library for rendering icons.')
        
    if not conf.CheckCHeader(['X11/Xutil.h', 'X11/Xlib.h'],'<>'):
        problems.append('Xlib.h and Xutil.h required. Please install libx11-dev.')
    
    if not conf.CheckPkgConfig('xv')  : problems.append('Need libXv...')
    if not conf.CheckPkgConfig('xext'): problems.append('Need libXext.')
    
    
    # report missing dependencies
    if problems:
        print "*** unable to build due to the following problems:"
        for isue in problems:
            print " *  %s" % isue
        print
        print "build aborted."
        Exit(1)
    
    print "** Gathered Library Info: %s" % conf.env.libInfo.keys()
    
    
    # create new env containing the finished configuration
    return conf.Finish()

