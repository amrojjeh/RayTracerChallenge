local target_output = "build/%{cfg.platform}/%{cfg.buildcfg}/bin/%{prj.name}/"
local object_output = "build/%{cfg.platform}/%{cfg.buildcfg}/bin-int/%{prj.name}/"

workspace "RayTracerChallenge"
	configurations {"Debug", "Release"}

project "RayTracer"
	location "RayTracer"
	kind "ConsoleApp"
	language "C++"
	files 
	{
		"%{prj.name}/**.h",
		"%{prj.name}/**.cpp"
	}

	targetdir (target_output)
	objdir (object_output)

	filter {"configurations:Debug"}
		symbols "On"
	
	filter {"configurations:Release"}
		optimize "On"

project "RayTester"
	location "RayTester"
	kind "ConsoleApp"
	language "C++"
	
	files 
	{
		"%{prj.name}/**.h",
		"%{prj.name}/**.cpp"
	}

	includedirs
	{
		"RayTracer/src/",
		"gtest/include"
	}

	links
	{
		"gtest"
	}

	targetdir (target_output)
	objdir (object_output)


project "gtest"
	location "gtest"
	kind "StaticLib"
	language "C++"

	files
	{
		"%{prj.name}/**.h",
		"%{prj.name}/**.cc"
	}

	includedirs
	{
		"%{prj.name}/",
		"%{prj.name}/include"
	}

	targetdir (target_output)
	objdir (object_output)

