newaction {
   trigger     = "indent",
   description = "Format source files",
   execute = function ()
      os.execute("astyle -N -A1 -R '*.c' '*.h'");
   end
}
newaction {
    trigger     = "install",
    description = "Install headers and lib",
    execute = function() 

      if not _ARGS[1] or not os.isdir(_ARGS[1]) then
        error("You must specify an install location")
      end

      if not os.isfile("bin/release/libcparse.a") then
        error("You must make a release build first")
      end

      bindir = _ARGS[1].."/lib"
      headerdir = _ARGS[1].."/include/cparse"

      if not os.isdir(bindir) then
        os.mkdir(bindir)
      end

      if not os.isdir(headerdir) then
        os.mkdir(headerdir)
      end

      fos.copyfile("bin/release/libcparse.a", bindir)

      headers = os.matchfiles("cparse/*.h")
      tempdir = headerdir.."/cparse"
      os.mkdir(tempdir)
      for i=1, #headers do
        os.copyfile(headers[i], tempdir)
      end
    end
}

if _ACTION == "clean" then
    matches = os.matchfiles("**.orig")
    for i=1, #matches do
        os.remove(matches[i])
    end
    os.rmdir("bin")
    os.rmdir("obj")
end


solution "cparse"
    configurations { "Debug", "Release" }

    language "C"

    buildoptions { "-Wall", "-Werror", "-std=c89", "-I." }

    configuration "Debug"
        flags "Symbols"
        targetdir "bin/debug"
        buildoptions { "-g" }
    configuration "Release"
        targetdir "bin/release"
        buildoptions { "-O" }
    
    project "cparse"
        kind "StaticLib"
        files {
            "**.c",
            "**.h"
        }
        excludes {
            "**.test.c"
        }
    
    project "cparsetest"
        kind "ConsoleApp"
        files {
            "**.test.c"
        }
        links { "cparse", "check", "curl", "json"}
        
        configuration "Debug"
            postbuildcommands {
              "./bin/debug/cparsetest"
            }
        configuration "Release"
            postbuildcommands {
              "./bin/release/cparsetest"
            }

