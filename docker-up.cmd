@echo off

REM Bug Report Server
cd BugReportServer
docker-compose build
docker-compose up -d
cd ..

REM Account Server
cd AccountServer/AccountServer
docker-compose build
docker-compose up -d
cd ../..

REM Bot Server
cd BotWebServer
docker-compose build
docker-compose up -d
cd ..
