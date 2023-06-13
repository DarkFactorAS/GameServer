--
-- Create user and privileges
--
drop user 'botuser'@'%';
flush privileges;
CREATE USER 'botuser'@'%' IDENTIFIED BY 'pewpewpew';
GRANT ALL PRIVILEGES ON * . * TO 'botuser'@'%' IDENTIFIED BY 'pewpewpew';
GRANT CREATE ON *.* TO 'botuser'@'%' IDENTIFIED BY 'pewpewpew';
commit;
flush privileges;
