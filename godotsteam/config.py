def can_build(env, platform):
  return platform == "linuxbsd" or platform == "windows" or platform == "osx" or platform=="server"

def configure(env):
  pass
