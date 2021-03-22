@echo off

REM Bug Report Server
cd BugReportServer
docker-compose down
cd ..

REM Account Server
cd AccountServer/AccountServer
docker-compose down
cd ../..

REM Bot Server
cd BotWebServer
docker-compose down
cd ..
