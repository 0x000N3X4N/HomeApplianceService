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
CREATE TABLE street (PK_street_id INT PRIMARY KEY IDENTITY, FK_city_id INT REFERENCES city(PK_city_id), [name] NVARCHAR(50) NOT NULL, 
					 house_num SMALLINT NOT NULL, porch SMALLINT NOT NULL, [floor] SMALLINT NOT NULL,
					 CONSTRAINT [CK_StreetIsValid] CHECK ([name] <> N'' AND
														  [house_num] <> N'' AND
														  [porch] <> N'' AND
														  [floor] <> N'')
					);
CREATE TABLE customers (
						 PK_customer_id INT PRIMARY KEY IDENTITY, FK_street_id INT REFERENCES street(PK_street_id), [name] NVARCHAR(30) NOT NULL, phone NVARCHAR(12) NOT NULL,
						 CONSTRAINT [CK_CustIsValid] CHECK ([name] <> N'' AND
															[phone] <> N'')
					   );
CREATE TABLE orders (
						PK_order_id INT PRIMARY KEY IDENTITY, FK_component_id INT REFERENCES components(PK_component_id)
						ON DELETE CASCADE, FK_employee_id INT REFERENCES employees(PK_employee_id) ON DELETE CASCADE, 
						FK_customer_id INT REFERENCES customers(PK_customer_id) ON DELETE CASCADE, acceptance_date DATE NOT NULL DEFAULT(GETDATE()),
						comp_count INT NOT NULL, price MONEY NOT NULl
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
VALUES ('1', 'Dell SE2419H', 'Разрешение\1920x1080;' +
							 'Соотношение сторон\16:9;' + 
							 'Диагональ\23.8";' + 
							 'Расширенный динамический диапазон (HDR)\false;' + 
							 'Поверхность экрана\матовая;' + 
							 'Сенсорный экран\false;' + 
							 'Улучшенная глубина цвета\false;' + 
							 'NVIDIA G-Sync\false;' +
							 'AMD FreeSync\false;' +
							 'Яркость\250 кд/м2;' +
							 'Контрастность\1 000 :1;' +
							 'Время отклика (GtG)\5 мс;' +
							 'Угол обзора по горизонтали\178 °;' +
							 'Угол обзора по вертикали\178 °', '251.25', '2018'),

		('1', 'LG 24MK600M-B', 'Диагональ\23.8"' + ';' +
							    'Соотношение сторон\16:9' + ';' +
								'Разрешение\1920x1080' + ';' +
								'Расширенный динамический диапазон (HDR)\false' + ';' + 
								'Поверхность экрана\матовая' + ';' + 
								'Сенсорный экран\false' + ';' +
								'Улучшенная глубина цвета\false' + ';' +
								'NVIDIA G-Sync\false' + ';' +
								'AMD FreeSync\true' + ';' +
								'Яркость\250 кд/м2' + ';' +
								'Контрастность\1 000 :1' + ';' +
								'Время отклика (GtG)\5 мс' + ';' +
								'Угол обзора по горизонтали\178 °' + ';' +
								'Угол обзора по вертикали\178 °', '308.88', '2018'),

		('2', 'Gigabyte Aorus GeForce RTX 2060 Super 8GB GDDR6', 'Интерфейс\PCI Express x16 3.0' + ';' +
																 'Производитель графического процессора\NVIDIA' + ';' +
																 'Графический процессор\GeForce RTX 2060 Super' + ';' +
																 '«Разогнанная» версия\true' + ';' +
																 'Частота графического процессора\1 470 МГц' + ';' + 
																 'Turbo-частота графического процессора\1 845 МГц' + ';' +
																 'Количество потоковых процессоров\2 176' + ';' +
																 'Видеопамять\8 ГБ' + ';' +
																 'Тип видеопамяти\GDDR6' + ';' +
																 'Частота памяти\3 500 МГц' + ';' + 
																 'Ширина шины памяти\256 бит' + ';' + 
																 'Поддержка DirectX\12' + ';' +
																 'Поддержка SLI/CrossFire\false' + ';' +
																 'Разъёмы питания\6+8 pin' + ';' +
																 'Рекомендуемый блок питания\550 Вт' + ';' +
																 'Охлаждение\активное' + ';' +
																 'Толщина системы охлаждения\2.7 слота' + ';' +
																 'Количество вентиляторов\3' + ';' +
																 'Длина видеокарты\290 мм' + ';' + 
																 'Высота видеокарты\134.31 мм' + ';' +
																 'Низкопрофильная (Low Profile)\false', '1050,62', '2019'),
		('3', 'AMD Ryzen 5 2600', 'Модельный ряд\Ryzen 5'                                    + ';' +
																 'Тип поставки\OEM'                                         + ';' +
																 'Охлаждение в комплекте\false'                             + ';' +
																 'Кодовое название кристалла\Pinnacle Ridge'                + ';' +
																 'Сокет\AM4'                                                + ';' +
																 'Количество ядер\6'                                        + ';' +
																 'Максимальное количество потоков\12'                       + ';' +
																 'Тактовая частота\3.4 ГГц'                                 + ';' +
																 'Максимальная Turbo-частота\3.9 ГГц'                       + ';' +
																 'Кэш L2\3 МБ'                                              + ';' +
																 'Кэш L3\16 МБ'                                             + ';' +
																 'Поддержка памяти\DDR4'                                    + ';' +
																 'Количество каналов памяти\2'                              + ';' +
																 'Макс. частота памяти\2 933 МГц'                           + ';' +
																 'Встроенный контроллер PCI Express\PCI Express 3.0 x16'    + ';' +
																 'Встроенная графика\false'                                 + ';' +
																 'Расчетная тепловая мощность (TDP) 65 Вт'                  + ';' +
																 'Толщина транзистора\12 нм'                                + ';' +
																 'Многопоточность ядра\true'                                + ';' +
																 'Виртуализация AMD-V\true', '260.82', '2018'),
		('4', 'Intel Core i5-9400F', 'Модельный ряд\Core i5'                                                          + ';' +
																'Тип поставки\OEM'                                                               + ';' +
																'Охлаждение в комплекте\false'                                                   + ';' +
																'Кодовое название кристалла\Coffee Lake'                                         + ';' +
																'Сокет\LGA1151 v2'                                                               + ';' +
																'Количество ядер\6'                                                              + ';' +
																'Максимальное количество потоков\6'                                              + ';' +
																'Тактовая частота\2.9 ГГц'                                                       + ';' +
																'Максимальная Turbo-частота\4.1 ГГц'                                             + ';' +
																'Кэш L3\9 МБ'                                                                    + ';' +
																'Поддержка памяти\DDR4'                                                          + ';' +
																'Количество каналов памяти\2'                                                    + ';' +
																'Макс. частота памяти\2 666 МГц'                                                + ';' +
																'Встроенный контроллер PCI Express\PCI Express 3.0 x16/x8+x8/x8+x4+x4'           + ';' +
																'Встроенная графика\false'                                                       + ';' +
																'Расчетная тепловая мощность (TDP)\65 Вт'                                        + ';' +
																'Толщина транзистора\14 нм'                                                      + ';' +
																'Многопоточность ядра\false'                                                     + ';' +
																'Виртуализация Intel VT-x\true'                                                  + ';' +
																'Виртуализация Intel VT-d\true', '310.29', '2019'),
		('5', 'AMD Ryzen 5 3600', 'Модельный ряд\Ryzen 5'                                              + ';' +
																'Тип поставки\OEM'                                                   + ';' +
																'Охлаждение в комплекте\false'                                       + ';' +
																'Кодовое название кристалла\Matisse'                                 + ';' +
																'Сокет\AM4'                                                          + ';' +
																'Количество ядер\6'                                                  + ';' +
																'Максимальное количество потоков\12'                                 + ';' +
																'Тактовая частота\3.6 ГГц'                                           + ';' +
																'Максимальная Turbo-частота\4.2 ГГц'                                 + ';' +
																'Кэш L2\3 МБ'                                                        + ';' +
																'Кэш L3\32 МБ'                                                       + ';' +
																'Поддержка памяти\DDR4'                                              + ';' +
																'Количество каналов памяти\2'                                        + ';' +
																'Макс. частота памяти\3 200 МГц'                                     + ';' +
																'Встроенный контроллер PCI Express\PCI Express 4.0 x16'              + ';' +
																'Встроенная графика\false'                                           + ';' +
																'Расчетная тепловая мощность (TDP)\65 Вт'                            + ';' +
																'Толщина транзистора\7 нм'                                           + ';' +
																'Многопоточность ядра\true ', '411.72', '2019');


INSERT INTO employees
VALUES ('John Track Lir', 'Accountant', 500, 1),
	   ('Stephan King Sir', 'Accountant', 1000, 2);


INSERT INTO city
VALUES ('Минск'),
	   ('Гомель'),
	   ('Могилёв'),
	   ('Витебск'),
	   ('Гродно'),
	   ('Брест');


INSERT INTO street
VALUES ('1', 'Автозоводская', '1337', '2', '3'),
	   ('6', 'Матусевича', '123', '4', '3');


INSERT INTO customers
VALUES ('1', 'Jeff', '+1234567'),
       ('2', 'G.A.', '+4522156');


INSERT INTO orders
VALUES ('1', '2', '2', '2001-04-04', '4', 1022.3);


SELECT * FROM components;
SELECT * FROM components_type;
SELECT * FROM employees;
SELECT * FROM city;
SELECT * FROM street;
SELECT * FROM customers;
SELECT * FROM orders;
--SELECT C.title AS 'Title', O.comp_count AS 'Count of components', O.acceptance_date AS 'Acceptance date', O.price AS 'Price'
--FROM orders O
--JOIN components C
--ON O.FK_component_id = C.PK_component_id;

--SELECT C.title AS 'Title', E.fullname AS 'Employee ', 
--                                  O.acceptance_date AS 'Acceptance date', O.price AS 'Price' 
--                                  FROM orders O 
--                                  JOIN components C 
--                                  ON O.FK_component_id = C.PK_component_id 
--                                  JOIN employees E 
--                                  ON o.FK_employee_id = e.PK_employee_id;

DELETE FROM orders
WHERE (
	    FK_component_id = (SELECT C.PK_component_id
			               FROM components C 
						   WHERE C.title = 'AMD Ryzen 5 3600') AND
		FK_employee_id =  (SELECT E.PK_employee_id
						   FROM employees E
						   WHERE E.fullname = 'Stephan King Sir') AND
	    price = 1646.88 AND
		acceptance_date = '2019-12-15')

GO
CREATE PROC [stats]
@from_date DATE, @to_date DATE
AS
BEGIN
	SELECT C.title AS 'Title', O.comp_count AS 'Count of components', O.acceptance_date AS 'Acceptance date', O.price AS 'Price'
	FROM orders O
	JOIN components C
	ON O.FK_component_id = C.PK_component_id
	WHERE O.acceptance_date
	BETWEEN @from_date AND @to_date;
END

GO
EXEC [stats] '2000-04-01','2001-04-05'

--INSERT INTO passport
--VALUES ('John Track Lir', 'Kolesnikova 5', 'Frunzenskoe RUVD', 123);

		--TODO: Entities city, street
--*********************************************************
--/\/\/\/\/\/\/\/\/\/ END OF FILL TABLES /\/\/\/\/\/\/\/\/\
--*********************************************************