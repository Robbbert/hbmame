defines {
	"UNICODE",
	"_UNICODE",
	"OSD_WINDOWS",
	"USE_SDL=0",
	--"WIN95_MULTIMON"
	"_WIN32_WINNT=0x0501",
	"USE_QTDEBUG=0",
	"WIN32_LEAN_AND_MEAN",
	"NOMINMAX",
}

configuration { "vs*" }
	flags {
		"Unicode",
	}

configuration { }

if _OPTIONS["USE_TAPTUN"]=="1" or _OPTIONS["USE_PCAP"]==1 then
	defines {
		"USE_NETWORK",
	}
	if _OPTIONS["USE_TAPTUN"]=="1" then
		defines {
			"OSD_NET_USE_TAPTUN",
		}
	end
	if _OPTIONS["USE_PCAP"]=="1" then
		defines {
			"OSD_NET_USE_PCAP",
		}
	end
end

