# Flush Nuget repo
dotnet nuget locals all -c

# Build and pack common lib
dotnet restore DFCommonLib/DFCommonLib.csproj
dotnet build DFCommonLib/DFCommonLib.csproj
dotnet pack DFCommonLib/DFCommonLib.csproj -o ~/NuGet



