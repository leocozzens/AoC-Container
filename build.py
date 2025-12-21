from scripts.config import Config

def main():
    build = Config(True)
    build.run(True, False)

if __name__ == "__main__":
    main()