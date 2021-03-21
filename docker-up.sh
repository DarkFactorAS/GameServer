# Bug Report Server
cd BugReportServer
docker-compose up &
cd ..

# Account Server
cd AccountServer/AccountServer
docker-compose up &
cd ../..

# Bot Server
cd BotWebServer
docker-compose up &
cd ..
