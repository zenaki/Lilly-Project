-- MySQL Workbench Forward Engineering

SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL,ALLOW_INVALID_DATES';

-- -----------------------------------------------------
-- Schema rumah_susun
-- -----------------------------------------------------

-- -----------------------------------------------------
-- Schema rumah_susun
-- -----------------------------------------------------
CREATE SCHEMA IF NOT EXISTS `rumah_susun` DEFAULT CHARACTER SET utf8 ;
USE `rumah_susun` ;

-- -----------------------------------------------------
-- Table `rumah_susun`.`admin`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `rumah_susun`.`admin` (
  `id_user` INT NOT NULL AUTO_INCREMENT,
  `user_name` VARCHAR(45) NOT NULL,
  `password` VARCHAR(45) NOT NULL,
  `create_date` DATETIME NOT NULL,
  `update_date` DATETIME NOT NULL,
  `create_by` INT NOT NULL,
  `update_by` INT NOT NULL,
  `status` INT NOT NULL DEFAULT 0,
  `rfid` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`id_user`, `user_name`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `rumah_susun`.`rusun`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `rumah_susun`.`rusun` (
  `nomor_kamar` INT NOT NULL,
  `id_kepala_keluarga` INT NOT NULL,
  `create_date` DATETIME NOT NULL,
  `update_date` DATETIME NOT NULL,
  `create_by` INT NOT NULL,
  `update_by` INT NOT NULL,
  `status` INT NOT NULL DEFAULT 0,
  PRIMARY KEY (`nomor_kamar`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `rumah_susun`.`keluarga`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `rumah_susun`.`keluarga` (
  `id_keluarga` INT NOT NULL,
  `id_identitas` INT NOT NULL,
  `id_kepala_keluarga` INT NOT NULL,
  `create_date` DATETIME NOT NULL,
  `update_date` DATETIME NOT NULL,
  `create_by` INT NOT NULL,
  `update_by` INT NOT NULL,
  `status` INT NOT NULL DEFAULT 0,
  `rfid` VARCHAR(45) NOT NULL,
  `finger_id` INT NOT NULL,
  PRIMARY KEY (`id_keluarga`, `finger_id`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `rumah_susun`.`identitas`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `rumah_susun`.`identitas` (
  `id_identitas` INT NOT NULL,
  `nomor_ktp` INT NOT NULL,
  `nama` VARCHAR(45) NOT NULL,
  `tempat_lahir` VARCHAR(45) NOT NULL,
  `tanggal_lahir` DATE NOT NULL,
  `agama` VARCHAR(45) NOT NULL,
  `nomor_hp` VARCHAR(45) NOT NULL,
  `foto` VARCHAR(45) NOT NULL,
  `create_date` DATETIME NOT NULL,
  `update_date` DATETIME NOT NULL,
  `create_by` INT NOT NULL,
  `update_by` INT NOT NULL,
  `status` INT NOT NULL DEFAULT 0,
  PRIMARY KEY (`id_identitas`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `rumah_susun`.`log_history`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `rumah_susun`.`log_history` (
  `id_log` INT NOT NULL,
  `action` TEXT NOT NULL,
  `create_date` DATETIME NOT NULL,
  `create_by` INT NOT NULL,
  `time` INT NOT NULL,
  PRIMARY KEY (`id_log`))
ENGINE = InnoDB;


SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
