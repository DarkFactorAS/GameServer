# Host: 192.168.1.141  (Version: 5.1.71)
# Date: 2014-06-24 00:25:15
# Generator: MySQL-Front 5.3  (Build 4.4)

/*!40101 SET NAMES utf8 */;

#
# Source for table "Account"
#

DROP TABLE IF EXISTS `Account`;
CREATE TABLE `Account` (
  `PlayerId` int(11) NOT NULL DEFAULT '0',
  `Token` varchar(64) NOT NULL DEFAULT '',
  `Name` varchar(20) NOT NULL DEFAULT '',
  `Username` varchar(20) DEFAULT NULL,
  `Password` varchar(20) DEFAULT NULL,
  `IconId` int(10) DEFAULT '0',
  `Score` int(11) DEFAULT '0',
  `Platform` int(11) NOT NULL DEFAULT '0' COMMENT 'Enum with platform',
  UNIQUE KEY `id` (`PlayerId`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

#
# Source for table "gamedeck"
#

DROP TABLE IF EXISTS `gamedeck`;
CREATE TABLE `gamedeck` (
  `TileId` int(11) NOT NULL AUTO_INCREMENT,
  `GameId` int(11) NOT NULL DEFAULT '0',
  `ResourceId` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`TileId`)
) ENGINE=MyISAM AUTO_INCREMENT=21858 DEFAULT CHARSET=latin1;

#
# Source for table "games"
#

DROP TABLE IF EXISTS `games`;
CREATE TABLE `games` (
  `GameId` int(11) NOT NULL AUTO_INCREMENT,
  `Player1Id` int(11) NOT NULL DEFAULT '0',
  `Player1Score` int(11) DEFAULT '0',
  `Player1PosX` int(11) DEFAULT '0',
  `Player1PosY` int(11) DEFAULT '0',
  `Player2Id` int(11) NOT NULL DEFAULT '0',
  `Player2Score` int(11) DEFAULT '0',
  `Player2PosX` int(11) DEFAULT '0',
  `Player2PosY` int(11) DEFAULT '0',
  `Turn` int(11) NOT NULL,
  `PlayfieldId` int(11) NOT NULL DEFAULT '1',
  `PlacedTiles` int(11) DEFAULT '0',
  `LastUpdated` timestamp NULL DEFAULT NULL,
  PRIMARY KEY (`GameId`)
) ENGINE=MyISAM AUTO_INCREMENT=21 DEFAULT CHARSET=latin1;

#
# Source for table "id"
#

DROP TABLE IF EXISTS `id`;
CREATE TABLE `id` (
  `Type` int(11) NOT NULL DEFAULT '0',
  `Id` int(11) NOT NULL DEFAULT '0'
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

#
# Source for table "playfield"
#

DROP TABLE IF EXISTS `playfield`;
CREATE TABLE `playfield` (
  `Id` int(11) NOT NULL AUTO_INCREMENT,
  `Name` varchar(255) DEFAULT NULL COMMENT 'Name of the playfield',
  `OwnerId` int(11) DEFAULT '0',
  `TileType` int(11) NOT NULL DEFAULT '0',
  `Tileset` int(11) NOT NULL DEFAULT '0' COMMENT 'What type of tiles to use ( Dungeon/Castle/etc )',
  `SizeX` int(11) NOT NULL DEFAULT '0' COMMENT 'Size of the board/playfield',
  `SizeY` int(11) NOT NULL DEFAULT '0' COMMENT 'Size of the board/playfield',
  `NumTiles` int(11) NOT NULL DEFAULT '0' COMMENT 'Number of tiles in the draw deck',
  `StartTilePosX` int(11) NOT NULL DEFAULT '0' COMMENT 'Position for the starting tile',
  `StartTilePosY` int(11) DEFAULT '0' COMMENT 'Position for the starting tile',
  `MissionType` int(11) DEFAULT '0',
  PRIMARY KEY (`Id`)
) ENGINE=MyISAM AUTO_INCREMENT=11 DEFAULT CHARSET=latin1;

#
# Source for table "PlayfieldTiles"
#

DROP TABLE IF EXISTS `PlayfieldTiles`;
CREATE TABLE `PlayfieldTiles` (
  `PlayfieldId` int(11) DEFAULT NULL,
  `TileId` int(11) DEFAULT NULL,
  `PositionX` int(11) DEFAULT NULL,
  `PositionY` int(11) DEFAULT NULL,
  `RotationZ` int(11) DEFAULT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
