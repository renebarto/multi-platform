-- MySQL dump 10.13  Distrib 5.1.46, for Win32 (ia32)
--
-- Host: localhost    Database: simple
-- ------------------------------------------------------
-- Server version	5.1.46-community

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Current Database: `simple`
--

CREATE DATABASE /*!32312 IF NOT EXISTS*/ `simple` /*!40100 DEFAULT CHARACTER SET latin1 */;

USE `simple`;

--
-- Temporary table structure for view `count by artist`
--

DROP TABLE IF EXISTS `count by artist`;
/*!50001 DROP VIEW IF EXISTS `count by artist`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE TABLE `count by artist` (
  `Artist` tinyint NOT NULL,
  `CountOfArtist` tinyint NOT NULL
) ENGINE=MyISAM */;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `table1`
--

DROP TABLE IF EXISTS `table1`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `table1` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `A` int(10) unsigned NOT NULL DEFAULT '0',
  `B` varchar(45) DEFAULT NULL,
  `C` tinyint(1) DEFAULT NULL,
  `D` double NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=24 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `table1`
--

LOCK TABLES `table1` WRITE;
/*!40000 ALTER TABLE `table1` DISABLE KEYS */;
INSERT INTO `table1` VALUES (22,0,'One',0,0),(23,0,'Two',-1,0);
/*!40000 ALTER TABLE `table1` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `top 40`
--

DROP TABLE IF EXISTS `top 40`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `top 40` (
  `ThisWeek` int(10) unsigned NOT NULL,
  `PrevWeek` int(10) unsigned NOT NULL,
  `Artist` varchar(255) NOT NULL,
  `Title` varchar(255) NOT NULL,
  `NumWeeks` int(10) unsigned NOT NULL,
  `HighestPos` int(10) unsigned NOT NULL,
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  PRIMARY KEY (`ID`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=83 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `top 40`
--

LOCK TABLES `top 40` WRITE;
/*!40000 ALTER TABLE `top 40` DISABLE KEYS */;
INSERT INTO `top 40` VALUES (1,0,'The Beatles','I feel fine',1,0,41),(2,0,'Lucille Starr','The French song (quand le soleil dit bonjour aux montagnes)',1,0,42),(3,0,'Roy Orbison','Pretty woman',1,0,43),(4,0,'Adamo','Dolce Paola',1,0,44),(5,0,'Imca Marina','Harlekino',1,0,45),(6,0,'Willeke Alberti','Mijn dagboek',1,0,46),(7,0,'Gert & Hermien','In der mondhellen nacht',1,0,47),(8,0,'Cliff Richard','I could easily fall',1,0,48),(9,0,'Julie Rogers','The wedding',1,0,49),(10,0,'The Rolling Stones','Time is on my side',1,0,50),(11,0,'Jim Reeves','There\'s a heartache following me',1,0,51),(12,0,'Edwin Rutten','Ik moet altijd weer opnieuw aan je denken',1,0,52),(13,0,'Roy Orbison','Pretty paper',1,0,53),(14,0,'Marc Aryan','Si j\'etais un fils d\'un roi',1,0,54),(15,0,'The Supremes','Baby love',1,0,55),(16,0,'The Rolling Stones','Tell me',1,0,56),(17,0,'The Kinks','All day and all of the night',1,0,57),(18,0,'Sandie Shaw','There\'s always something there to remind me',1,0,58),(19,0,'Cocktail Trio','Hup hup hup',1,0,59),(20,0,'Lorne Greene','Ringo',1,0,60),(21,0,'The Rolling Stones','Little red rooster',1,0,61),(22,0,'Freddy Quinn','Vergangen vergessen voruber',1,0,62),(23,0,'The Supremes','Where did our love go',1,0,63),(24,0,'Geschwister Jakob','Tr?ume der liebe',1,0,64),(25,0,'Ronny','Kenn ein land',1,0,65),(26,0,'The Beatles','If I fell',1,0,66),(27,0,'Petula Clark','Downtown',1,0,67),(28,0,'Newbeats','Everything\'s allright',1,0,68),(29,0,'Johnny Kendall & The Heralds','Jezebel',1,0,69),(30,0,'Manuela','Schneemann',1,0,70),(31,0,'The Supremes','Come see about me',1,0,71),(32,0,'ZZ & De Maskers','Ik heb genoeg van jou',1,0,72),(33,0,'Trini Lopez','Adalita',1,0,73),(34,0,'Bobby Solo','Una la crima sul visa',1,0,74),(35,0,'Rene Carol','Bianca rosa',1,0,75),(36,0,'Jim Reeves','I won\'t forget you',1,0,76),(37,0,'Jay & The Americans','Come a little bit closer',1,0,77),(38,0,'Manfred Mann','Sha la la',1,0,78),(39,0,'Los Indios Tabajaras','Maria Elena',1,0,79),(40,0,'The Beatles','I should have known better',1,0,80),(7,0,'Gert & Hermien','Der bunte hochzeitswagen',1,0,81),(25,0,'Ronny','Kleine Annabel',1,0,82);
/*!40000 ALTER TABLE `top 40` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Current Database: `simple`
--

USE `simple`;

--
-- Final view structure for view `count by artist`
--

/*!50001 DROP TABLE IF EXISTS `count by artist`*/;
/*!50001 DROP VIEW IF EXISTS `count by artist`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `count by artist` AS select `top 40`.`Artist` AS `Artist`,count(`top 40`.`Artist`) AS `CountOfArtist` from `top 40` group by `top 40`.`Artist` order by `top 40`.`Artist` */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2010-05-03 22:23:10
