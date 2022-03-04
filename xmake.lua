if is_os("windows") then
    set_config("arch", "x86")
    if(is_mode("release")) then
        set_config("vs_runtime", "MD")
        add_cxflags("-MD", {force = true})
        set_installdir("build/x86/debug")
    else
        set_config("vs_runtime", "MDd")
        add_cxflags("-MDd", {force = true})
        set_installdir("build/x86/release")
    end
else
    add_cxflags("-MD", {force = true})
end
add_requires("vcpkg::boost-asio", "vcpkg::boost-system", "vcpkg::boost-filesystem", {configs={runtime_link="shared", shared=true, debug=true}})

target("cpp-pm")
    set_kind("binary")
    set_symbols("debug")
    add_packages("vcpkg::boost-asio", "vcpkg::boost-system", "vcpkg::boost-filesystem")
    add_headerfiles("sources/*.h", "sources/*.hpp")
    add_files("sources/*.cpp")
    after_build(function(target)
        import("target.action.install")(target)
    end)

    