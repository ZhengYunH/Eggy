@echo off
set suffix=.spirv.cache
set folder=%cd%\Engine\Shader

for /R %folder% %%f in (*%suffix%,) do ( 
	del %%f
)