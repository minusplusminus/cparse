newaction {
   trigger     = "indent",
   description = "Format source files",
   execute = function ()
      os.execute("astyle -N -A1 -R '*.cpp' '*.h'");
   end
}
newaction {
    trigger   = "release",
    description = "builds a release",
    execute = function()
       if( os.isdir("bin")) then
          error("You must run clean first")
       end

       if _ARGS[1] then
         version = _ARGS[1]
       else
         version = "1.0"
       end
       os.execute("tar --exclude=legacy --exclude=cparse_"..version..".tar.gz -czf cparse_"..version..".tar.gz .");
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

    language "C++"

    buildoptions { "-Wall", "-Werror", "-std=c++11", "-stdlib=libc++", "-I." }
    linkoptions { "-stdlib=libc++" }

    configuration "Debug"
        flags "Symbols"
        targetdir "bin/debug"
        buildoptions { "-g" }
    configuration "Release"
        targetdir "bin/release"
        buildoptions { "-O" }
    
    project "cparse"
        kind "SharedLib"
        files {
            "**.cpp",
            "**.h"
        }
        excludes {
            "**.test.cpp"
        }
        links {
            "arg3", "curl"
        }
    
    configuration "Debug"
    
    project "cparsetest"
        kind "ConsoleApp"
        files {
            "**.test.cpp"
        }
        links { "cparse", "arg3" }
        
        postbuildcommands {
          "./bin/debug/cparsetest"
        }


