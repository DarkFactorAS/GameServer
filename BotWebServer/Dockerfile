# Use .Net Core 3.1 image
FROM mcr.microsoft.com/dotnet/sdk:8.0 AS build-env
WORKDIR /app

# The following arguments must be passed as environment variables
# Get the NuGet arguments and set as environment to use in NuGet
ARG username
ARG token
ENV NUGET_USERNAME=$username
ENV NUGET_TOKEN=$token

# Copy files
COPY ./ ./

# Restore and build web
RUN dotnet restore BotWebServer.csproj
RUN dotnet publish BotWebServer.csproj -c Release -o out

# Build runtime image
FROM mcr.microsoft.com/dotnet/aspnet:8.0
WORKDIR /app
COPY --from=build-env /app/out .
ENTRYPOINT ["dotnet", "BotWebServer.dll"]

EXPOSE 5001:80
