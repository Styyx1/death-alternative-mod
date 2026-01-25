-- set minimum xmake version
set_xmakever("2.8.2")

-- includes
includes("lib/commonlibsse")
includes("extern/styyx-utils")

-- set project
set_project("shades-of-mortality")
set_version("3.0.0")
set_license("GPL-3.0")

-- set defaults
set_languages("c++23")
set_warnings("allextra")

-- set policies
set_policy("package.requires_lock", true)

-- add rules
add_rules("mode.debug", "mode.releasedbg")
add_rules("plugin.vsxmake.autoupdate")
set_defaultmode("releasedbg")

if is_mode("debug") then
    add_defines("_BUILD_DEFAULT_CONFIG")
end

-- configs
set_config("commonlib_toml", true)

-- extra files
add_extrafiles("release/**.toml")

-- targets
target("shades-of-mortality")
    -- add dependencies to target
    add_deps("commonlibsse")
    add_deps("styyx-util")

    -- add commonlibsse plugin
    add_rules("commonlibsse.plugin", {
        name = "shades-of-mortality",
        author = "styyx",
        description = "skse plugin that allows you to cheat death"
    })

    -- add src files
    add_files("src/**.cpp")
    add_headerfiles("src/**.h")
    add_includedirs("src")
    set_pcxxheader("src/pch.h")