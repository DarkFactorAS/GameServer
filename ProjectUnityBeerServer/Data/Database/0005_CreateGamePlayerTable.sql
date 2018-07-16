#
# Source for table "games"
#
delimiter $$
drop procedure if exists tmp $$
create procedure tmp()
begin
	set @patchid = 5;
	if not f_patch_executed( @patchid ) then
	
		DROP TABLE IF EXISTS `game_player`;
		CREATE TABLE `game_player` (
      `GameId` int(11) NOT NULL DEFAULT '0',
		  `PlayerId` int(11) NOT NULL DEFAULT '0',
		  `PlayerPosX` int(11) NOT NULL DEFAULT '0',
		  `PlayerPosY` int(11) NOT NULL DEFAULT '0',
		  `SpawnPointId` int(11) NOT NULL DEFAULT '0',
      PRIMARY KEY (`GameId`,`PlayerId`)
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