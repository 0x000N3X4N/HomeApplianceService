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
CREATE TABLE components_type(PK_component_type_id INT PRIMARY KEY IDENTITY, product_type VARCHAR(70) NOT NULL);
CREATE TABLE components (
						  PK_component_id INT PRIMARY KEY IDENTITY, FK_type_code INT REFERENCES components_type(PK_component_type_id)
						  ON DELETE CASCADE, title VARCHAR(256) NOT NULL, tech_characteristics TEXT NOT NULL, price MONEY NOT NULL,
						  release_date INT NOT NULL
						);
CREATE TABLE employees (
						 PK_employee_id INT PRIMARY KEY IDENTITY, fullname NVARCHAR(70) NOT NULL, post NVARCHAR(70) NOT NULL,
						 working_hours INT NOT NULL DEFAULT(8), salary MONEY NULL
					   );
CREATE TABLE passport (
					    PK_passport_id INT PRIMARY KEY IDENTITY, fullname NVARCHAR(70) NOT NULL, place_of_residence NVARCHAR(70) NOT NULL,
						body_which_issued_doc NVARCHAR (256) NOT NULL, series NVARCHAR(2) DEFAULT('MP') NOT NULL, pass_num INT NOT NULL,
					  );
CREATE TABLE city (PK_city_id INT PRIMARY KEY IDENTITY, [name] NVARCHAR(256) NOT NULL);
CREATE TABLE street (PK_street_id INT PRIMARY KEY IDENTITY, FK_city_id INT REFERENCES city(PK_city_id), house_num SMALLINT NOT NULL, porch SMALLINT NOT NULL, [floor] SMALLINT NOT NULL);
CREATE TABLE customers (
						 PK_customer_id INT PRIMARY KEY IDENTITY, FK_city_id INT REFERENCES city(PK_city_id), [name] NVARCHAR(30) NOT NULL, phone NVARCHAR(12) NOT NULL
					   );
CREATE TABLE orders (
						PK_order_id INT PRIMARY KEY IDENTITY, FK_component_type INT REFERENCES components(PK_component_id)
						ON DELETE CASCADE, FK_employee_id INT REFERENCES employees(PK_employee_id) ON DELETE CASCADE, 
						FK_customer_id INT REFERENCES customers(PK_customer_id) ON DELETE CASCADE, acceptance_date DATE NOT NULL DEFAULT(GETDATE())
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
	   ('RAM'		   ),
	   ('ECC Memory	  ');
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
								'Угол обзора по вертикали\178 °' + CHAR(0xA), '308.88', '2018');
SELECT * FROM components;

SELECT PK_component_id AS 'ID', product_type AS 'Component type', title, tech_characteristics AS 'Tech characteristics', 
	   price AS 'Price', release_date AS 'Release date'
FROM components
JOIN components_type
ON FK_type_code = PK_component_type_id;
		--TODO: Entities city, street
--*********************************************************
--/\/\/\/\/\/\/\/\/\/ END OF FILL TABLES /\/\/\/\/\/\/\/\/\
--*********************************************************