#!/usr/bin/env python

import sys, optparse, subprocess, urllib2, os, os.path, errno, zipfile, string, json, platform, shutil, tarfile, urlparse, tempfile, multiprocessing
from subprocess import Popen, PIPE
import argparse
import sys

WINDOWS = False
if os.name == 'nt':
    WINDOWS = True
    ENVPATH_SEPARATOR = ';'

LINUX = False
if platform.system() == 'Linux':
    LINUX = True
    ENVPATH_SEPARATOR = ':'

OSX = False
if platform.mac_ver()[0] != '':
    OSX = True
    ENVPATH_SEPARATOR = ':'

CPU_CORES = max(multiprocessing.cpu_count(), 1)
PATH = os.getcwd();

PATH_SRC = "~/agrigpspi"

def mkdir_p(path):
    if not os.path.exists(path):
        os.makedirs(path)

def callWithoutPrint(cmdline):
    ret = subprocess.call(cmdline, shell=True)
    if ret != 0:
        print >> sys.stderr, 'Running: ' + cmdline + ' failed with exit code ' + str(ret) + '!'
    return ret

def call(cmdline):
    print 'Running: ' + cmdline
    callWithoutPrint(cmdline)

def mkdir_p(path):
    if not os.path.exists(path):
        os.makedirs(path)

class AgriGpsPi(object):

    def __init__(self):
        parser = argparse.ArgumentParser( description='WeedVision');
        parser.add_argument('command', help='Subcommand to run'
                , choices=['run', 'clear', 'info', 'install', 'config_git', 'install_vim', 'install_platformio', 'install_desktop', 'piece3d', 'vim_f5', 'vim_f6', 'vim_f7', 'vim_f8']);
        args = parser.parse_args(sys.argv[1:2])
        if not hasattr(self, args.command):
            print 'Unrecognized command'
            parser.print_help()
            exit(1)
        getattr(self, args.command)()


    def info(self):
        print("path " + str(PATH))
        print("nbr cores " + str(CPU_CORES))
        if(WINDOWS):
            print("WINDOWS")
        if(LINUX):
            print("LINUX")
        if(OSX):
            print("OSX")

    def install_platformio(self):
        call('python3 -c "$(curl -fsSL https://raw.githubusercontent.com/platformio/platformio/develop/scripts/get-platformio.py)"')


    def install_vim(self):
        call('sudo apt-get install -y vim')
        call('git clone https://github.com/VundleVim/Vundle.vim.git ~/.vim/bundle/Vundle.vim')
        call('cp ~/weedvision/install/vimrc ~/.vimrc')

    def install_desktop(self):
        call('sudo cp ' + PATH_SRC + '/install/agri_gps_pi.desktop ~/Desktop')
        call('sudo chmod +x ~/Desktop/agri_gps_pi.desktop')
        #debug dex weedvision.desktop

    def config_git(self):
        call('git config --global user.name \"Clement Lemaire\"')
        call('git config --global user.email lemairec02@gmail.com')
        call('git config --global push.default simple')
        call('git config --global core.editor vi')
        call('git config --global color.ui')

    def install(self):
        if OSX:
            call('brew install cmake')
            #call('brew tap homebrew/science')
            #call('brew install opencv')
            call('brew install qt5')
            #call('brew install fftw')
            #call('brew install libharu')
            call('brew install boost')
            #call('brew install boost-python')
            #call('brew install fluid-synth')
        else:
            call('sudo apt-get update')
            call('sudo apt-get install -y git')
            call('sudo apt-get install -y build-essential')
            call('sudo apt-get install -y cmake')
            call('sudo apt-get install -y libqt5serialport5-dev')
            #call('sudo apt-get install -y gdb')
            #call('sudo apt-get install -y valgrind')

            #call('sudo apt-get install -y libcv-dev')
            #call('sudo apt-get install -y libopencv-dev')
            #call('sudo apt-get install -y libhighgui-dev')
            call('sudo apt-get install -y qtmultimedia5-dev')
            call('sudo apt-get install -y libboost-dev')
            call('sudo apt-get install -y libboost-system-dev')
            call('sudo apt-get install -y libboost-thread-dev')
            #call('sudo apt-get install -y exuberant-ctags')
            #call('sudo apt-get install -y gitg gitk')
            #call('sudo apt-get install -y cmake-gui')
        #self.config_git()
        #self.install_vim()
        self.clear()


    def run(self):
        if OSX:
            print "warning OSX"
            return
        s = ""
        for i in range(2,len(sys.argv)):
            s += " " + sys.argv[i]
        print "run " + s
        call('cd ' + PATH_SRC + ';mkdir -p '+PATH_SRC+'/build')
        call('cd ' + PATH_SRC + '/build && cmake .. && make -j '+ str(CPU_CORES)
            + ' && ' + PATH_SRC + '/build/agri_gps_pi ' + s);

    def clear(self):
        if OSX:
            print "warning OSX"
            return
        call('rm -rf ' + PATH_SRC + '/build')
        call('find ' + PATH_SRC + ' -name \"*.swp\" -exec rm -rf {} \\;; find ~/weedvision -name \"*~\" -exec rm -rf {} \\;;')
        self.run()

    def setup(self):
        call('cd ' + PATH_SRC)
        self.install_desktop()
        self.install()

    def piece3d(self):
        call('python ~/weedvision/piece3d/piece3d.py')

    def vim_f5(self):
        self.run()

    def vim_f6(self):
        self.setup()


if __name__ == '__main__':
    AgriGpsPi()
