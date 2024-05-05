-- project
set_project("wx32")
set_xmakever("2.8.9")
set_description("a win32 api wrapper using utf-8 and modern c++")

-- language
set_warnings("all")
set_languages("c++20")
set_encodings("utf-8")

-- allows
set_allowedplats("windows")
set_allowedarchs("x86", "x64")
set_allowedmodes("debug", "release")

-- rules
add_rules("plugin.vsxmake.autoupdate")
add_rules("mode.debug", "mode.release")

-- defines
-- add_defines("WIN32_LEAN_AND_MEAN")

-- lto
if is_mode("releasedbg") or is_mode("release") then
    if is_plat("windows") then
        set_policy("build.optimization.lto", true)
    end
end

-- targets
target("wx32")
    set_kind("$(kind)")
    if is_plat("windows") then
        if is_kind("shared") then
            add_rules("utils.symbols.export_all", {export_classes = true})
        end
    end
    add_syslinks("user32")
    add_files("src/*.cpp")
    add_files("src/Private/*.cpp")
    add_includedirs("include", {public = true})
    add_headerfiles("include/(**.h)")

target("wx32-test")
    set_default(false)
    set_kind("binary")
    add_files("test/main.cpp")
    add_deps("wx32")
