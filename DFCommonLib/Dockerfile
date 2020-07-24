# Use .Net Core 3.1 image
FROM mcr.microsoft.com/dotnet/core/sdk:3.1 AS build-env
WORKDIR /app

# Copy csproj and restore as distinct layers
COPY ./DFCommonLib/*.csproj ./
RUN dotnet restore DFCommonLib.csproj

# Copy everything else
COPY ./DFCommonLib/ ./

# Pack nuget 
RUN dotnet pack -o /nuget
