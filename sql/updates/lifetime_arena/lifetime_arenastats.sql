DROP TABLE IF EXISTS `lifetime_arena_stats`;
CREATE TABLE `lifetime_arena_stats` (
  `player_guid` INT(11) NOT NULL,
  `bracket` TINYINT NOT NULL,
  `personal_rating` SMALLINT(4) NOT NULL,
  `team_rating` SMALLINT(4) NOT NULL,
  `week_wins` SMALLINT(4) NOT NULL,
  `week_losses` SMALLINT(4) NOT NULL,
  `season_wins` SMALLINT(4) NOT NULL,
  `season_losses` SMALLINT(4) NOT NULL,
  `rank` SMALLINT(4) NOT NULL,
  `mmr` SMALLINT(4) NOT NULL,
  PRIMARY KEY (`player_guid`, `bracket`)
);
