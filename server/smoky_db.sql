
CREATE DATABASE IF NOT EXISTS `smoky_db` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;
USE `smoky_db`;

-- --------------------------------------------------------

--
-- Structure de la table `faces`
--

CREATE TABLE IF NOT EXISTS `faces` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `persons_id` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `persons_id` (`persons_id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;



-- --------------------------------------------------------

--
-- Structure de la table `logs`
--

CREATE TABLE IF NOT EXISTS `logs` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `user_id` int(11) NOT NULL,
  `op_id` int(11) NOT NULL,
  `op_data` varchar(255) NOT NULL,
  `op_date` datetime NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Structure de la table `options`
--

CREATE TABLE IF NOT EXISTS `options` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `option_name` varchar(255) NOT NULL,
  `option_value` varchar(255) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

--
-- Contenu de la table `options`
--

INSERT IGNORE INTO `options` (`id`, `option_name`, `option_value`) VALUES
(1, 'rec_method', '2'),
(2, 'eigenFace_threshold', '1500'),
(3, 'fisherFace_threshold', '50'),
(4, 'LBPHFace_threshold', '70'),
(5, 'eigenFace_rec', '0'),
(6, 'eigenFace_unrec', '0'),
(7, 'eigenFace_ign', '0'),
(8, 'fisherFace_rec', '0'),
(9, 'fisherFace_unrec', '0'),
(10, 'fisherFace_ign', '0'),
(11, 'LBPHFace_rec', '0'),
(12, 'LBPHFace_unrec', '0'),
(13, 'LBPHFace_ign', '0');

-- --------------------------------------------------------

--
-- Structure de la table `persons`
--

CREATE TABLE IF NOT EXISTS `persons` (
  `id` int(11) NOT NULL,
  `firstName` varchar(255) NOT NULL,
  `lastName` varchar(255) NOT NULL,
  `birthday` varchar(255) NOT NULL,
  `address` varchar(255) NOT NULL,
  `sexe` varchar(255) NOT NULL,
  `email` varchar(255) NOT NULL,
  `birthCity` varchar(255) NOT NULL,
  `phone` varchar(255) NOT NULL,
  `profession` varchar(255) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


CREATE TABLE IF NOT EXISTS `users` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `person_id` int(11) NOT NULL,
  `username` varchar(255) NOT NULL,
  `password` varchar(255) NOT NULL,
  `role` varchar(20) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

SELECT * from users;
