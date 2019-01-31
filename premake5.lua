workspace "NickVulkanEngine"
	architecture "x64"
	startproject "NickVulkanEngine"
	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
IncludeDir = {}
IncludeDir["GLFW"] = "ExternalResources/GLFW/include"
IncludeDir["GLM"] = "ExternalResources/glm"
IncludeDir["STB"] = "ExternalResources/stb"
IncludeDir["VULKAN"] = "ExternalResources/Vulkan/Include"
IncludeDir["TOL"] = "ExternalResources/tinyobjloader"

project "NickVulkanEngine"
	location "NickVulkanEngine"
	kind "ConsoleApp"
	language "C++"
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	staticruntime "off"
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}
	includedirs
	{
		"%{prj.name}/src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLM}",
		"%{IncludeDir.STB}",
		"%{IncludeDir.VULKAN}",
		"%{IncludeDir.TOL}"
	}
	libdirs  
	{ 
		"ExternalResources/Vulkan/Lib",
		"ExternalResources/GLFW/lib-vc2015"
	}
	links
	{
		"vulkan-1.lib",
		"glfw3.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"
		defines
		{
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "NVE_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "NVE_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "NVE_DIST"
		runtime "Release"
		optimize "On"