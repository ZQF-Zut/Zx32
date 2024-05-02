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

add_rules("mode.debug", "mode.release")

-- msvc runtime settings
if is_mode("release") then
    if is_plat("windows") then
        set_runtimes("MT")
    end
    set_policy("build.optimization.lto", true)
elseif is_mode("debug") then
    if is_plat("windows") then
        set_runtimes("MDd")
    end
end

-- defines
add_defines("WIN32_LEAN_AND_MEAN")

-- rules
rule("module")
    on_load(function (target)
        if is_mode("debug") then
            target:set("kind", "shared")
            if target:is_plat("windows") then
                import("core.project.rule")
                local rule = rule.rule("utils.symbols.export_all")
                target:rule_add(rule)
                target:extraconf_set("rules", "utils.symbols.export_all", {export_classes = true})
            end
        elseif is_mode("release") then
            target:set("kind", "static")
        end
    end)

-- targets
target("wx32")
    set_kind("$(kind)")
    add_syslinks("user32")
    add_files("src/*.cpp")
    add_files("src/Private/*.cpp")
    add_includedirs("include", {public = true})
    add_headerfiles("include/(**.h)")
    add_rules("module")
    
target("wx32-test")
    set_default(false)
    set_kind("binary")
    add_files("test/main.cpp")
    add_deps("wx32")