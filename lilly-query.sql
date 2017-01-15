SELECT id_user, user_name, password FROM rumah_susun.admin WHERE status = 1;

insert into rumah_susun.admin (
	user_name, password, create_date, update_date, create_by, update_by, status
) value (
	'admin', 'admin', now(), now(), 1, 1, 1
);

insert into rumah_susun.rusun (
	nomor_kamar, create_date, update_date, create_by, update_by
) values (
	1, now(), now(), 1, 1
), (
	2, now(), now(), 1, 1
);

SELECT nomor_kamar from rumah_susun.rusun where status = 0;

-- 1. Insert Identitas Kepala Keluarga --
insert into rumah_susun.identitas (
	nomor_ktp,
	nama,
	tempat_lahir,
	tanggal_lahir,
	agama,
	nomor_hp,
	foto,
	create_date,
	update_date,
	create_by,
	update_by,
	status
) value (
	'1234567890',
	'Bapak 1',
	'Jakarta',
	'1985-11-30',
	'Islam',
	'+6281234567890',
	'',
	now(),
	now(),
	1,
	1,
	1
);

-- 2. Insert Kepala Keluarga ke Table Keluarga --
insert into rumah_susun.keluarga (
	id_identitas,
	create_date,
	update_date,
	create_by,
	update_by,
	status,
	rfid,
	finger_id
) value (
	(select max(id_identitas) from rumah_susun.identitas where status = 1),
	now(),
	now(),
	1,
	1,
	1,
	'#123123',
	100
);

-- 3. Update Table rusun dengan id_kepala_keluarga --
update rumah_susun.rusun set
	id_kepala_keluarga = (select max(id_keluarga) from rumah_susun.keluarga where status = 1),
	status = 1
where
	nomor_kamar = 1 and
	status = 0
;

-- 4. Insert Ibu ke Table Identitas --
insert into rumah_susun.identitas (
	nomor_ktp,
	nama,
	tempat_lahir,
	tanggal_lahir,
	agama,
	nomor_hp,
	foto,
	create_date,
	update_date,
	create_by,
	update_by,
	status
) value (
	'1234567890',
	'Ibu 1',
	'Jakarta',
	'1985-11-30',
	'Islam',
	'+6281234567890',
	'',
	now(),
	now(),
	1,
	1,
	1
);

-- 5. Insert Ibu ke Table Keluarga --
insert into rumah_susun.keluarga (
	id_identitas,
	id_kepala_keluarga,
	create_date,
	update_date,
	create_by,
	update_by,
	status,
	rfid,
	finger_id
) value (
	(select max(id_identitas) from rumah_susun.identitas where status = 1),
	(select id_kepala_keluarga from rumah_susun.rusun where nomor_kamar = 1 and status = 1),
	now(),
	now(),
	1,
	1,
	1,
	'#123123',
	100
);

-- 6. Insert Anak ke Table Identitas --
insert into rumah_susun.identitas (
	nomor_ktp,
	nama,
	tempat_lahir,
	tanggal_lahir,
	agama,
	nomor_hp,
	foto,
	create_date,
	update_date,
	create_by,
	update_by,
	status
) value (
	'',
	'Anak 1',
	'Jakarta',
	'2000-11-30',
	'Islam',
	'+6281234567890',
	'',
	now(),
	now(),
	1,
	1,
	1
);

-- 7. Insert Anak ke Table Keluarga --
insert into rumah_susun.keluarga (
	id_identitas,
	id_kepala_keluarga,
	create_date,
	update_date,
	create_by,
	update_by,
	status,
	rfid,
	finger_id
) value (
	(select max(id_identitas) from rumah_susun.identitas where status = 1),
	(select id_kepala_keluarga from rumah_susun.rusun where nomor_kamar = 1 and status = 1),
	now(),
	now(),
	1,
	1,
	1,
	'#123123',
	100
);

-- Get Kepala keluarga
SELECT
	rsn.nomor_kamar,
	kel.id_keluarga,
	ident.nomor_ktp,
	ident.nama,
	ident.tempat_lahir,
	ident.tanggal_lahir,
	ident.agama,
	ident.nomor_hp,
	ident.foto,
	kel.finger_id,
	kel.rfid
FROM
	rumah_susun.rusun rsn,
	rumah_susun.keluarga kel,
	rumah_susun.identitas ident
WHERE
	rsn.status = 1 and
	kel.id_kepala_keluarga = 0 and
	kel.id_identitas = ident.id_identitas and
	kel.status = 1 and
	ident.status = 1
;

-- Get Anggota Keluarga
SELECT
	kel.id_keluarga,
	kel.id_kepala_keluarga,
	ident.nomor_ktp,
	ident.nama,
	ident.tempat_lahir,
	ident.tanggal_lahir,
	ident.agama,
	ident.nomor_hp,
	ident.foto,
	kel.finger_id,
	kel.rfid
FROM
	rumah_susun.keluarga kel,
	rumah_susun.identitas ident
WHERE
	kel.id_kepala_keluarga =
		(select id_kepala_keluarga from rumah_susun.rusun
		 where nomor_kamar = 1) and
	kel.id_identitas = ident.id_identitas and
	kel.status = 1 and
	ident.status = 1
;

-- ------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------
-- Membuat Store Procedure --
DELIMITER $$
USE `rumah_susun`$$
DROP PROCEDURE IF EXISTS `tambah_admin`$$
CREATE PROCEDURE `tambah_admin`(
	in v_username varchar(45),	-- wajib
	in v_password varchar(45),	-- wajib
	in v_RFID varchar(45),			-- wajib
	in v_user int(5),						-- wajib
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
			`Berhasil tambah admin dengan username : ` + v_username + ` ..`
			, now(), v_user
		);
		set v_result = `Berhasil tambah admin dengan username : ` + v_username + ` ..`;
	else
		insert into rumah_susun.log_history (
			action, create_date, create_by
		) value (
			`Gagal tambah admin dengan username : ` + v_username + ` ,, username sudah ada ..`
			, now(), v_user
		);
		set v_result = `Gagal tambah admin dengan username : ` + v_username + ` ,, username sudah ada ..`;
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
	in v_username varchar(45),	-- wajib
	in v_password varchar(45),	-- wajib
	in v_RFID varchar(45),			-- wajib
	in v_user int(5),						-- wajib
	out v_result varchar(100)
)
BEGIN
  set @validation = 0;
  select count(*) into @validation from rumah_susun.admin where status = 1 and (user_name = v_username or rfid = v_RFID);
	if @validation = 0 then
		update rumah_susun.admin set
			password = v_password,
			rfid = v_RFID
		where
			status = 1 and
			id_user = v_user
		;

		insert into rumah_susun.log_history (
			action, create_date, create_by
		) value (
			`Berhasil edit admin dengan username : ` + v_username + ` ..`
			, now(), v_user
		);
		set v_result = `Berhasil edit admin dengan username : ` + v_username + ` ..`;
	else
		insert into rumah_susun.log_history (
			action, create_date, create_by
		) value (
			`Gagal edit admin dengan username : ` + v_username + ` ,, RFID Sudah Terdaftar ..`
			, now(), v_user
		);
		set v_result = `Gagal edit admin dengan username : ` + v_username + ` ,, RFID Sudah Terdaftar ..`;
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
	in v_id_user int(5),			-- wajib
	in v_user int(5),					-- wajib
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
				`Berhasil menonaktifkan admin dengan username : ` + @v_username + ` ..`
				, now(), v_user
			);
			set v_result = `Berhasil menonaktifkan admin dengan username : ` + @v_username + ` ..`;
		else
			insert into rumah_susun.log_history (
				action, create_date, create_by
			) value (
				`Gagal menonaktifkan admin ,, admin dengan id ` + v_id_user + ` tidak ditemukan ..`
				, now(), v_user
			);
			set v_result = `Gagal menonaktifkan admin ,, admin dengan id ` + v_id_user + ` tidak ditemukan ..`;
		end if;
	else
		insert into rumah_susun.log_history (
			action, create_date, create_by
		) value (
			`Gagal menonaktifkan admin ,, tidak boleh menonaktifkan admin sendiri ..`
			, now(), v_user
		);
		set v_result = `Gagal menonaktifkan admin ,, tidak boleh menonaktifkan admin sendiri ..`;
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
	in v_nomor_kamar varchar(45),		-- wajib
	in v_user int(5),								-- wajib
	out v_result varchar(100)
)
BEGIN
  set @validation = 0;
  select count(*) into @validation from rumah_susun.rusun where nomor_kamar = v_nomor_kamar;
	if @validation > 0 then
		insert into rumah_susun.log_history (
			action, create_date, create_by
		) value (
			`Nomor Kamar : ` + v_name + ` Sudah ada ..`
			, now(), v_user
		);
		set v_result = `Nomor Kamar : ` + v_name + ` Sudah ada ..`;
	else
		insert into rumah_susun.rusun (
			nomor_kamar, create_date, update_date, create_by, update_by, status
		) value (
			v_nomor_kamar, now(), now(), v_user, v_user, 0
		);

		insert into rumah_susun.log_history (
			action, create_date, create_by
		) value (
			`Berhasil Tambah Kamar, Nomor Kamar : ` + v_name + ` ..`
			, now(), v_user
		);
		set v_result = `Berhasil Tambah Kamar, Nomor Kamar : ` + v_name + ` ..`;
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
	in v_nomor_ktp varchar(45),				-- wajib
	in v_nama varchar(45),						-- wajib
	in v_tempat_lahir varchar(45),		-- wajib
	in v_tanggal_lahir varchar(45),		-- wajib
	in v_agama varchar(45),						-- wajib
	in v_nomor_hp varchar(45),				-- wajib
	in v_foto varchar(45),						-- wajib
	in v_user int(5),									-- wajib
	in v_fingerID int(5),							-- wajib
	in v_RFID varchar(45),						-- wajib
	in v_nomor_kamar int(1),					-- wajib
	out v_result varchar(100)
)
BEGIN
  set @validation = 0;
  select count(*) into @validation from rumah_susun.rusun where nomor_kamar = v_nomor_kamar and status = 1;
	if @validation = 0 then
		-- 1. Insert Identitas Kepala Keluarga --
		insert into rumah_susun.identitas (
			nomor_ktp, nama, tempat_lahir, tanggal_lahir, agama, nomor_hp, foto, create_date, update_date, create_by, update_by, status
		) value (
			v_nomor_ktp, v_nama, v_tempat_lahir, v_tanggal_lahir, v_agama, v_nomor_hp, v_foto, now(), now(), v_user, v_user, 1
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
			`Berhasil Tambah Kepala Keluarga, nama : ` + v_name + `, ` +
			`Nomor KTP : ` + v_nomor_ktp + `, ` +
			`Di Kamar : ` + v_nomor_kamar + ` ..`
			, now(), v_user
		);
		set v_result = `Berhasil Tambah Kepala Keluarga, nama : ` + v_name + `, ` + `Nomor KTP : ` + v_nomor_ktp + `, ` + `Di Kamar : ` + v_nomor_kamar + ` ..`;
	else
		insert into rumah_susun.log_history (
			action, create_date, create_by
		) value (
			`Gagal Tambah Kepala Keluarga, nama : ` + v_name + `, ` +
			`Nomor KTP : ` + v_nomor_ktp + `, ` +
			`Di Kamar : ` + v_nomor_kamar + ` ,, Kamar Sudah Terisi ..`
			, now(), v_user
		);
		set v_result = `Gagal Tambah Kepala Keluarga, nama : ` + v_name + `, ` + `Nomor KTP : ` + v_nomor_ktp + `, ` + `Di Kamar : ` + v_nomor_kamar + ` ,, Kamar Sudah Terisi ..`;
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
	in v_nomor_ktp varchar(45),
	in v_nama varchar(45),
	in v_tempat_lahir varchar(45),
	in v_tanggal_lahir varchar(45),
	in v_agama varchar(45),
	in v_nomor_hp varchar(45),
	in v_foto varchar(45),
	in v_user int(5),
	in v_fingerID int(5),
	in v_RFID varchar(45),
	in v_nomor_kamar int(1),
	out v_result varchar(100)
)
BEGIN
  set @validation = 0;
  select count(*) into @validation from rumah_susun.rusun where nomor_kamar = v_nomor_kamar and status = 1;
	if @validation > 0 then
		-- 4. Insert Anggota Keluarga ke Table Identitas --
		insert into rumah_susun.identitas (
			nomor_ktp, nama, tempat_lahir, tanggal_lahir, agama, nomor_hp, foto, create_date, update_date, create_by, update_by, status
		) value (
			v_nomor_ktp, v_nama, v_tempat_lahir, v_tanggal_lahir, v_agama, v_nomor_hp, v_foto, now(), now(), v_user, v_user, 1
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
			`Berhasil Tambah Anggota Keluarga, nama : ` + v_name + `, ` +
			`Nomor KTP : ` + v_nomor_ktp + `, ` +
			`Di Kamar : ` + v_nomor_kamar + ` ..`
			, now(), v_user
		);
		set v_result = `Berhasil Tambah Anggota Keluarga, nama : ` + v_name + `, ` + `Nomor KTP : ` + v_nomor_ktp + `, ` + `Di Kamar : ` + v_nomor_kamar + ` ..`;
	else
		insert into rumah_susun.log_history (
			action, create_date, create_by
		) value (
			`Gagal Tambah Anggota Keluarga, nama : ` + v_name + `, ` +
			`Nomor KTP : ` + v_nomor_ktp + `, ` +
			`Di Kamar : ` + v_nomor_kamar + ` ,, Kamar Belum Memiliki Kepala Keluarga ..`
			, now(), v_user
		);
		set v_result = `Gagal Tambah Anggota Keluarga, nama : ` + v_name + `, ` + `Nomor KTP : ` + v_nomor_ktp + `, ` + `Di Kamar : ` + v_nomor_kamar + ` ,, Kamar Belum Memiliki Kepala Keluarga ..`;
	end if;
END$$
DELIMITER ;
-- ------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------
DELIMITER $$
USE `rumah_susun`$$
DROP PROCEDURE IF EXISTS `update_anggota_keluarga`$$
CREATE PROCEDURE `update_anggota_keluarga`(
	in v_nomor_identitas int(5),
	in v_nomor_ktp varchar(45),
	in v_nama varchar(45),
	in v_tempat_lahir varchar(45),
	in v_tanggal_lahir varchar(45),
	in v_agama varchar(45),
	in v_nomor_hp varchar(45),
	in v_foto varchar(45),
	in v_user int(5),
	in v_fingerID int(5),
	in v_RFID varchar(45),
	in v_nomor_kamar int(1),
	out v_result varchar(100)
)
BEGIN
  set @validation = 0;
  select count(*) into @validation from rumah_susun.identitas ident, rumah_susun.keluarga kel, rumah_susun.rusun rsn
	where ident.status = 1 and kel.status = 1 and rsn.status = 1 and ident.id_identitas = v_nomor_identitas;
	if @validation > 0 then
		update rumah_susun.identitas set
			nomor_ktp = v_nomor_ktp,
			nama = v_nama,
			tempat_lahir = v_tempat_lahir,
			tanggal_lahir = v_tanggal_lahir,
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
			`Berhasil Update Data Anggota Keluarga, nama : ` + v_name + `, ` +
			`Nomor KTP : ` + v_nomor_ktp + `, ` +
			`Di Kamar : ` + v_nomor_kamar + ` ..`
			, now(), v_user
		);
		set v_result = `Berhasil Update Data Anggota Keluarga, nama : ` + v_name + `, ` + `Nomor KTP : ` + v_nomor_ktp + `, ` + `Di Kamar : ` + v_nomor_kamar + ` ..`;
	else
		insert into rumah_susun.log_history (
			action, create_date, create_by
		) value (
			`Gagal Update Data Anggota Keluarga, nama : ` + v_name + `, ` +
			`Nomor KTP : ` + v_nomor_ktp + `, ` +
			`Di Kamar : ` + v_nomor_kamar + ` ,, Anggota Keluarga dengan id ` + v_nomor_identitas + ` sudah tidak aktif lagi ..`
			, now(), v_user
		);
		set v_result = `Gagal Update Data Anggota Keluarga, nama : ` + v_name + `, ` + `Nomor KTP : ` + v_nomor_ktp + `, ` +
		`Di Kamar : ` + v_nomor_kamar + ` ,, Anggota Keluarga dengan id ` + v_nomor_identitas + ` sudah tidak aktif lagi ..`;
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
	in v_nomor_identitas int(5),	-- wajib
	in v_user int(5),							-- wajib
	out v_result varchar(100)
)
BEGIN
  set @v_nama = ''; set @v_id_kepala_keluarga = 0; set @v_nomor_kamar = 0;
	select nama into @v_nama from rumah_susun.identitas where id_identitas = v_nomor_identitas;
	select id_kepala_keluarga into @v_id_kepala_keluarga from rumah_susun.keluarga where id_identitas = v_nomor_identitas;
	select rsn.nomor_kamar into @v_nomor_kamar
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
			`Anggota Keluarga, nama : ` + @v_name + `, ` +
			`Di Kamar : ` + @v_nomor_kamar + ` Dinonaktifkan ..`
			, now(), v_user
		);
	else
		update rumah_susun.identitas set status = 0
		where id_identitas = v_nomor_identitas;
		update rumah_susun.keluarga set status = 0
		where id_identitas in (select id_identitas from rumah_susun.keluarga
													 where id_kepala_keluarga = (select id_keluarga from rumah_susun.keluarga
													 														 where id_identitas = v_nomor_identitas)
													);

		insert into rumah_susun.log_history (
			action, create_date, create_by
		) value (
			`Kepala Keluarga, nama : ` + @v_name + `, ` +
			`Di Kamar : ` + @v_nomor_kamar + ` Beserta Aanggota Keluarga Dinonaktifkan ..`
			, now(), v_user
		);
	end if;
END$$
DELIMITER ;
-- ------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------
DELIMITER $$
USE `rumah_susun`$$
DROP PROCEDURE IF EXISTS `delete_non_aktif_data`$$
CREATE PROCEDURE `delete_non_aktif_data`()
BEGIN
	delete from rumah_susun.admin where status = 0;
	delete from rumah_susun.identitas where status = 0;
	delete from rumah_susun.keluarga where status = 0;

	insert into rumah_susun.log_history (
		action, create_date, create_by
	) value (
		`Hapus Data Yang Berstatus Tidak Aktif ..`
		, now(), v_user
	);
END$$
DELIMITER ;
-- ------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------

select * from rumah_susun.log_history;
-- ------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------
-- Coba procedure
call tambah_admin('admin_1', 'admin_1', '', 1, @result);
select @result;

call non_aktifkan_admin(2, 1, @result);
select @result;

call edit_admin('admin_1', 'admin_2', 'ABCDE', 2, @result);
select @result;

call delete_non_aktif_data(3, @result);
select @result;

call insert_kamar_baru(2, 2, @result);
select @result;

call insert_kepala_keluarga_baru(
	'456456456456', -- '123123123123', -- 
	'Kepala Keluarga 2',
	'Jakarta',
	'08-01-1980',
	'Islam',
	'+6281234567890',
	'', -- Path Foto
	2,  -- User ID
	1,	-- Finger ID
	'',	-- RFID
	2,	-- Nomor Kamar
	@result
);
select @result;

-- select date_format(str_to_date('08-01-1980', '%d-%m-%Y'), '%d-%m-%Y') from dual;

call insert_anggota_keluarga_baru(
	'321321321321',
	'Anggota Keluarga 1',
	'Jakarta',
	'08-01-1990',
	'Islam',
	'+6281234567890',
	'', -- Path Foto
	2,  -- User ID
	1,	-- Finger ID
	'',	-- RFID
	1,	-- Nomor Kamar
	@result
);
select @result;

call update_identitas_keluarga(
	2, -- in v_nomor_identitas int(5),     -- wajib
	'321321321321', -- in v_nomor_ktp varchar(45),      -- wajib
	'Anggota Keluarga 123', -- in v_nama varchar(45),           -- wajib
	'Bandung', -- in v_tempat_lahir varchar(45),   -- wajib
	'01-01-2017', -- in v_tanggal_lahir varchar(45),  -- wajib  format: dd-mm-YYYY
	'Kristen', -- in v_agama varchar(45),          -- wajib
	'089636967339', -- in v_nomor_hp varchar(45),       -- wajib
	'', -- in v_foto varchar(45),           -- wajib
	2, -- in v_user int(5),                -- wajib
	2, -- in v_fingerID int(5),            -- wajib
	'CDEFCDEF', -- in v_RFID varchar(45),           -- wajib
	1, -- in v_nomor_kamar int(1),         -- wajib
	@result
);
select @result;

call non_aktifkan_anggota_keluarga(
	1, -- in v_nomor_identitas int(5), -- wajib
	2, -- in v_user int(5),            -- wajib
	@result
);
select @result;
-- ------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------
-- procedure tambahan
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
call get_data_rusun_kepala();