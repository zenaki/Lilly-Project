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
	kel.id_identitas = ident.id_identitas
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
	kel.id_identitas = ident.id_identitas
;

-- ------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------
-- Membuat Store Procedure --
DELIMITER $$
USE `rumah_susun`$$
CREATE PROCEDURE `insert_kamar_baru`(
	in v_nomor_kamar varchar(45),
	in v_user int(5)
)
BEGIN
	insert into rumah_susun.rusun (
		nomor_kamar, create_date, update_date, create_by, update_by, status
	) value (
		v_nomor_kamar, now(), now(), v_user, v_user, 0
	);

	insert into rumah_susun.log_history (
		action, create_date, create_by
	) value (
		'Tambah Kamar, Nomor Kamar : ' + v_name
		, now(), v_user
	);
END$$
DELIMITER ;
-- ------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------
DELIMITER $$
USE `rumah_susun`$$
CREATE PROCEDURE `insert_kepala_keluarga_baru`(
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
	in v_nomor_kamar int(1)
)
BEGIN
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
		v_nomor_ktp,
		v_nama,
		v_tempat_lahir,
		v_tanggal_lahir,
		v_agama,
		v_nomor_hp,
		v_foto,
		now(),
		now(),
		v_user,
		v_user,
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
		(select max(id_identitas) from rumah_susun.identitas),
		now(),
		now(),
		v_user,
		v_user,
		1,
		v_RFID,
		v_fingerID
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
		'Tambah Kepala Keluarga, nama : ' + v_name + ', ' +
		'Nomor KTP : ' + v_nomor_ktp + ', ' +
		'Di Kamar : ' + v_nomor_kamar
		, now(), v_user
	);
END$$
DELIMITER ;
-- ------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------
DELIMITER $$
USE `rumah_susun`$$
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
	in v_nomor_kamar int(1)
)
BEGIN
	-- 4. Insert Anggota Keluarga ke Table Identitas --
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
		v_nomor_ktp,
		v_nama,
		v_tempat_lahir,
		v_tanggal_lahir,
		v_agama,
		v_nomor_hp,
		v_foto,
		now(),
		now(),
		v_user,
		v_user,
		1
	);

	-- 5. Insert Anggota Keluarga ke Table Keluarga --
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
		(select max(id_identitas) from rumah_susun.identitas),
		(select id_kepala_keluarga from rumah_susun.rusun where nomor_kamar = v_nomor_kamar and status = 1),
		now(),
		now(),
		v_user,
		v_user,
		1,
		v_RFID,
		v_fingerID
	);

	insert into rumah_susun.log_history (
		action, create_date, create_by
	) value (
		'Tambah Anggota Keluarga, nama : ' + v_name + ', ' +
		'Nomor KTP : ' + v_nomor_ktp + ', ' +
		'Di Kamar : ' + v_nomor_kamar
		, now(), v_user
	);
END$$
DELIMITER ;
-- ------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------
DELIMITER $$
USE `rumah_susun`$$
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
	in v_nomor_kamar int(1)
)
BEGIN
	update rumah_susun.identitas set
		nomor_ktp = v_nomor_ktp,
		nama = v_nama,
		tempat_lahir = v_tempat_lahir,
		tanggal_lahir = v_tanggal_lahir,
		agama = v_agama,
		nomor_hp = v_nomor_hp,
		foto = v_foto,
		update_date = now(),
		update_by = v_user,
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
		'Update Data Anggota Keluarga, nama : ' + v_name + ', ' +
		'Nomor KTP : ' + v_nomor_ktp + ', ' +
		'Di Kamar : ' + v_nomor_kamar
		, now(), v_user
	);
END$$
DELIMITER ;
-- ------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------
DELIMITER $$
USE `rumah_susun`$$
CREATE PROCEDURE `delete_anggota_keluarga`(
	in v_nomor_identitas int(5),
	in v_user int(5)
)
BEGIN
	v_nama = select nama from rumah_susun.identitas where id_identitas = v_nomor_identitas;
	v_nomor_kamar = select rsn.nomor_kamar
									from rumah_susun.rusun rsn, rumah_susun.keluarga klg
									where rsn.id_kepala_keluarga = (select
																									case id_kepala_keluarga
																										when 0 then id_keluarga
																										else id_kepala_keluarga
																									end case
										 															from rumah_susun.keluarga where id_identitas = v_nomor_identitas);
	delete from rumah_susun.identitas where id_identitas in (
		select id_identitas
		from rumah_susun.keluarga
		where id_keluarga = (select id_keluarga)
	);
	delete from rumah_susun.keluarga where id_identitas = v_nomor_identitas;

	insert into rumah_susun.log_history (
		action, create_date, create_by
	) value (
		'Delete Anggota Keluarga, nama : ' + v_name + ', ' +
		'Di Kamar : ' + v_nomor_kamar
		, now(), v_user
	);
END$$
DELIMITER ;
-- ------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------
