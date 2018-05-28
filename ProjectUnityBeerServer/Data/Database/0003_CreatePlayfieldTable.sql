#
# Source for table "playfield"
#
delimiter $$
drop procedure if exists tmp $$
create procedure tmp()
begin
	set @patchid = 3;
	if not f_patch_executed( @patchid ) then
	
		DROP TABLE IF EXISTS `playfield`;
		CREATE TABLE `playfield` (
      `Id` int(11) NOT NULL AUTO_INCREMENT,
		  `OwnerAccountId` int(11) NOT NULL DEFAULT '0',
      `Name` varchar(32) DEFAULT NULL,
      `Description` varchar(1024) DEFAULT NULL,
      `LastUpdated` datetime DEFAULT NULL,
		  `TimesPlayed` int(11) NOT NULL DEFAULT '0',
		  `Flags` int(11) NOT NULL DEFAULT '0',
		  `NumPlayers` int(11) NOT NULL DEFAULT '0',
		  `NumGoals` int(11) NOT NULL DEFAULT '0',
		  `BoardSizeX` int(11) NOT NULL DEFAULT '0',
		  `BoardSizeY` int(11) NOT NULL DEFAULT '0',
      `Data` blob,
      PRIMARY KEY (`Id`)
		) ENGINE=MyISAM DEFAULT CHARSET=latin1;

		call p_patch_executed_successfully( @patchid );		
	else
		set @prnt = concat( 'select ''Patch ',@patchid,' already executed!'' as ''''');
		prepare pstat from @prnt;
		execute pstat;
	end if;
end $$
delimiter ;

call tmp();