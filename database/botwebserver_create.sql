
CREATE DATABASE bots;
CREATE USER 'botuser'@'%' IDENTIFIED BY 'pewpewpew';
GRANT ALL PRIVILEGES ON * . * TO 'botuser'@'%';
commit;
