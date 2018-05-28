# Host: localhost  (Version: 5.6.25-log)
# Date: 2015-11-05 16:04:11
# Generator: MySQL-Front 5.3  (Build 4.4)

/*!40101 SET NAMES utf8 */;

#
# Source for table "patches"
#

DROP TABLE IF EXISTS `patches`;
CREATE TABLE `patches` (
  `PatchId` int(11) NOT NULL,
  `Updated` timestamp NULL DEFAULT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;


#
# Source for table "account"
#

DROP TABLE IF EXISTS `account`;
CREATE TABLE `account` (
  `AccountId` int(11) NOT NULL DEFAULT '0',
  `Token` varchar(64) NOT NULL DEFAULT '' COMMENT 'Device token usually',
  `Name` varchar(30) NOT NULL DEFAULT '',
  `Username` varchar(64) DEFAULT NULL,
  `Password` varchar(64) DEFAULT NULL,
  `IconId` int(10) DEFAULT '0',
  `Platform` int(11) NOT NULL DEFAULT '0' COMMENT 'Enum with platform',
  `LoginMethod` int(11) DEFAULT '0',
  `PurchaseFlags` int(11) DEFAULT '0',
  `LastLogin` datetime DEFAULT null,
  UNIQUE KEY `id` (`AccountId`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

#
# Source for table "purchases"
#

DROP TABLE IF EXISTS `purchases`;
CREATE TABLE `purchases` (
  `Id` int(11) NOT NULL AUTO_INCREMENT,
  `AccountId` int(11) NOT NULL DEFAULT '0',
  `ItemCode` int(11) NOT NULL DEFAULT '0',
  `PurchaseDate` date DEFAULT NULL,
  `ReceiptId` varchar(255) DEFAULT NULL,
  `Receipt` blob,
  PRIMARY KEY (`Id`)
) ENGINE=MyISAM AUTO_INCREMENT=21 DEFAULT CHARSET=latin1;
