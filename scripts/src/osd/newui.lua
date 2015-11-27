dofile("modules.lua")

premake.make.linkoptions_after = false;

function maintargetosdoptions(_target,_subtarget)
	osdmodulestargetconf()

	configuration { "mingw*-gcc" }
		linkoptions {
			"-municode",
		}

	configuration { }

	if _OPTIONS["DIRECTINPUT"] == "8" then
		links {
			"dinput8",
		}
	else
		links {
			"dinput",
		}
	end

	links {
		"comctl32",
		"comdlg32",
		"psapi",
	}

-- needs same resources as messui, because dropdown menus are in mameui.rc
	override_resources = true;
	local rcfile = MAME_DIR .. "src/osd/winui/" .. _subtarget .. ".rc"
	local uifile = MAME_DIR .. "src/osd/winui/" .. _subtarget .. "ui.rc"

	if not os.isfile(rcfile) then
		print(string.format("***** %s not found *****",rcfile))
		os.exit();
	end

	if not os.isfile(uifile) then
		print(string.format("***** %s not found *****",uifile))
		os.exit();
	end

	files {
		rcfile,
	}
	dependency {
		{ "$(OBJDIR)/".._subtarget..".res" ,  GEN_DIR  .. "/resource/".._subtarget.."vers.rc", true  },
	}
end


newoption {
	trigger = "DIRECTINPUT",
	description = "Minimum DirectInput version to support",
	allowed = {
		{ "7",  "Support DirectInput 7 or later"  },
		{ "8",  "Support DirectInput 8 or later" },
	},
}

if not _OPTIONS["DIRECTINPUT"] then
	_OPTIONS["DIRECTINPUT"] = "8"
end


project ("osd_" .. _OPTIONS["osd"])
	uuid (os.uuid("osd_" .. _OPTIONS["osd"]))
	kind "StaticLib"

	dofile("winui_cfg.lua")
	osdmodulesbuild()

	defines {
		"DIRECT3D_VERSION=0x0900",
	}

	if _OPTIONS["DIRECTINPUT"] == "8" then
		defines {
			"DIRECTINPUT_VERSION=0x0800",
		}
	else
		defines {
			"DIRECTINPUT_VERSION=0x0700",
		}
	end

	includedirs {
		MAME_DIR .. "src/emu",
		MAME_DIR .. "src/devices",
		MAME_DIR .. "src/osd",
		MAME_DIR .. "src/lib",
		MAME_DIR .. "src/lib/util",
		MAME_DIR .. "src/osd/modules/render",
		MAME_DIR .. "3rdparty",
	}

	includedirs {
		MAME_DIR .. "src/osd/windows",
	}

	files {
		MAME_DIR .. "src/osd/modules/render/drawd3d.cpp",
		MAME_DIR .. "src/osd/modules/render/d3d/d3d9intf.cpp",
		MAME_DIR .. "src/osd/modules/render/d3d/d3dhlsl.cpp",
		MAME_DIR .. "src/osd/modules/render/drawdd.cpp",
		MAME_DIR .. "src/osd/modules/render/drawgdi.cpp",
		MAME_DIR .. "src/osd/modules/render/drawnone.cpp",
		MAME_DIR .. "src/osd/windows/input.cpp",
		MAME_DIR .. "src/osd/windows/output.cpp",
		MAME_DIR .. "src/osd/windows/video.cpp",
		MAME_DIR .. "src/osd/windows/window.cpp",
		MAME_DIR .. "src/osd/winui/newui.cpp",
		MAME_DIR .. "src/osd/windows/winmain.cpp",
		MAME_DIR .. "src/osd/modules/debugger/win/consolewininfo.cpp",
		MAME_DIR .. "src/osd/modules/debugger/win/debugbaseinfo.cpp",
		MAME_DIR .. "src/osd/modules/debugger/win/debugviewinfo.cpp",
		MAME_DIR .. "src/osd/modules/debugger/win/debugwininfo.cpp",
		MAME_DIR .. "src/osd/modules/debugger/win/disasmbasewininfo.cpp",
		MAME_DIR .. "src/osd/modules/debugger/win/disasmviewinfo.cpp",
		MAME_DIR .. "src/osd/modules/debugger/win/disasmwininfo.cpp",
		MAME_DIR .. "src/osd/modules/debugger/win/editwininfo.cpp",
		MAME_DIR .. "src/osd/modules/debugger/win/logwininfo.cpp",
		MAME_DIR .. "src/osd/modules/debugger/win/memoryviewinfo.cpp",
		MAME_DIR .. "src/osd/modules/debugger/win/memorywininfo.cpp",
		MAME_DIR .. "src/osd/modules/debugger/win/pointswininfo.cpp",
		MAME_DIR .. "src/osd/modules/debugger/win/uimetrics.cpp",
	}


project ("ocore_" .. _OPTIONS["osd"])
	uuid (os.uuid("ocore_" .. _OPTIONS["osd"]))
	kind "StaticLib"

	options {
		"ForceCPP",
	}
	removeflags {
		"SingleOutputDir",
	}

	dofile("windows_cfg.lua")

	includedirs {
		MAME_DIR .. "src/emu",
		MAME_DIR .. "src/osd",
		MAME_DIR .. "src/lib",
		MAME_DIR .. "src/lib/util",
	}

	BASE_TARGETOS = "win32"
	SDLOS_TARGETOS = "win32"
	SYNC_IMPLEMENTATION = "windows"

	includedirs {
		MAME_DIR .. "src/osd/windows",
		MAME_DIR .. "src/lib/winpcap",
	}

	files {
		MAME_DIR .. "src/osd/osdcore.cpp",
		MAME_DIR .. "src/osd/strconv.cpp",
		MAME_DIR .. "src/osd/windows/main.cpp",
		MAME_DIR .. "src/osd/windows/windir.cpp",
		MAME_DIR .. "src/osd/windows/winfile.cpp",
		MAME_DIR .. "src/osd/modules/sync/sync_windows.cpp",
		MAME_DIR .. "src/osd/windows/winutf8.cpp",
		MAME_DIR .. "src/osd/windows/winutil.cpp",
		MAME_DIR .. "src/osd/windows/winclip.cpp",
		MAME_DIR .. "src/osd/windows/winsocket.cpp",
		MAME_DIR .. "src/osd/windows/winptty.cpp",
		MAME_DIR .. "src/osd/modules/osdmodule.cpp",
		MAME_DIR .. "src/osd/modules/lib/osdlib_win32.cpp",
	}

	if _OPTIONS["NOASM"]=="1" then
		files {
			MAME_DIR .. "src/osd/modules/sync/work_mini.cpp",
		}
	else
		files {
			MAME_DIR .. "src/osd/modules/sync/work_osd.cpp",
		}
	end


--------------------------------------------------
-- ledutil
--------------------------------------------------

if _OPTIONS["with-tools"] then
	project("ledutil")
		uuid ("061293ca-7290-44ac-b2b5-5913ae8dc9c0")
		kind "ConsoleApp"

		options {
			"ForceCPP",
		}

		targetdir(MAME_DIR)

		links {
			"ocore_" .. _OPTIONS["osd"],
		}

		includedirs {
			MAME_DIR .. "src/osd",
		}
		
		files {
			MAME_DIR .. "src/osd/windows/ledutil.cpp",
		}
end
