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

### Generate Project

run `Build.bat` and  you will get ***EggyEngine.sln***

## Technical Solutions

### Cross-Platform Render 

+ Shader Language(based on HLSL): compile to SPIR-V
  + converted by [***DirectXShaderCompiler***](https://github.com/microsoft/DirectXShaderCompiler), see [SPIR‐V CodeGen](https://github.com/Microsoft/DirectXShaderCompiler/wiki/SPIR%E2%80%90V-CodeGen) for more information
  + reflection information are extracted by  ***SPIRV-Reflect***
+ Abstract Interface: IRenderDevice \ IDeviceResource
  + Specific Graphics API should inherit IRenderDevice, such as D3D11Device, and implement virtual function provided in these Interface.

### Test(TODO)

+ [google/googletest: GoogleTest - Google Testing and Mocking Framework (github.com)](https://github.com/google/googletest)

