cd DFCommonLib

# Clear
rm -Rf bin/debug

# Flush Nuget repo
dotnet nuget locals all -c

# Build and pack common lib
dotnet restore DFCommonLib.csproj
dotnet build DFCommonLib.csproj
dotnet pack DFCommonLib.csproj -o bin/debug

cd ..

# Copy nuget files
cp ./DFCommonLib/bin/debug/DarkFactor.Common.Lib.* ./NuGet
cp ./NuGet/* BugReportServer/NuGet/
cp ./NuGet/* BotWebServer/NuGet/
cp ./NuGet/* AccountServer/AccountServer/NuGet/

read -p "Press any key to resume ..."
