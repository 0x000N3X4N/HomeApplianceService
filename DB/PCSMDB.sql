USE master;
IF DB_ID(N'PCSMDB') IS NOT NULL
BEGIN
	ALTER DATABASE PCSMDB 
	SET SINGLE_USER 
	WITH ROLLBACK IMMEDIATE;
	DROP DATABASE PCSMDB;
END

GO
CREATE DATABASE PCSMDB;
GO
USE PCSMDB;

--/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
--/\/\/\/\/\/\/\/\/\/ DEFINE TABLES /\/\/\/\/\/\/\/\/\/\/\/\/\/\
--/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
CREATE TABLE components_type (PK_component_type_id INT PRIMARY KEY IDENTITY, component_type VARCHAR(70) NOT NULL UNIQUE,
							  CONSTRAINT [CK_Comp_tyIsValid] CHECK ([component_type] <> N'')
							 );
CREATE TABLE components (
						  PK_component_id INT PRIMARY KEY IDENTITY, FK_type_code INT REFERENCES components_type(PK_component_type_id)
						  ON DELETE CASCADE, title VARCHAR(256) NOT NULL, specifications VARCHAR(MAX) NOT NULL, price MONEY NOT NULL,
						  release_date INT NOT NULL, 
						  CONSTRAINT [CK_CompIsValid] CHECK ([title] <> N'' AND 
															 [specifications] <> N'' AND
															 [price] <> N'' AND
															 [release_date] <> N'')
						);
CREATE TABLE employees (
						 PK_employee_id INT PRIMARY KEY IDENTITY, fullname NVARCHAR(70) NOT NULL, post NVARCHAR(70) NOT NULL,
						 salary MONEY NULL, working_hours INT NULL DEFAULT(8),
						 CONSTRAINT [CK_EmpIsValid] CHECK ([fullname] <> N'' AND 
														   [post] <> N'')
					   );
CREATE TABLE passport (
					    PK_passport_id INT PRIMARY KEY IDENTITY, fullname NVARCHAR(70) NOT NULL, place_of_residence NVARCHAR(70) NOT NULL,
						body_which_issued_doc NVARCHAR (256) NOT NULL, series NVARCHAR(2) DEFAULT('MP') NOT NULL, pass_num INT NOT NULL,
						CONSTRAINT [CK_PassIsValid] CHECK ([fullname] <> N'' AND 
														   [place_of_residence] <> N'' AND
														   [body_which_issued_doc] <> N'' AND
														   [series] <> N'' AND
														   [pass_num] <> N'')
					  );
CREATE TABLE city (PK_city_id INT PRIMARY KEY IDENTITY, [name] NVARCHAR(256) NOT NULL,
				   CONSTRAINT [CK_CityIsValid] CHECK ([name] <> N'')
				   );
CREATE TABLE street (PK_street_id INT PRIMARY KEY IDENTITY, FK_city_id INT REFERENCES city(PK_city_id), house_num SMALLINT NOT NULL, porch SMALLINT NOT NULL, [floor] SMALLINT NOT NULL,
					 CONSTRAINT [CK_StreetIsValid] CHECK ([house_num] <> N'' AND
														  [porch] <> N'' AND
														  [floor] <> N'')
					);
CREATE TABLE customers (
						 PK_customer_id INT PRIMARY KEY IDENTITY, FK_city_id INT REFERENCES city(PK_city_id), [name] NVARCHAR(30) NOT NULL, phone NVARCHAR(12) NOT NULL,
						 CONSTRAINT [CK_CustIsValid] CHECK ([name] <> N'' AND
															[phone] <> N'')
					   );
CREATE TABLE orders (
						PK_order_id INT PRIMARY KEY IDENTITY, FK_component_type INT REFERENCES components(PK_component_id)
						ON DELETE CASCADE, FK_employee_id INT REFERENCES employees(PK_employee_id) ON DELETE CASCADE, 
						FK_customer_id INT REFERENCES customers(PK_customer_id) ON DELETE CASCADE, acceptance_date DATE NOT NULL DEFAULT(GETDATE()),
						price MONEY NOT NULL, comp_count INT NOT NULL
					);

--/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
--/\/\/\/\/\/\/\/\/\/ END OF DEFINE TABLE /\/\/\/\/\/\/\/\/\/\/
--/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\

--*********************************************************
--/\/\/\/\/\/\/\/\/\/\ FILL TABLES /\/\/\/\/\/\/\/\/\/\/\/\
--*********************************************************
INSERT INTO components_type
VALUES ('Monitor'	   ),
	   ('GPU'		   ),
	   ('CPU'		   ),
	   ('Power supply' ),
	   ('Metherboard'  ),
	   ('Water cooling'),
	   ('Cooling'	   ),
	   ('RAM'		   );
SELECT * FROM components_type;
INSERT INTO components
VALUES ('1', 'Dell SE2419H', 'Разрешение\1920x1080' + CHAR(0xA) +
							 'Соотношение сторон\16:9' + CHAR(0xA) +
							 'Диагональ\23.8"' + CHAR(0xA) +
							 'Расширенный динамический диапазон (HDR)\false' + CHAR(0xA) + 
							 'Поверхность экрана\матовая' + CHAR(0xA) + 
							 'Сенсорный экран\false' + CHAR(0xA) + 
							 'Улучшенная глубина цвета\false' + CHAR(0xA) + 
							 'NVIDIA G-Sync\false' + CHAR(0xA) +
							 'AMD FreeSync\false' + CHAR(0xA) +
							 'Яркость\250 кд/м2' + CHAR(0xA) +
							 'Контрастность\1 000 :1' + CHAR(0xA) +
							 'Время отклика (GtG)\5 мс' + CHAR(0xA) +
							 'Угол обзора по горизонтали\178 °' + CHAR(0xA) +
							 'Угол обзора по вертикали\178 °', '251.25', '2018'),

		('1', 'LG 24MK600M-B', 'Диагональ\23.8"' + CHAR(0xA) +
							    'Соотношение сторон\16:9 ' + CHAR(0xA) +
								'Разрешение\1920x1080' + CHAR(0xA) +
								'Тип\домашний/офисный' + CHAR(0xA) +
								'Матрица\IPS' + CHAR(0xA) +
								'Расширенный динамический диапазон (HDR)\false' + CHAR(0xA) + 
								'Поверхность экрана\матовая' + CHAR(0xA) + 
								'Сенсорный экран\false' + CHAR(0xA) +
								'Улучшенная глубина цвета\false' + CHAR (0xA) +
								'NVIDIA G-Sync\false' + CHAR(0xA) +
								'AMD FreeSync\true' + CHAR(0xA) +
								'Яркость\250 кд/м2' + CHAR(0xA) +
								'Контрастность\1 000 :1' + CHAR(0xA) +
								'Время отклика (GtG)\5 мс' + CHAR(0xA) +
								'Угол обзора по горизонтали\178 °' + CHAR(0xA) +
								'Угол обзора по вертикали\178 °' + CHAR(0xA), '308.88', '2018'),

		('2', 'Gigabyte Aorus GeForce RTX 2060 Super 8GB GDDR6', 'Интерфейс\PCI Express x16 3.0' + CHAR(0xA) +
																 'Производитель графического процессора\NVIDIA' + CHAR(0xA) +
																 'Графический процессор\GeForce RTX 2060 Super' + CHAR(0xA) +
																 '«Разогнанная» версия\true' + CHAR(0xA) +
																 'Частота графического процессора\1 470 МГц' + CHAR(0xA) + 
																 'Turbo-частота графического процессора\1 845 МГц' + CHAR(0xA) +
																 'Количество потоковых процессоров\2 176' + CHAR(0xA) +
																 'Видеопамять\8 ГБ' + CHAR(0xA) +
																 'Тип видеопамяти\GDDR6' + CHAR(0xA) +
																 'Частота памяти\3 500 МГц' + CHAR(0xA) + 
																 'Ширина шины памяти\256 бит' + CHAR(0xA) + 
																 'Поддержка DirectX\12' + CHAR(0xA) +
																 'Поддержка SLI/CrossFire\false' + CHAR(0xA) +
																 'Разъёмы питания\6+8 pin' + CHAR(0xA) +
																 'Рекомендуемый блок питания\550 Вт' + CHAR(0xA) +
																 'Охлаждение\активное' + CHAR(0xA) +
																 'Толщина системы охлаждения\2.7 слота' + CHAR(0xA) +
																 'Количество вентиляторов\3' + CHAR(0xA) +
																 'Длина видеокарты\290 мм' + CHAR(0xA) + 
																 'Высота видеокарты\134.31 мм' + CHAR(0xA) +
																 'Низкопрофильная (Low Profile)\false' + CHAR(0xA), '1050,62', '2019'),
		('3', 'AMD Ryzen 5 2600', 'Модельный ряд\Ryzen 5'                                    + CHAR(0xA) +
																 'Тип поставки\OEM'                                         + CHAR(0xA) +
																 'Охлаждение в комплекте\false'                             + CHAR(0xA) +
																 'Кодовое название кристалла\Pinnacle Ridge'                + CHAR(0xA) +
																 'Сокет\AM4'                                                + CHAR(0xA) +
																 'Количество ядер\6'                                        + CHAR(0xA) +
																 'Максимальное количество потоков\12'                       + CHAR(0xA) +
																 'Тактовая частота\3.4 ГГц'                                 + CHAR(0xA) +
																 'Максимальная Turbo-частота\3.9 ГГц'                       + CHAR(0xA) +
																 'Кэш L2\3 МБ'                                              + CHAR(0xA) +
																 'Кэш L3\16 МБ'                                             + CHAR(0xA) +
																 'Поддержка памяти\DDR4'                                    + CHAR(0xA) +
																 'Количество каналов памяти\2'                              + CHAR(0xA) +
																 'Макс. частота памяти\2 933 МГц'                           + CHAR(0xA) +
																 'Встроенный контроллер PCI Express\PCI Express 3.0 x16'    + CHAR(0xA) +
																 'Встроенная графика\false'                                 + CHAR(0xA) +
																 'Расчетная тепловая мощность (TDP) 65 Вт'                  + CHAR(0xA) +
																 'Толщина транзистора\12 нм'                                + CHAR(0xA) +
																 'Многопоточность ядра\true'                                + CHAR(0xA) +
																 'Виртуализация AMD-V\true'                                 + CHAR(0xA), '260.82', '2018'),
		('4', 'Intel Core i5-9400F', 'Модельный ряд\Core i5'                                                          + CHAR(0xA) +
																'Тип поставки\OEM'                                                               + CHAR(0xA) +
																'Охлаждение в комплекте\false'                                                   + CHAR(0xA) +
																'Кодовое название кристалла\Coffee Lake'                                         + CHAR(0xA) +
																'Сокет\LGA1151 v2'                                                               + CHAR(0xA) +
																'Количество ядер\6'                                                              + CHAR(0xA) +
																'Максимальное количество потоков\6'                                              + CHAR(0xA) +
																'Тактовая частота\2.9 ГГц'                                                       + CHAR(0xA) +
																'Максимальная Turbo-частота\4.1 ГГц'                                             + CHAR(0xA) +
																'Кэш L3\9 МБ'                                                                    + CHAR(0xA) +
																'Поддержка памяти\DDR4'                                                          + CHAR(0xA) +
																'Количество каналов памяти\2'                                                    + CHAR(0xA) +
																'Макс. частота памяти\2 666 МГц'                                                + CHAR(0xA) +
																'Встроенный контроллер PCI Express\PCI Express 3.0 x16/x8+x8/x8+x4+x4'           + CHAR(0xA) +
																'Встроенная графика\false'                                                       + CHAR(0xA) +
																'Расчетная тепловая мощность (TDP)\65 Вт'                                        + CHAR(0xA) +
																'Толщина транзистора\14 нм'                                                      + CHAR(0xA) +
																'Многопоточность ядра\false'                                                     + CHAR(0xA) +
																'Виртуализация Intel VT-x\true'                                                  + CHAR(0xA) +
																'Виртуализация Intel VT-d\true'                                                  + CHAR(0xA), '310.29', '2019'),
		('5', 'AMD Ryzen 5 3600', 'Модельный ряд\Ryzen 5'                                              + CHAR(0xA) +
																'Тип поставки\OEM'                                                   + CHAR(0xA) +
																'Охлаждение в комплекте\false'                                       + CHAR(0xA) +
																'Кодовое название кристалла\Matisse'                                 + CHAR(0xA) +
																'Сокет\AM4'                                                          + CHAR(0xA) +
																'Количество ядер\6'                                                  + CHAR(0xA) +
																'Максимальное количество потоков\12'                                 + CHAR(0xA) +
																'Тактовая частота\3.6 ГГц'                                           + CHAR(0xA) +
																'Максимальная Turbo-частота\4.2 ГГц'                                 + CHAR(0xA) +
																'Кэш L2\3 МБ'                                                        + CHAR(0xA) +
																'Кэш L3\32 МБ'                                                       + CHAR(0xA) +
																'Поддержка памяти\DDR4'                                              + CHAR(0xA) +
																'Количество каналов памяти\2'                                        + CHAR(0xA) +
																'Макс. частота памяти\3 200 МГц'                                     + CHAR(0xA) +
																'Встроенный контроллер PCI Express\PCI Express 4.0 x16'              + CHAR(0xA) +
																'Встроенная графика\false'                                           + CHAR(0xA) +
																'Расчетная тепловая мощность (TDP)\65 Вт'                            + CHAR(0xA) +
																'Толщина транзистора\7 нм'                                           + CHAR(0xA) +
																'Многопоточность ядра\true '                                         + CHAR(0xA), '411.72', '2019');

SELECT * FROM components;
SELECT * FROM components_type;
INSERT INTO employees
VALUES ('John Trac1k Lir', 'Accountant', 500, 1),
	   ('Stephan Ki1ng Sir', 'Accountant', 1000, 2);
SELECT * FROM employees;
--INSERT INTO passport
--VALUES ('John Track Lir', 'Kolesnikova 5', 'Frunzenskoe RUVD', 123);

		--TODO: Entities city, street
--*********************************************************
--/\/\/\/\/\/\/\/\/\/ END OF FILL TABLES /\/\/\/\/\/\/\/\/\
--*********************************************************