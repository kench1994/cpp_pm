set_config("arch", "x86")
set_config("mode", "debug")
if is_os("windows") then
    if(is_mode("release")) then
        set_config("vs_runtime", "MD")
        add_cxflags("-MD", {force = true})
    else
        set_config("vs_runtime", "MDd")
        add_cxflags("-MDd", {force = true})
    end
else
    add_cxflags("-MD", {force = true})
end
--0add_requires("vcpkg::boost", {configs={runtime_link="shared", shared=true, debug=true}, alias = "boost"})

target("cpp-pm")
    set_kind("binary")
    set_symbols("debug")
    add_packages("vcpkg::boost")
    add_includedirs("sources")
    add_files("sources/*.cpp")