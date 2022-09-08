# Build and pack common lib
cd DFCommonLib
dotnet restore DFCommonLib.csproj
dotnet build DFCommonLib.csproj
dotnet pack DFCommonLib.csproj -o bin/debug
dotnet nuget push bin/debug/DarkFactor.Common.Lib.*.nupkg --api-key 1337 --source DarkFactor --skip-duplicate
cd ..

# Build and pack AccountClient lib
cd AccountServer/AccountClient
dotnet restore AccountClient.csproj
dotnet build AccountClient.csproj
dotnet pack AccountClient.csproj -o bin/debug
dotnet nuget push bin/debug/DarkFactor.AccountClient.Lib.*.nupkg --api-key 1337 --source DarkFactor --skip-duplicate
cd ../..

# Bug Report Server
cd BugReportServer
docker-compose build
cd ..

# Account Server
cd AccountServer/AccountServer
docker-compose build
cd ../..

# Mail Server
cd MailServer
docker-compose build
cd ..

# Bot Server
cd BotWebServer
scd ..
