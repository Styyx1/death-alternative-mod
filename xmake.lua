-- set minimum xmake version
set_xmakever("2.8.2")

-- includes
includes("lib/commonlibsse-ng")

-- set project
set_project("shades-of-mortality")
set_version("1.5.0")
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

-- configs
set_config("rex_ini", true)

-- packages
add_requires("simpleini")
add_requires("spdlog", { configs = { header_only = false } })

-- extra files
add_extrafiles("release/**.ini")

-- targets
target("shades-of-mortality")
    -- add dependencies to target
    add_deps("commonlibsse-ng")
    add_packages("fmt", "spdlog", "simpleini")
    add_options("rex_ini")

    -- add commonlibsse-ng plugin
    add_rules("commonlibsse-ng.plugin", {
        name = "shades-of-mortality",
        author = "styyx",
        description = "skse plugin that allows you to cheat death"
    })

    -- add src files
    add_files("src/**.cpp")
    add_headerfiles("src/**.h")
    add_includedirs("src")
    set_pcxxheader("src/pch.h")

after_build(function(target)
    local copy = function(env, ext)
        for _, env in pairs(env:split(";")) do
            if os.exists(env) then
                local plugins = path.join(env, ext, "SKSE/Plugins")
                os.mkdir(plugins)
                os.trycp(target:targetfile(), plugins)
                os.trycp(target:symbolfile(), plugins)
                os.trycp("$(projectdir)/release/**.ini", plugins)
            end
        end
    end
    if os.getenv("XSE_TES5_MODS_PATH") then
        copy(os.getenv("XSE_TES5_MODS_PATH"), target:name())
    elseif os.getenv("XSE_TES5_GAME_PATH") then
        copy(os.getenv("XSE_TES5_GAME_PATH"), "Data")
    end
end)