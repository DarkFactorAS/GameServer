#
# Source for table "games"
#
delimiter $$
drop procedure if exists tmp $$
create procedure tmp()
begin
	set @patchid = 4;
	if not f_patch_executed( @patchid ) then
	
		DROP TABLE IF EXISTS `game`;
		CREATE TABLE `game` (
      `GameId` int(11) NOT NULL DEFAULT '0',
		  `PlayfieldId` int(11) NOT NULL DEFAULT '0',
		  `GameStatus` int(11) NOT NULL DEFAULT '0',
		  `WinningPlayerId` int(11) NOT NULL DEFAULT '0',
      PRIMARY KEY (`GameId`)
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