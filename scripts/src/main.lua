-- license:BSD-3-Clause
-- copyright-holders:MAMEdev Team

---------------------------------------------------------------------------
--
--   main.lua
--
--   Rules for building main binary
--
---------------------------------------------------------------------------

function mainProject(_target, _subtarget)
if (_OPTIONS["SOURCES"] == nil) then
	if (_target == _subtarget) then
		project (_target)
	else
		if (_subtarget=="mess") then
			project (_subtarget)
		else
			project (_target .. _subtarget)
		end
	end
else
	project (_subtarget)
end
	uuid (os.uuid(_target .."_" .. _subtarget))
	kind "ConsoleApp"

	configuration { "android*" }
		targetprefix "lib"
		targetname "main"
		targetextension ".so"
		linkoptions {
			"-shared",
			"-Wl,-soname,libmain.so"
		}
		links {
			"EGL",
			"GLESv1_CM",
			"GLESv2",
			"SDL2",
		}

	configuration {  }

	addprojectflags()
	flags {
		"NoManifest",
		"Symbols", -- always include minimum symbols for executables
	}

	if _OPTIONS["SYMBOLS"]~=nil and _OPTIONS["SYMBOLS"]~=0 and (_OPTIONS["PDB_SYMBOLS"]==nil or _OPTIONS["PDB_SYMBOLS"]==0) then
		local llvm_obdjump = false
		local objdump_ver = backtick('objdump --version')
		if string.match(objdump_ver, 'LLVM version ') then
			llvm_obdjump = true
		end

		configuration { "mingw*" }
			if llvm_obdjump then
				postbuildcommands {
					"$(SILENT) echo Dumping symbols.",
					"$(SILENT) " .. PYTHON .. " " .. MAME_DIR .. "scripts/build/llvm-objdump-filter.py $(TARGET) | c++filt >$(subst .exe,.sym,$(TARGET))"
				}
			else
				postbuildcommands {
					"$(SILENT) echo Dumping symbols.",
					"$(SILENT) objdump --section=.text --syms --demangle $(TARGET) >$(subst .exe,.sym,$(TARGET))"
				}
			end
	end

	configuration { "Release" }
		targetsuffix ""
		if _OPTIONS["PROFILE"] then
			targetsuffix "p"
		end

	configuration { "Debug" }
		targetsuffix "d"
		if _OPTIONS["PROFILE"] then
			targetsuffix "dp"
		end

	configuration { "mingw*" or "vs20*" }
		targetextension ".exe"

	configuration { "asmjs" }
		targetextension ".html"

	configuration { }

	if _OPTIONS["targetos"]=="android" then
		files {
			MAME_DIR .. "src/osd/sdl/android_main.cpp",
		}
		targetsuffix ""
		if _OPTIONS["SEPARATE_BIN"]~="1" then
			if _OPTIONS["PLATFORM"]=="arm" then
				targetdir(MAME_DIR .. "android-project/app/src/main/libs/armeabi-v7a")
				os.copyfile(_OPTIONS["SDL_INSTALL_ROOT"] .. "/lib/libSDL2.so", MAME_DIR .. "android-project/app/src/main/libs/armeabi-v7a/libSDL2.so")
				os.copyfile(androidToolchainRoot() .. "/sysroot/usr/lib/arm-linux-androideabi/libc++_shared.so", MAME_DIR .. "android-project/app/src/main/libs/armeabi-v7a/libc++_shared.so")
			end
			if _OPTIONS["PLATFORM"]=="arm64" then
				targetdir(MAME_DIR .. "android-project/app/src/main/libs/arm64-v8a")
				os.copyfile(_OPTIONS["SDL_INSTALL_ROOT"] .. "/lib/libSDL2.so", MAME_DIR .. "android-project/app/src/main/libs/arm64-v8a/libSDL2.so")
				os.copyfile(androidToolchainRoot() .. "/sysroot/usr/lib/aarch64-linux-android/libc++_shared.so", MAME_DIR .. "android-project/app/src/main/libs/arm64-v8a/libc++_shared.so")
			end
			if _OPTIONS["PLATFORM"]=="x86" then
				targetdir(MAME_DIR .. "android-project/app/src/main/libs/x86")
				os.copyfile(_OPTIONS["SDL_INSTALL_ROOT"] .. "/lib/libSDL2.so", MAME_DIR .. "android-project/app/src/main/libs/x86/libSDL2.so")
				os.copyfile(androidToolchainRoot() .. "/sysroot/usr/lib/i686-linux-android/libc++_shared.so", MAME_DIR .. "android-project/app/src/main/libs/x86/libc++_shared.so")
			end
			if _OPTIONS["PLATFORM"]=="x64" then
				targetdir(MAME_DIR .. "android-project/app/src/main/libs/x86_64")
				os.copyfile(_OPTIONS["SDL_INSTALL_ROOT"] .. "/lib/libSDL2.so", MAME_DIR .. "android-project/app/src/main/libs/x86_64/libSDL2.so")
				os.copyfile(androidToolchainRoot() .. "/sysroot/usr/lib/x86_64-linux-android/libc++_shared.so", MAME_DIR .. "android-project/app/src/main/libs/x86_64/libc++_shared.so")
			end
		end
	else
		if _OPTIONS["SEPARATE_BIN"]~="1" then
			targetdir(MAME_DIR)
		end
	end

if (STANDALONE~=true) then
	findfunction("linkProjects_" .. _OPTIONS["target"] .. "_" .. _OPTIONS["subtarget"])(_OPTIONS["target"], _OPTIONS["subtarget"])
end
	links {
		"osd_" .. _OPTIONS["osd"],
	}
	links {
		"qtdbg_" .. _OPTIONS["osd"],
	}
if (STANDALONE~=true) then
	links {
		"frontend",
	}
end
	links {
		"optional",
		"emu",
	}
--if (STANDALONE~=true) then
	links {
		"formats",
	}
--end
if #disasm_files > 0 then
	links {
		"dasm",
	}
end
if (MACHINES["NETLIST"]~=null) then
	links {
		"netlist",
	}
end
	links {
		"utils",
		ext_lib("expat"),
		"softfloat3",
		"wdlfft",
		"ymfm",
		ext_lib("jpeg"),
		"7z",
	}
if CPU_INCLUDE_DRC_NATIVE then
	links {
		"asmjit",
	}
end
if (STANDALONE~=true) then
	links {
		ext_lib("lua"),
		"lualibs",
		"linenoise",
	}
end
	links {
		ext_lib("zlib"),
		ext_lib("zstd"),
		ext_lib("flac"),
		ext_lib("utf8proc"),
	}
if (STANDALONE~=true) then
	links {
		ext_lib("sqlite3"),
	}
end

	if _OPTIONS["NO_USE_PORTAUDIO"]~="1" then
		links {
			ext_lib("portaudio"),
		}
		if _OPTIONS["targetos"]=="windows" then
			links {
				"setupapi",
			}
		end
	end
	if _OPTIONS["NO_USE_MIDI"]~="1" then
		links {
			ext_lib("portmidi"),
		}
	end
	links {
		"bgfx",
		"bimg",
		"bx",
		"ocore_" .. _OPTIONS["osd"],
	}

	override_resources = false;
	rctarget = _subtarget;

	maintargetosdoptions(_target,_subtarget)

	includedirs {
		MAME_DIR .. "src/osd",
		MAME_DIR .. "src/emu",
		MAME_DIR .. "src/devices",
		MAME_DIR .. "src/" .. _target,
		MAME_DIR .. "src/lib",
		MAME_DIR .. "src/lib/util",
		MAME_DIR .. "3rdparty",
		GEN_DIR  .. _target .. "/layout",
		GEN_DIR  .. "resource",
		ext_includedir("zlib"),
		ext_includedir("flac"),
	}


if (STANDALONE==true) then
	standalone();
end

if (STANDALONE~=true) then
	if _OPTIONS["targetos"]=="macosx" and (not override_resources) then
		linkoptions {
			"-sectcreate __TEXT __info_plist " .. _MAKE.esc(GEN_DIR) .. "resource/" .. _subtarget .. "-Info.plist"
		}
		custombuildtask {
			{ GEN_DIR .. "version.cpp" ,  GEN_DIR .. "resource/" .. _subtarget .. "-Info.plist",    {  MAME_DIR .. "scripts/build/verinfo.py" }, {"@echo Emitting " .. _subtarget .. "-Info.plist" .. "...",    PYTHON .. " $(1)  -p -b " .. _subtarget .. " $(<) > $(@)" }},
		}
		dependency {
			{ "$(TARGET)" ,  GEN_DIR  .. "resource/" .. _subtarget .. "-Info.plist", true  },
		}

	end

	if _OPTIONS["targetos"]=="windows" and (not override_resources) then
		rcfile = MAME_DIR .. "scripts/resources/windows/" .. _subtarget .. "/" .. rctarget ..".rc"
		if os.isfile(rcfile) then
			files {
				rcfile,
			}
			dependency {
				{ "$(OBJDIR)/".._subtarget ..".res" ,  GEN_DIR  .. "resource/" .. rctarget .. "vers.rc", true  },
			}
		else
			rctarget = "mame"
			files {
				MAME_DIR .. "scripts/resources/windows/mame/mame.rc",
			}
			dependency {
				{ "$(OBJDIR)/mame.res" ,  GEN_DIR  .. "resource/" .. rctarget .. "vers.rc", true  },
			}
		end
	end

	local mainfile = MAME_DIR .. "src/".._target .."/" .. _subtarget ..".cpp"
	if not os.isfile(mainfile) then
		mainfile = MAME_DIR .. "src/".._target .."/" .. _target ..".cpp"
	end
	files {
		mainfile,
--		MAME_DIR .. "src/version.cpp",
		GEN_DIR .. "version.cpp",
		GEN_DIR  .. _target .. "/" .. _subtarget .."/drivlist.cpp",
	}

	if (_OPTIONS["SOURCES"] == nil) then

		if os.isfile(MAME_DIR .. "src/".._target .."/" .. _subtarget ..".flt") then
			dependency {
			{
				GEN_DIR  .. _target .. "/" .. _subtarget .."/drivlist.cpp",  MAME_DIR .. "src/".._target .."/" .. _target ..".lst", true },
			}
			custombuildtask {
				{ MAME_DIR .. "src/".._target .."/" .. _subtarget ..".flt" ,  GEN_DIR  .. _target .. "/" .. _subtarget .."/drivlist.cpp",    {  MAME_DIR .. "scripts/build/makedep.py", MAME_DIR .. "src/".._target .."/" .. _target ..".lst"  }, {"@echo Building driver list...",    PYTHON .. " $(1) -r " .. MAME_DIR .. " driverlist $(2) -f $(<) > $(@)" }},
			}
		else
			if os.isfile(MAME_DIR .. "src/".._target .."/" .. _subtarget ..".lst") then
				custombuildtask {
					{ MAME_DIR .. "src/".._target .."/" .. _subtarget ..".lst" ,  GEN_DIR  .. _target .. "/" .. _subtarget .."/drivlist.cpp",    {  MAME_DIR .. "scripts/build/makedep.py" }, {"@echo Building driver list...",    PYTHON .. " $(1) -r " .. MAME_DIR .. " driverlist $(<) > $(@)" }},
				}
			else
				dependency {
				{
					GEN_DIR  .. _target .. "/" .. _target .."/drivlist.cpp",  MAME_DIR .. "src/".._target .."/" .. _target ..".lst", true },
				}
				custombuildtask {
					{ MAME_DIR .. "src/".._target .."/" .. _target ..".lst" ,  GEN_DIR  .. _target .. "/" .. _target .."/drivlist.cpp",    {  MAME_DIR .. "scripts/build/makedep.py" }, {"@echo Building driver list...",    PYTHON .. " $(1) -r " .. MAME_DIR .. " driverlist $(<) > $(@)" }},
				}
			end
		end
	end

	if (_OPTIONS["SOURCES"] ~= nil) then
			dependency {
			{
				GEN_DIR  .. _target .. "/" .. _subtarget .."/drivlist.cpp",  MAME_DIR .. "src/".._target .."/" .. _target ..".lst", true },
			}
			custombuildtask {
				{ GEN_DIR .. _target .."/" .. _subtarget ..".flt" ,  GEN_DIR  .. _target .. "/" .. _subtarget .."/drivlist.cpp",    {  MAME_DIR .. "scripts/build/makedep.py", MAME_DIR .. "src/".._target .."/" .. _target ..".lst"  }, {"@echo Building driver list...",    PYTHON .. " $(1) -r " .. MAME_DIR .. " driverlist $(2) -f $(<) > $(@)" }},
			}
	end

	configuration { "mingw*" }
		custombuildtask {
			{
--				MAME_DIR .. "src/version.cpp" ,
				GEN_DIR .. "version.cpp" ,
				GEN_DIR  .. "resource/" .. rctarget .. "vers.rc",
				{ MAME_DIR .. "scripts/build/verinfo.py" },
				{
					"@echo Emitting " .. rctarget .. "vers.rc" .. "...",
					PYTHON .. " $(1)  -r -b " .. rctarget .. " $(<) > $(@)"
				}
			},
		}

	configuration { "vs20*" }
		prebuildcommands {
			"mkdir \"" .. path.translate(GEN_DIR  .. "resource/","\\") .. "\" 2>NUL",
			"@echo Emitting ".. rctarget .. "vers.rc...",
			PYTHON .. " \"" .. path.translate(MAME_DIR .. "scripts/build/verinfo.py","\\") .. "\" -r -b " .. rctarget .. " \"" .. path.translate(GEN_DIR .. "version.cpp","\\") .. "\" > \"" .. path.translate(GEN_DIR  .. "resource/" .. rctarget .. "vers.rc", "\\") .. "\"" ,
		}
end

	configuration { }

	if _OPTIONS["DEBUG_DIR"]~=nil then
		debugdir(_OPTIONS["DEBUG_DIR"])
	else
		debugdir (MAME_DIR)
	end
	if _OPTIONS["DEBUG_ARGS"]~=nil then
		debugargs (_OPTIONS["DEBUG_ARGS"])
	else
		debugargs ("-window")
	end

end
