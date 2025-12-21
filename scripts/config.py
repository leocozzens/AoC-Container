import os, sys, subprocess, __main__
from os.path import abspath, dirname, join, isfile, splitext, basename
from dataclasses import dataclass
import scripts.util as util

@dataclass
class Program:
    source: str
    exec: str
    command: list[str]

class Config:
    CC            = "gcc"
    STANDARD      = "-std=c17"
    DEBUG_FLAGS   = "-g -O0 -Wall -Werror"
    RELEASE_FLAGS = "-O2 -DNDEBUG"
    SOURCE_EXT    = ".c"

    def __init__(self, debug: bool):
        if sys.platform.startswith("win"):
            self.platformFlags = ""
            self.exe = ".exe"
        elif sys.platform.startswith("linux"):
            self.platformFlags = ""
            self.exe = ".bin"
        else:
            raise Exception("Unknown platform")
        self._load_paths()
        self.flags = Config.DEBUG_FLAGS if debug else Config.RELEASE_FLAGS
        self.debug = debug
        self.includes = f"-I{self.srcDir} -I{self.includeDir}"
        self.libs = ""
        self.load_programs()
        self.create_folders()

    def _load_paths(self):
        self.parentDir    = dirname(abspath(__main__.__file__))
        self.binDir       = join(self.parentDir, "bin")
        self.srcDir       = join(self.parentDir, "src")
        self.utilDir      = join(self.srcDir, "util")
        self.challengeDir = join(self.srcDir, "challenges")
        self.includeDir   = join(self.parentDir, "include")
    
    def _create_command(self, cmdIn: str, cmdOut: str) -> list[str]:
        command = [
            Config.CC,
            Config.STANDARD,
            cmdIn,
            *self.utils,
            "-o",
            cmdOut,
        ]
        util.check_args(command, self.flags)
        util.check_args(command, self.includes)
        util.check_args(command, self.libs)
        util.check_args(command, self.platformFlags)
        return command

    def load_programs(self):
        self.utils: list[str] = []
        for file in os.listdir(self.utilDir):
            file = join(self.utilDir, file)
            if isfile(file) and file.endswith(Config.SOURCE_EXT):
                self.utils.append(file)
        self.programs: list[Program] = []
        for file in os.listdir(self.challengeDir):
            sourcePath = join(self.challengeDir, file)
            if isfile(sourcePath) and file.endswith(Config.SOURCE_EXT):
                execPath = join(self.binDir, splitext(file)[0] + self.exe)
                fullCommand = self._create_command(sourcePath, execPath)
                self.programs.append(Program(sourcePath, execPath, fullCommand))
        if len(self.programs) == 0:
            raise Exception(f"No files that end with {Config.SOURCE_EXT} in {self.challengeDir}")

    def create_folders(self):
        os.makedirs(self.binDir, exist_ok=True)

    def run(self, compileAll: bool, printCommands: bool):
        pre = '\n' if printCommands else ""
        compileList: list[Program]
        if compileAll:
            compileList = self.programs
        else:
            compileList = []
            for program in self.programs:
                if util.should_compile(program.source, program.exec):
                    compileList.append(program)
        if len(compileList) == 0:
            print("📁 No executables to compile.")
            return

        for program in compileList:
            print(f"{pre}🔨 Compiling {basename(program.source)} to {basename(program.exec)}")
            if printCommands:
                print(*program.command, sep=' ')
            subprocess.run(program.command, shell=True)

    def print(self):
        for program in self.programs:
            print(*program.command, sep=' ')