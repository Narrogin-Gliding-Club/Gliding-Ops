SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL';

DROP SCHEMA IF EXISTS `Logkeeper` ;
CREATE SCHEMA IF NOT EXISTS `Logkeeper` DEFAULT CHARACTER SET latin1 COLLATE latin1_swedish_ci ;
USE `Logkeeper` ;

-- -----------------------------------------------------
-- Table `Logkeeper`.`Member`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `Logkeeper`.`Member` ;

CREATE  TABLE IF NOT EXISTS `Logkeeper`.`Member` (
  `idMember` INT NOT NULL ,
  `Name` VARCHAR(45) NULL ,
  PRIMARY KEY (`idMember`) )
ENGINE = ndbcluster
COMMENT = 'Current members\n';

CREATE UNIQUE INDEX `idMember_UNIQUE` ON `Logkeeper`.`Member` (`idMember` ASC) ;


-- -----------------------------------------------------
-- Table `Logkeeper`.`Member_attrib`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `Logkeeper`.`Member_attrib` ;

CREATE  TABLE IF NOT EXISTS `Logkeeper`.`Member_attrib` (
  `idMember` INT NOT NULL ,
  `Cleared_to_fly` TINYINT(1) NULL ,
  PRIMARY KEY (`idMember`) )
ENGINE = ndbcluster
COMMENT = 'Members\' attributes';

CREATE UNIQUE INDEX `idMember_attrib_UNIQUE` ON `Logkeeper`.`Member_attrib` (`idMember` ASC) ;


-- -----------------------------------------------------
-- Table `Logkeeper`.`Aircraft`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `Logkeeper`.`Aircraft` ;

CREATE  TABLE IF NOT EXISTS `Logkeeper`.`Aircraft` (
  `idAircraft` INT NOT NULL ,
  `Registration` VARCHAR(45) NULL ,
  PRIMARY KEY (`idAircraft`) )
ENGINE = ndbcluster;


-- -----------------------------------------------------
-- Table `Logkeeper`.`Aircraft_attrib`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `Logkeeper`.`Aircraft_attrib` ;

CREATE  TABLE IF NOT EXISTS `Logkeeper`.`Aircraft_attrib` (
  `idAircraft` INT NOT NULL ,
  `Hours` TIME NULL ,
  `Landings` INT(11) NULL ,
  `Form2_valid_to` DATE NULL ,
  `Number_of_places` INT(11) NULL ,
  `Tug` VARCHAR(45) NULL ,
  PRIMARY KEY (`idAircraft`) )
ENGINE = ndbcluster;

CREATE UNIQUE INDEX `idAircraft_UNIQUE` ON `Logkeeper`.`Aircraft_attrib` (`idAircraft` ASC) ;


-- -----------------------------------------------------
-- Table `Logkeeper`.`Member_financial`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `Logkeeper`.`Member_financial` ;

CREATE  TABLE IF NOT EXISTS `Logkeeper`.`Member_financial` (
  `idMember` INT NOT NULL ,
  `Balance` DECIMAL(10,0) NULL ,
  PRIMARY KEY (`idMember`) )
ENGINE = ndbcluster;


-- -----------------------------------------------------
-- Table `Logkeeper`.`Filghts`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `Logkeeper`.`Filghts` ;

CREATE  TABLE IF NOT EXISTS `Logkeeper`.`Filghts` (
  `idFilghts` INT NOT NULL ,
  `Aircraft` INT NULL ,
  `Pilot` INT NULL ,
  `Second` INT NULL ,
  `Type` INT NULL ,
  `Tug` INT NULL ,
  `Launch` DATETIME NULL ,
  `TugLanding` DATETIME NULL ,
  `GliderLanding` DATETIME NULL ,
  `TugCost` DECIMAL(10,0) NULL ,
  `GliderCost` DECIMAL(10,0) NULL ,
  PRIMARY KEY (`idFilghts`) )
ENGINE = ndbcluster;

CREATE UNIQUE INDEX `idFilghts_UNIQUE` ON `Logkeeper`.`Filghts` (`idFilghts` ASC) ;


-- -----------------------------------------------------
-- Table `Logkeeper`.`Aircraft_cost`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `Logkeeper`.`Aircraft_cost` ;

CREATE  TABLE IF NOT EXISTS `Logkeeper`.`Aircraft_cost` (
  `idAircraft` INT NOT NULL ,
  `PerMinute` DECIMAL(10,0) NULL ,
  `PerLaunch` DECIMAL(10,0) NULL ,
  PRIMARY KEY (`idAircraft`) )
ENGINE = ndbcluster;


-- -----------------------------------------------------
-- Placeholder table for view `Logkeeper`.`Flights`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `Logkeeper`.`Flights` (`'*'` INT);

-- -----------------------------------------------------
-- View `Logkeeper`.`Flights`
-- -----------------------------------------------------
DROP VIEW IF EXISTS `Logkeeper`.`Flights` ;
DROP TABLE IF EXISTS `Logkeeper`.`Flights`;
USE `Logkeeper`;
CREATE  OR REPLACE VIEW `mydb`.`Flights` AS SELECT '*' FROM Flights;
;


SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
