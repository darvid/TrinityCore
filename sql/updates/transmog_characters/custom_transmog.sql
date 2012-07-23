CREATE TABLE `transmog_sets` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `account_id` int(11) DEFAULT NULL,
  `empty` tinyint(1) NOT NULL,
  `class` tinyint(4) NOT NULL,
  `race` tinyint(4) NOT NULL,
  `rating` smallint(6) NOT NULL,
  `bracket` smallint(6) NOT NULL,
  `helm_id` int(11) DEFAULT NULL,
  `shoulder_id` int(11) DEFAULT NULL,
  `chest_id` int(11) DEFAULT NULL,
  `wrist_id` int(11) DEFAULT NULL,
  `hands_id` int(11) DEFAULT NULL,
  `waist_id` int(11) DEFAULT NULL,
  `legs_id` int(11) DEFAULT NULL,
  `boots_id` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`,`class`,`race`,`rating`,`bracket`)
) ENGINE=MyISAM AUTO_INCREMENT=530 DEFAULT CHARSET=utf8
