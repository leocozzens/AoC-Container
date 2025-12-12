from os.path import exists, getmtime

def check_args(command: list[str], args: str):
    argList = args.split(" ")
    if len(argList) <= 1:
        return
    for arg in argList:
        command.append(arg)

def should_compile(sourcePath: str, execPath: str) -> bool:
    if not exists(execPath):
        return True
    return getmtime(sourcePath) >= getmtime(execPath)