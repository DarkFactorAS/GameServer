# Use .Net Core 3.1 image
FROM mcr.microsoft.com/dotnet/core/sdk:3.1 AS build-env
WORKDIR /app

# Add nuget repository
COPY ./NuGet/DarkFactor.Common.Lib.*.nupkg ./NuGet/
RUN dotnet nuget add source /app/NuGet --name DarkFactor

# Copy csproj and restore as distinct layers
COPY ./BotWebServer/*.csproj ./
RUN dotnet restore

# Copy everything else
COPY ./BotWebServer/ ./

# Build solution
#RUN dotnet publish PPJobService.csproj -r win-x64 -c Release -o out
RUN dotnet publish BotWebServer.csproj -c Release -o out

# Build runtime image
FROM mcr.microsoft.com/dotnet/core/aspnet:3.1
WORKDIR /app
COPY --from=build-env /app/out .
ENTRYPOINT ["dotnet", "BotWebServer.dll"]

EXPOSE 5000:80
