# Eggy

toy Game-Engine

## File Structure

+ Engine: store source code , some dependent SDK , config , shaders
+ Package: all Resource except shaders

## Setup

### Prerequisite

+ cmake 3.22.0
+ C++20
+ Visual Studio 2022
+ (optional)C++ ATL v143 generator(x86 & x64) (Installed in Visual Studio Installer, to compile ShaderCondutor )
+ (optional) Python3

### Generate Project

run `Build.bat` and  you will get ***EggyEngine.sln***

## Technical Solutions

### Cross-Platform Render 

+ Shader Language(based on HLSL): compile to SPIR-V
  + [ShaderConductor](https://github.com/microsoft/ShaderConductor)
  + reflection information are extracted by  ***SPIRV-Reflect***
+ Abstract Interface: IRenderDevice \ IDeviceResource
  + Specific Graphics API should inherit IRenderDevice, such as D3D11Device, and implement virtual function provided in these Interface.

### Test(TODO)

+ [google/googletest: GoogleTest - Google Testing and Mocking Framework (github.com)](https://github.com/google/googletest)

