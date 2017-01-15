-- MySQL Workbench Forward Engineering

SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE=`TRADITIONAL,ALLOW_INVALID_DATES`;

-- -----------------------------------------------------
-- Schema rumah_susun
-- -----------------------------------------------------

-- -----------------------------------------------------
-- Schema rumah_susun
-- -----------------------------------------------------
DROP SCHEMA IF EXISTS `rumah_susun`;
CREATE SCHEMA IF NOT EXISTS `rumah_susun` DEFAULT CHARACTER SET utf8 ;
USE `rumah_susun` ;

-- -----------------------------------------------------
-- Table `rumah_susun`.`admin`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `rumah_susun`.`admin`;
CREATE TABLE IF NOT EXISTS `rumah_susun`.`admin` (
  `id_user` INT NOT NULL AUTO_INCREMENT,
  `user_name` VARCHAR(45) NOT NULL,
  `password` VARCHAR(45) NOT NULL,
  `create_date` DATETIME NOT NULL,
  `update_date` DATETIME NOT NULL,
  `create_by` INT NOT NULL,
  `update_by` INT NOT NULL,
  `status` INT NOT NULL DEFAULT 0,
  `rfid` VARCHAR(45),
  PRIMARY KEY (`id_user`, `user_name`, `rfid`))
ENGINE = InnoDB;

INSERT INTO `rumah_susun`.`admin` (
  user_name, password, create_date, update_date, create_by, update_by, status
) VALUE (
  'admin', 'admin', now(), now(), 1, 1, 1
);


-- -----------------------------------------------------
-- Table `rumah_susun`.`rusun`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `rumah_susun`.`rusun`;
CREATE TABLE IF NOT EXISTS `rumah_susun`.`rusun` (
  `nomor_kamar` INT NOT NULL,
  `id_kepala_keluarga` INT,
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
DROP TABLE IF EXISTS `rumah_susun`.`keluarga`;
CREATE TABLE IF NOT EXISTS `rumah_susun`.`keluarga` (
  `id_keluarga` INT NOT NULL AUTO_INCREMENT,
  `id_identitas` INT NOT NULL,
  `id_kepala_keluarga` INT NOT NULL DEFAULT 0,
  `create_date` DATETIME NOT NULL,
  `update_date` DATETIME NOT NULL,
  `create_by` INT NOT NULL,
  `update_by` INT NOT NULL,
  `status` INT NOT NULL DEFAULT 0,
  `rfid` VARCHAR(45),
  `finger_id` INT,
  PRIMARY KEY (`id_keluarga`, `id_identitas`, `rfid`, `finger_id`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `rumah_susun`.`identitas`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `rumah_susun`.`identitas`;
CREATE TABLE IF NOT EXISTS `rumah_susun`.`identitas` (
  `id_identitas` INT NOT NULL AUTO_INCREMENT,
  `nomor_ktp` VARCHAR(45) NOT NULL,
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
  PRIMARY KEY (`id_identitas`, `nomor_ktp`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `rumah_susun`.`log_history`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `rumah_susun`.`log_history`;
CREATE TABLE IF NOT EXISTS `rumah_susun`.`log_history` (
  `id_log` INT NOT NULL AUTO_INCREMENT,
  `action` TEXT NOT NULL,
  `create_date` DATETIME NOT NULL,
  `create_by` INT NOT NULL,
  PRIMARY KEY (`id_log`))
ENGINE = InnoDB;

-- ------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------
-- Membuat Store Procedure --
DELIMITER $$
USE `rumah_susun`$$
DROP PROCEDURE IF EXISTS `tambah_admin`$$
CREATE PROCEDURE `tambah_admin`(
	in v_username varchar(45), -- wajib
	in v_password varchar(45), -- wajib
	in v_RFID varchar(45),     -- wajib
	in v_user int(5),          -- wajib
	out v_result varchar(100)
)
BEGIN
  set @validation = 0;
	select count(*) into @validation from rumah_susun.admin where status = 1 and user_name = v_username;
	if @validation = 0 then
		insert into rumah_susun.admin (
			user_name, password, rfid, create_date, update_date, create_by, update_by, status
		) value (
			v_username, v_password, v_RFID, now(), now(), v_user, v_user, 1
		);

		insert into rumah_susun.log_history (
			action, create_date, create_by
		) value (
			concat('Berhasil tambah admin dengan username : ', v_username, ' ..')
			, now(), v_user
		);
		set v_result = concat('Berhasil tambah admin dengan username : ', v_username, ' ..');
	else
		insert into rumah_susun.log_history (
			action, create_date, create_by
		) value (
			concat('Gagal tambah admin dengan username : ', v_username, ' ,, username sudah ada ..')
			, now(), v_user
		);
		set v_result = concat('Gagal tambah admin dengan username : ', v_username, ' ,, username sudah ada ..');
	end if;
END$$
DELIMITER ;
-- ------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------
DELIMITER $$
USE `rumah_susun`$$
DROP PROCEDURE IF EXISTS `edit_admin`$$
CREATE PROCEDURE `edit_admin`(
	in v_username varchar(45), -- wajib
	in v_password varchar(45), -- wajib
	in v_RFID varchar(45),     -- wajib
	in v_user int(5),          -- wajib
	out v_result varchar(100)
)
BEGIN
  set @validation = 0;
  select count(*) into @validation from rumah_susun.admin where status = 1 and user_name = v_username;
	if @validation > 0 then
		update rumah_susun.admin set
			password = v_password,
			rfid = v_RFID,
      update_date = now(),
      update_by = v_user
		where
			status = 1 and
			user_name = v_username
		;

		insert into rumah_susun.log_history (
			action, create_date, create_by
		) value (
			concat('Berhasil edit admin dengan username : ', v_username, ' ..')
			, now(), v_user
		);
		set v_result = concat('Berhasil edit admin dengan username : ', v_username, ' ..');
	else
		insert into rumah_susun.log_history (
			action, create_date, create_by
		) value (
			concat('Gagal edit admin dengan username : ', v_username, ' ,, Username Belum Terdaftar ..')
			, now(), v_user
		);
		set v_result = concat('Gagal edit admin dengan username : ', v_username, ' ,, Username Belum Terdaftar ..');
	end if;
END$$
DELIMITER ;
-- ------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------
DELIMITER $$
USE `rumah_susun`$$
DROP PROCEDURE IF EXISTS `non_aktifkan_admin`$$
CREATE PROCEDURE `non_aktifkan_admin`(
	in v_id_user int(5),       -- wajib
	in v_user int(5),          -- wajib
	out v_result varchar(100)
)
BEGIN
  set @validation = 0; set @v_username = '';
  if v_user != v_id_user then
		select count(*) into @validation from rumah_susun.admin where id_user = v_id_user and status = 1;
		if @validation > 0 then
			select user_name into @v_username from rumah_susun.admin where id_user = v_id_user and status = 1;
			update rumah_susun.admin set status = 0 where id_user = v_id_user and status = 1;

			insert into rumah_susun.log_history (
				action, create_date, create_by
			) value (
				concat('Berhasil menonaktifkan admin dengan username : ', @v_username, ' ..')
				, now(), v_user
			);
			set v_result = concat('Berhasil menonaktifkan admin dengan username : ', @v_username, ' ..');
		else
			insert into rumah_susun.log_history (
				action, create_date, create_by
			) value (
				concat('Gagal menonaktifkan admin ,, admin dengan id ', v_id_user, ' tidak ditemukan ..')
				, now(), v_user
			);
			set v_result = concat('Gagal menonaktifkan admin ,, admin dengan id ', v_id_user, ' tidak ditemukan ..');
		end if;
	else
		insert into rumah_susun.log_history (
			action, create_date, create_by
		) value (
			'Gagal menonaktifkan admin ,, tidak boleh menonaktifkan admin sendiri ..'
			, now(), v_user
		);
		set v_result = 'Gagal menonaktifkan admin ,, tidak boleh menonaktifkan admin sendiri ..';
	end if;
END$$
DELIMITER ;
-- ------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------
DELIMITER $$
USE `rumah_susun`$$
DROP PROCEDURE IF EXISTS `insert_kamar_baru`$$
CREATE PROCEDURE `insert_kamar_baru`(
	in v_nomor_kamar varchar(45),  -- wajib
	in v_user int(5),              -- wajib
	out v_result varchar(100)
)
BEGIN
  set @validation = 0;
  select count(*) into @validation from rumah_susun.rusun where nomor_kamar = v_nomor_kamar;
	if @validation > 0 then
		insert into rumah_susun.log_history (
			action, create_date, create_by
		) value (
			concat('Nomor Kamar : ', convert(v_nomor_kamar, char), ' Sudah ada ..')
			, now(), v_user
		);
		set v_result = concat('Nomor Kamar : ', convert(v_nomor_kamar, char), ' Sudah ada ..');
	else
		insert into rumah_susun.rusun (
			nomor_kamar, create_date, update_date, create_by, update_by, status
		) value (
			v_nomor_kamar, now(), now(), v_user, v_user, 0
		);

		insert into rumah_susun.log_history (
			action, create_date, create_by
		) value (
			concat('Berhasil Tambah Kamar, Nomor Kamar : ', convert(v_nomor_kamar, char), ' ..')
			, now(), v_user
		);
		set v_result = concat('Berhasil Tambah Kamar, Nomor Kamar : ', convert(v_nomor_kamar, char), ' ..');
	end if;
END$$
DELIMITER ;
-- ------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------
DELIMITER $$
USE `rumah_susun`$$
DROP PROCEDURE IF EXISTS `insert_kepala_keluarga_baru`$$
CREATE PROCEDURE `insert_kepala_keluarga_baru`(
	in v_nomor_ktp varchar(45),      -- wajib
	in v_nama varchar(45),           -- wajib
	in v_tempat_lahir varchar(45),   -- wajib
	in v_tanggal_lahir varchar(45),  -- wajib  format: dd-mm-YYYY
	in v_agama varchar(45),          -- wajib
	in v_nomor_hp varchar(45),       -- wajib
	in v_foto varchar(45),           -- wajib
	in v_user int(5),                -- wajib
	in v_fingerID int(5),            -- wajib
	in v_RFID varchar(45),           -- wajib
	in v_nomor_kamar int(5),         -- wajib
	out v_result varchar(200)
)
BEGIN
  set @validation = 0;
  select count(*) into @validation from rumah_susun.rusun where nomor_kamar = v_nomor_kamar and status = 1;
	if @validation = 0 then
    select count(*) into @validation from rumah_susun.identitas where nomor_ktp = v_nomor_ktp and status = 1;
    if @validation > 0 then
      insert into rumah_susun.log_history (
        action, create_date, create_by
      ) value (
        concat('Gagal Tambah Kepala Keluarga, nama : ', v_nama, ', ',
               'Nomor KTP : ', v_nomor_ktp, ', ',
               'Di Kamar : ', v_nomor_kamar, ' ,, Nomor KTP Sudah Terdaftar ..')
        , now(), v_user
      );
      set v_result = concat('Gagal Tambah Kepala Keluarga, nama : ', v_nama, ', ',
                            'Nomor KTP : ', v_nomor_ktp, ', ',
                            'Di Kamar : ', v_nomor_kamar, ' ,, Nomor KTP Sudah Terdaftar ..');
    else
      select count(*) into @validation from rumah_susun.rusun where nomor_kamar = v_nomor_kamar;
      if @validation > 0 then
        -- 1. Insert Identitas Kepala Keluarga --
        insert into rumah_susun.identitas (
          nomor_ktp, nama, tempat_lahir, tanggal_lahir, agama, nomor_hp, foto, create_date, update_date, create_by, update_by, status
        ) value (
          v_nomor_ktp, v_nama, v_tempat_lahir, str_to_date(v_tanggal_lahir, '%d-%m-%Y'), v_agama, v_nomor_hp, v_foto, now(), now(), v_user, v_user, 1
        );

        -- 2. Insert Kepala Keluarga ke Table Keluarga --
        insert into rumah_susun.keluarga (
          id_identitas, create_date, update_date, create_by, update_by, status, rfid, finger_id
        ) value (
          (select max(id_identitas) from rumah_susun.identitas), now(), now(), v_user, v_user, 1, v_RFID, v_fingerID
        );

        -- 3. Update Table rusun dengan id_kepala_keluarga --
        update rumah_susun.rusun set
          id_kepala_keluarga = (select max(id_keluarga) from rumah_susun.keluarga where status = 1),
          status = 1,
          update_date = now(),
          update_by = v_user
        where
          nomor_kamar = v_nomor_kamar and
          status = 0
        ;

        insert into rumah_susun.log_history (
          action, create_date, create_by
        ) value (
          concat('Berhasil Tambah Kepala Keluarga, nama : ', v_nama, ', ',
                'Nomor KTP : ', v_nomor_ktp, ', ',
                'Di Kamar : ', v_nomor_kamar, ' ..')
          , now(), v_user
        );
        set v_result = concat('Berhasil Tambah Kepala Keluarga, nama : ', v_nama, ', ',
                              'Nomor KTP : ', v_nomor_ktp, ', ',
                              'Di Kamar : ', v_nomor_kamar, ' ..');
      else
        insert into rumah_susun.log_history (
          action, create_date, create_by
        ) value (
          concat('Gagal Tambah Kepala Keluarga, nama : ', v_nama, ', ',
                 'Nomor KTP : ', v_nomor_ktp, ', ',
                 'Di Kamar : ', v_nomor_kamar, ' ,, Kamar Belum Ada ..')
          , now(), v_user
        );
        set v_result = concat('Gagal Tambah Kepala Keluarga, nama : ', v_nama, ', ',
                              'Nomor KTP : ', v_nomor_ktp, ', ',
                              'Di Kamar : ', v_nomor_kamar, ' ,, Kamar Belum Ada ..');
      end if;
    end if;
	else
		insert into rumah_susun.log_history (
			action, create_date, create_by
		) value (
			concat('Gagal Tambah Kepala Keluarga, nama : ', v_nama, ', ',
			       'Nomor KTP : ', v_nomor_ktp, ', ',
			       'Di Kamar : ', v_nomor_kamar, ' ,, Kamar Sudah Terisi ..')
			, now(), v_user
		);
		set v_result = concat('Gagal Tambah Kepala Keluarga, nama : ', v_nama, ', ',
                          'Nomor KTP : ', v_nomor_ktp, ', ',
                          'Di Kamar : ', v_nomor_kamar, ' ,, Kamar Sudah Terisi ..');
	end if;
END$$
DELIMITER ;
-- ------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------
DELIMITER $$
USE `rumah_susun`$$
DROP PROCEDURE IF EXISTS `insert_anggota_keluarga_baru`$$
CREATE PROCEDURE `insert_anggota_keluarga_baru`(
	in v_nomor_ktp varchar(45),      -- wajib
	in v_nama varchar(45),           -- wajib
	in v_tempat_lahir varchar(45),   -- wajib
	in v_tanggal_lahir varchar(45),  -- wajib  format: dd-mm-YYYY
	in v_agama varchar(45),          -- wajib
	in v_nomor_hp varchar(45),       -- wajib
	in v_foto varchar(45),           -- wajib
	in v_user int(5),                -- wajib
	in v_fingerID int(5),            -- wajib
	in v_RFID varchar(45),           -- wajib
	in v_nomor_kamar int(5),         -- wajib
	out v_result varchar(200)
)
BEGIN
  set @validation = 0;
  select count(*) into @validation from rumah_susun.rusun where nomor_kamar = v_nomor_kamar and status = 1;
	if @validation > 0 then
    select case when v_nomor_ktp = '' then 0 else count(*) end into @validation from rumah_susun.identitas where nomor_ktp = v_nomor_ktp and status = 1;
    if @validation > 0 then
      insert into rumah_susun.log_history (
        action, create_date, create_by
      ) value (
        concat('Gagal Tambah Anggota Keluarga, nama : ', v_nama, ', ',
               'Nomor KTP : ', v_nomor_ktp, ', ',
               'Di Kamar : ', v_nomor_kamar, ' ,, Nomor KTP Sudah Pernah Terdaftar ..')
        , now(), v_user
      );
      set v_result = concat('Gagal Tambah Anggota Keluarga, nama : ', v_nama, ', ',
                            'Nomor KTP : ', v_nomor_ktp, ', ',
                            'Di Kamar : ', v_nomor_kamar, ' ,, Nomor KTP Sudah Pernah Terdaftar ..');
    else
      -- 4. Insert Anggota Keluarga ke Table Identitas --
      insert into rumah_susun.identitas (
        nomor_ktp, nama, tempat_lahir, tanggal_lahir, agama, nomor_hp, foto, create_date, update_date, create_by, update_by, status
      ) value (
        v_nomor_ktp, v_nama, v_tempat_lahir, str_to_date(v_tanggal_lahir, '%d-%m-%Y'), v_agama, v_nomor_hp, v_foto, now(), now(), v_user, v_user, 1
      );

      -- 5. Insert Anggota Keluarga ke Table Keluarga --
      insert into rumah_susun.keluarga (
        id_identitas, id_kepala_keluarga, create_date, update_date, create_by, update_by, status, rfid, finger_id
      ) value (
        (select max(id_identitas) from rumah_susun.identitas),
        (select id_kepala_keluarga from rumah_susun.rusun where nomor_kamar = v_nomor_kamar and status = 1),
        now(), now(), v_user, v_user, 1, v_RFID, v_fingerID
      );

      insert into rumah_susun.log_history (
        action, create_date, create_by
      ) value (
        concat('Berhasil Tambah Anggota Keluarga, nama : ', v_nama, ', ',
               'Nomor KTP : ', v_nomor_ktp, ', ',
               'Di Kamar : ', v_nomor_kamar, ' ..')
        , now(), v_user
      );
      set v_result = concat('Berhasil Tambah Anggota Keluarga, nama : ', v_nama, ', ',
                            'Nomor KTP : ', v_nomor_ktp, ', ',
                            'Di Kamar : ', v_nomor_kamar, ' ..');
    end if;
	else
		insert into rumah_susun.log_history (
			action, create_date, create_by
		) value (
			concat('Gagal Tambah Anggota Keluarga, nama : ', v_nama, ', ',
			       'Nomor KTP : ', v_nomor_ktp, ', ',
			       'Di Kamar : ', v_nomor_kamar, ' ,, Kamar Belum Memiliki Kepala Keluarga ..')
			, now(), v_user
		);
		set v_result = concat('Gagal Tambah Anggota Keluarga, nama : ', v_nama, ', ',
                          'Nomor KTP : ', v_nomor_ktp, ', ',
                          'Di Kamar : ', v_nomor_kamar, ' ,, Kamar Belum Memiliki Kepala Keluarga ..');
	end if;
END$$
DELIMITER ;
-- ------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------
DELIMITER $$
USE `rumah_susun`$$
DROP PROCEDURE IF EXISTS `update_identitas_keluarga`$$
CREATE PROCEDURE `update_identitas_keluarga`(
	in v_nomor_identitas int(5),     -- wajib
	in v_nomor_ktp varchar(45),      -- wajib
	in v_nama varchar(45),           -- wajib
	in v_tempat_lahir varchar(45),   -- wajib
	in v_tanggal_lahir varchar(45),  -- wajib  format: dd-mm-YYYY
	in v_agama varchar(45),          -- wajib
	in v_nomor_hp varchar(45),       -- wajib
	in v_foto varchar(45),           -- wajib
	in v_user int(5),                -- wajib
	in v_fingerID int(5),            -- wajib
	in v_RFID varchar(45),           -- wajib
	in v_nomor_kamar int(1),         -- wajib
	out v_result varchar(200)        -- wajib
)
BEGIN
  set @validation = 0;
  select count(*) into @validation from rumah_susun.identitas ident, rumah_susun.keluarga kel, rumah_susun.rusun rsn
	where ident.status = 1 and kel.status = 1 and rsn.status = 1 and ident.id_identitas = v_nomor_identitas;
	if @validation > 0 then
		update rumah_susun.identitas set
			-- nomor_ktp = v_nomor_ktp,
			nama = v_nama,
			tempat_lahir = v_tempat_lahir,
			tanggal_lahir = str_to_date(v_tanggal_lahir, '%d-%m-%Y'),
			agama = v_agama,
			nomor_hp = v_nomor_hp,
			foto = v_foto,
			update_date = now(),
			update_by = v_user
		where
			id_identitas = v_nomor_identitas and
			status = 1
		;

		update rumah_susun.keluarga set
			update_date = now(),
			update_by = v_user,
			rfid = v_RFID,
			finger_id = v_fingerID
		where
			id_identitas = v_nomor_identitas and
			status = 1
		;

		insert into rumah_susun.log_history (
			action, create_date, create_by
		) value (
			concat('Berhasil Update Data Anggota Keluarga, nama : ', v_nama, ', ',
			       'Nomor KTP : ', v_nomor_ktp, ', ',
			       'Di Kamar : ', v_nomor_kamar, ' ..')
			, now(), v_user
		);
		set v_result = concat('Berhasil Update Data Anggota Keluarga, nama : ', v_nama, ', ',
                          'Nomor KTP : ', v_nomor_ktp, ', ',
                          'Di Kamar : ', v_nomor_kamar, ' ..');
	else
		insert into rumah_susun.log_history (
			action, create_date, create_by
		) value (
			concat('Gagal Update Data Anggota Keluarga, nama : ', v_nama, ', ',
			       'Nomor KTP : ', v_nomor_ktp, ', ',
			       'Di Kamar : ', v_nomor_kamar, ' ,, Anggota Keluarga dengan id ', v_nomor_identitas, ' tidak terdaftar ..')
			, now(), v_user
		);
		set v_result = concat('Gagal Update Data Anggota Keluarga, nama : ', v_nama, ', ',
                          'Nomor KTP : ', v_nomor_ktp, ', ',
                          'Di Kamar : ', v_nomor_kamar, ' ,, Anggota Keluarga dengan id ', v_nomor_identitas, ' tidak terdaftar ..');
	end if;
END$$
DELIMITER ;
-- ------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------
DELIMITER $$
USE `rumah_susun`$$
DROP PROCEDURE IF EXISTS `non_aktifkan_anggota_keluarga`$$
CREATE PROCEDURE `non_aktifkan_anggota_keluarga`(
	in v_nomor_identitas int(5), -- wajib
	in v_user int(5),            -- wajib
	out v_result varchar(200)
)
BEGIN
  set @v_nama = ''; set @v_id_kepala_keluarga = 0; set @v_nomor_kamar = 0;
	select nama into @v_nama from rumah_susun.identitas where id_identitas = v_nomor_identitas;
	select id_kepala_keluarga into @v_id_kepala_keluarga from rumah_susun.keluarga where id_identitas = v_nomor_identitas;
	select distinct rsn.nomor_kamar into @v_nomor_kamar
	from rumah_susun.rusun rsn, rumah_susun.keluarga klg
	where rsn.id_kepala_keluarga = (select
																		case id_kepala_keluarga
																			when 0 then id_keluarga
																			else id_kepala_keluarga
																		end
										 							from rumah_susun.keluarga where id_identitas = v_nomor_identitas);
	if @v_id_kepala_keluarga > 0 then
		update rumah_susun.identitas set status = 0
		where id_identitas = v_nomor_identitas;
		update rumah_susun.keluarga set status = 0
		where id_identitas = v_nomor_identitas;

		insert into rumah_susun.log_history (
			action, create_date, create_by
		) value (
			concat('Anggota Keluarga, nama : ', @v_nama, ', ',
			       'Di Kamar : ', @v_nomor_kamar, ' Dinonaktifkan ..')
			, now(), v_user
		);
    set v_result = concat('Anggota Keluarga, nama : ', @v_nama, ', ',
                          'Di Kamar : ', @v_nomor_kamar, ' Dinonaktifkan ..');
	else
    select id_keluarga into @v_id_kepala_keluarga from rumah_susun.keluarga where id_identitas = v_nomor_identitas;
    -- set @child = select id_identitas from rumah_susun.keluarga where id_kepala_keluarga = @v_id_kepala_keluarga;

		update rumah_susun.identitas set status = 0
    where id_identitas = v_nomor_identitas or id_identitas in (select id_identitas from rumah_susun.keluarga where id_kepala_keluarga = @v_id_kepala_keluarga);
		update rumah_susun.keluarga set status = 0
    where id_identitas in (select id_identitas from rumah_susun.identitas where status = 0);
    update rumah_susun.rusun set status = 0
    where id_kepala_keluarga = @v_id_kepala_keluarga;

		insert into rumah_susun.log_history (
			action, create_date, create_by
		) value (
			concat('Kepala Keluarga, nama : ', @v_nama, ', ',
			       'Di Kamar : ', @v_nomor_kamar, ' Beserta Aanggota Keluarga Dinonaktifkan ..')
			, now(), v_user
		);
    set v_result = concat('Kepala Keluarga, nama : ', @v_nama, ', ',
                          'Di Kamar : ', @v_nomor_kamar, ' Beserta Aanggota Keluarga Dinonaktifkan ..');
	end if;
END$$
DELIMITER ;
-- ------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------
DELIMITER $$
USE `rumah_susun`$$
DROP PROCEDURE IF EXISTS `delete_non_aktif_data`$$
CREATE PROCEDURE `delete_non_aktif_data`(
  in v_user int(5),           -- wajib
  out v_result varchar(100)
)
BEGIN
	delete from rumah_susun.admin where status = 0;
	delete from rumah_susun.identitas where status = 0;
	delete from rumah_susun.keluarga where status = 0;

	insert into rumah_susun.log_history (
		action, create_date, create_by
	) value (
		'Berhasil Hapus Data Yang Berstatus Tidak Aktif ..'
		, now(), v_user
	);
  set v_result = 'Berhasil Hapus Data Yang Berstatus Tidak Aktif ..';
END$$
DELIMITER ;
-- ------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------
DELIMITER $$
USE `rumah_susun`$$
DROP PROCEDURE IF EXISTS `get_data_rusun_kepala`$$
CREATE PROCEDURE `get_data_rusun_kepala`()
BEGIN
	select
		rsn.nomor_kamar,
		rsn.id_kepala_keluarga,
		(select nomor_ktp from rumah_susun.identitas where id_identitas = (select id_identitas from rumah_susun.keluarga where id_keluarga = rsn.id_kepala_keluarga)) as nomor_ktp,
		(select nama from rumah_susun.identitas where id_identitas = (select id_identitas from rumah_susun.keluarga where id_keluarga = rsn.id_kepala_keluarga)) as nama,
		(select tempat_lahir from rumah_susun.identitas where id_identitas = (select id_identitas from rumah_susun.keluarga where id_keluarga = rsn.id_kepala_keluarga)) as tempat_lahir,
		(select tanggal_lahir from rumah_susun.identitas where id_identitas = (select id_identitas from rumah_susun.keluarga where id_keluarga = rsn.id_kepala_keluarga)) as tanggal_lahir,
		(select agama from rumah_susun.identitas where id_identitas = (select id_identitas from rumah_susun.keluarga where id_keluarga = rsn.id_kepala_keluarga)) as agama,
		(select nomor_hp from rumah_susun.identitas where id_identitas = (select id_identitas from rumah_susun.keluarga where id_keluarga = rsn.id_kepala_keluarga)) as nomor_hp,
		(select foto from rumah_susun.identitas where id_identitas = (select id_identitas from rumah_susun.keluarga where id_keluarga = rsn.id_kepala_keluarga)) as foto,
		(select finger_id from rumah_susun.keluarga where id_keluarga = rsn.id_kepala_keluarga) as finger_id,
		(select rfid from rumah_susun.keluarga where id_keluarga = rsn.id_kepala_keluarga) as rfid
	from rumah_susun.rusun rsn;
END$$
DELIMITER ;
-- ------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------
DELIMITER $$
USE `rumah_susun`$$
DROP PROCEDURE IF EXISTS `get_data_rusun_anggota`$$
CREATE PROCEDURE `get_data_rusun_anggota`(in v_id_kepala_keluarga int(5))
BEGIN
	select
		(select nomor_kamar from rumah_susun.rusun where id_kepala_keluarga = kel.id_kepala_keluarga) as nomor_kamar,
		kel.id_kepala_keluarga,
		(select nomor_ktp from rumah_susun.identitas where id_identitas = kel.id_identitas) as nomor_ktp,
		(select nama from rumah_susun.identitas where id_identitas = kel.id_identitas) as nama,
		(select tempat_lahir from rumah_susun.identitas where id_identitas = kel.id_identitas) as tempat_lahir,
		(select tanggal_lahir from rumah_susun.identitas where id_identitas = kel.id_identitas) as tanggal_lahir,
		(select agama from rumah_susun.identitas where id_identitas = kel.id_identitas) as agama,
		(select nomor_hp from rumah_susun.identitas where id_identitas = kel.id_identitas) as nomor_hp,
		(select foto from rumah_susun.identitas where id_identitas = kel.id_identitas) as foto,
		kel.finger_id,
		kel.rfid
	from rumah_susun.keluarga kel
	where kel.id_kepala_keluarga = v_id_kepala_keluarga and kel.status = 1 and
		(select status from rumah_susun.rusun where id_kepala_keluarga = v_id_kepala_keluarga) = 1;
END$$
DELIMITER ;
-- ------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------
DELIMITER $$
USE `rumah_susun`$$
DROP PROCEDURE IF EXISTS `get_kamar_tersedia`$$
CREATE PROCEDURE `get_kamar_tersedia`()
BEGIN
	SELECT nomor_kamar from rumah_susun.rusun where status = 0;
END$$
DELIMITER ;
-- ------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------
DELIMITER $$
USE `rumah_susun`$$
DROP PROCEDURE IF EXISTS `check_available_finger_id`$$
CREATE PROCEDURE `check_available_finger_id`(in v_finger_id int(5))
BEGIN
	SELECT true from rumah_susun.keluarga where finger_id = v_finger_id and status = 1;
END$$
DELIMITER ;
-- ------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------
DELIMITER $$
USE `rumah_susun`$$
DROP PROCEDURE IF EXISTS `check_available_rfid`$$
CREATE PROCEDURE `check_available_rfid`(in v_rfid varchar(10))
BEGIN
	SELECT true from rumah_susun.keluarga where rfid = v_rfid and status = 1;
END$$
DELIMITER ;
-- ------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------

SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
