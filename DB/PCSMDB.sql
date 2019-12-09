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
CREATE TABLE street (PK_street_id INT PRIMARY KEY IDENTITY, FK_city_id INT REFERENCES city(PK_city_id), [name] NVARCHAR(50) NOT NULL, house_num SMALLINT NOT NULL, porch SMALLINT NOT NULL, [floor] SMALLINT NOT NULL,
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
VALUES ('1', 'Dell SE2419H', '����������\1920x1080' + CHAR(0xA) +
							 '����������� ������\16:9' + CHAR(0xA) +
							 '���������\23.8"' + CHAR(0xA) +
							 '����������� ������������ �������� (HDR)\false' + CHAR(0xA) + 
							 '����������� ������\�������' + CHAR(0xA) + 
							 '��������� �����\false' + CHAR(0xA) + 
							 '���������� ������� �����\false' + CHAR(0xA) + 
							 'NVIDIA G-Sync\false' + CHAR(0xA) +
							 'AMD FreeSync\false' + CHAR(0xA) +
							 '�������\250 ��/�2' + CHAR(0xA) +
							 '�������������\1 000 :1' + CHAR(0xA) +
							 '����� ������� (GtG)\5 ��' + CHAR(0xA) +
							 '���� ������ �� �����������\178 �' + CHAR(0xA) +
							 '���� ������ �� ���������\178 �', '251.25', '2018'),

		('1', 'LG 24MK600M-B', '���������\23.8"' + CHAR(0xA) +
							    '����������� ������\16:9 ' + CHAR(0xA) +
								'����������\1920x1080' + CHAR(0xA) +
								'���\��������/�������' + CHAR(0xA) +
								'�������\IPS' + CHAR(0xA) +
								'����������� ������������ �������� (HDR)\false' + CHAR(0xA) + 
								'����������� ������\�������' + CHAR(0xA) + 
								'��������� �����\false' + CHAR(0xA) +
								'���������� ������� �����\false' + CHAR (0xA) +
								'NVIDIA G-Sync\false' + CHAR(0xA) +
								'AMD FreeSync\true' + CHAR(0xA) +
								'�������\250 ��/�2' + CHAR(0xA) +
								'�������������\1 000 :1' + CHAR(0xA) +
								'����� ������� (GtG)\5 ��' + CHAR(0xA) +
								'���� ������ �� �����������\178 �' + CHAR(0xA) +
								'���� ������ �� ���������\178 �' + CHAR(0xA), '308.88', '2018'),

		('2', 'Gigabyte Aorus GeForce RTX 2060 Super 8GB GDDR6', '���������\PCI Express x16 3.0' + CHAR(0xA) +
																 '������������� ������������ ����������\NVIDIA' + CHAR(0xA) +
																 '����������� ���������\GeForce RTX 2060 Super' + CHAR(0xA) +
																 '������������� ������\true' + CHAR(0xA) +
																 '������� ������������ ����������\1 470 ���' + CHAR(0xA) + 
																 'Turbo-������� ������������ ����������\1 845 ���' + CHAR(0xA) +
																 '���������� ��������� �����������\2 176' + CHAR(0xA) +
																 '�����������\8 ��' + CHAR(0xA) +
																 '��� �����������\GDDR6' + CHAR(0xA) +
																 '������� ������\3 500 ���' + CHAR(0xA) + 
																 '������ ���� ������\256 ���' + CHAR(0xA) + 
																 '��������� DirectX\12' + CHAR(0xA) +
																 '��������� SLI/CrossFire\false' + CHAR(0xA) +
																 '������� �������\6+8 pin' + CHAR(0xA) +
																 '������������� ���� �������\550 ��' + CHAR(0xA) +
																 '����������\��������' + CHAR(0xA) +
																 '������� ������� ����������\2.7 �����' + CHAR(0xA) +
																 '���������� ������������\3' + CHAR(0xA) +
																 '����� ����������\290 ��' + CHAR(0xA) + 
																 '������ ����������\134.31 ��' + CHAR(0xA) +
																 '��������������� (Low Profile)\false' + CHAR(0xA), '1050,62', '2019'),
		('3', 'AMD Ryzen 5 2600', '��������� ���\Ryzen 5'                                    + CHAR(0xA) +
																 '��� ��������\OEM'                                         + CHAR(0xA) +
																 '���������� � ���������\false'                             + CHAR(0xA) +
																 '������� �������� ���������\Pinnacle Ridge'                + CHAR(0xA) +
																 '�����\AM4'                                                + CHAR(0xA) +
																 '���������� ����\6'                                        + CHAR(0xA) +
																 '������������ ���������� �������\12'                       + CHAR(0xA) +
																 '�������� �������\3.4 ���'                                 + CHAR(0xA) +
																 '������������ Turbo-�������\3.9 ���'                       + CHAR(0xA) +
																 '��� L2\3 ��'                                              + CHAR(0xA) +
																 '��� L3\16 ��'                                             + CHAR(0xA) +
																 '��������� ������\DDR4'                                    + CHAR(0xA) +
																 '���������� ������� ������\2'                              + CHAR(0xA) +
																 '����. ������� ������\2 933 ���'                           + CHAR(0xA) +
																 '���������� ���������� PCI Express\PCI Express 3.0 x16'    + CHAR(0xA) +
																 '���������� �������\false'                                 + CHAR(0xA) +
																 '��������� �������� �������� (TDP) 65 ��'                  + CHAR(0xA) +
																 '������� �����������\12 ��'                                + CHAR(0xA) +
																 '��������������� ����\true'                                + CHAR(0xA) +
																 '������������� AMD-V\true'                                 + CHAR(0xA), '260.82', '2018'),
		('4', 'Intel Core i5-9400F', '��������� ���\Core i5'                                                          + CHAR(0xA) +
																'��� ��������\OEM'                                                               + CHAR(0xA) +
																'���������� � ���������\false'                                                   + CHAR(0xA) +
																'������� �������� ���������\Coffee Lake'                                         + CHAR(0xA) +
																'�����\LGA1151 v2'                                                               + CHAR(0xA) +
																'���������� ����\6'                                                              + CHAR(0xA) +
																'������������ ���������� �������\6'                                              + CHAR(0xA) +
																'�������� �������\2.9 ���'                                                       + CHAR(0xA) +
																'������������ Turbo-�������\4.1 ���'                                             + CHAR(0xA) +
																'��� L3\9 ��'                                                                    + CHAR(0xA) +
																'��������� ������\DDR4'                                                          + CHAR(0xA) +
																'���������� ������� ������\2'                                                    + CHAR(0xA) +
																'����. ������� ������\2 666 ���'                                                + CHAR(0xA) +
																'���������� ���������� PCI Express\PCI Express 3.0 x16/x8+x8/x8+x4+x4'           + CHAR(0xA) +
																'���������� �������\false'                                                       + CHAR(0xA) +
																'��������� �������� �������� (TDP)\65 ��'                                        + CHAR(0xA) +
																'������� �����������\14 ��'                                                      + CHAR(0xA) +
																'��������������� ����\false'                                                     + CHAR(0xA) +
																'������������� Intel VT-x\true'                                                  + CHAR(0xA) +
																'������������� Intel VT-d\true'                                                  + CHAR(0xA), '310.29', '2019'),
		('5', 'AMD Ryzen 5 3600', '��������� ���\Ryzen 5'                                              + CHAR(0xA) +
																'��� ��������\OEM'                                                   + CHAR(0xA) +
																'���������� � ���������\false'                                       + CHAR(0xA) +
																'������� �������� ���������\Matisse'                                 + CHAR(0xA) +
																'�����\AM4'                                                          + CHAR(0xA) +
																'���������� ����\6'                                                  + CHAR(0xA) +
																'������������ ���������� �������\12'                                 + CHAR(0xA) +
																'�������� �������\3.6 ���'                                           + CHAR(0xA) +
																'������������ Turbo-�������\4.2 ���'                                 + CHAR(0xA) +
																'��� L2\3 ��'                                                        + CHAR(0xA) +
																'��� L3\32 ��'                                                       + CHAR(0xA) +
																'��������� ������\DDR4'                                              + CHAR(0xA) +
																'���������� ������� ������\2'                                        + CHAR(0xA) +
																'����. ������� ������\3 200 ���'                                     + CHAR(0xA) +
																'���������� ���������� PCI Express\PCI Express 4.0 x16'              + CHAR(0xA) +
																'���������� �������\false'                                           + CHAR(0xA) +
																'��������� �������� �������� (TDP)\65 ��'                            + CHAR(0xA) +
																'������� �����������\7 ��'                                           + CHAR(0xA) +
																'��������������� ����\true '                                         + CHAR(0xA), '411.72', '2019');


INSERT INTO employees
VALUES ('John Track Lir', 'Accountant', 500, 1),
	   ('Stephan King Sir', 'Accountant', 1000, 2);


INSERT INTO city
VALUES ('�����'),
	   ('������'),
	   ('������'),
	   ('�������'),
	   ('������'),
	   ('�����');


INSERT INTO street
VALUES ('1', '�������������', '1337', '2', '3'),
	   ('6', '����������', '123', '4', '3');


INSERT INTO customers
VALUES ('1', 'Jeff', '+1234567'),
       ('2', 'G.A.', '+4522156');


SELECT * FROM components;
SELECT * FROM components_type;
SELECT * FROM employees;
SELECT * FROM city;
SELECT * FROM street;
SELECT * FROM customers;

--INSERT INTO passport
--VALUES ('John Track Lir', 'Kolesnikova 5', 'Frunzenskoe RUVD', 123);

		--TODO: Entities city, street
--*********************************************************
--/\/\/\/\/\/\/\/\/\/ END OF FILL TABLES /\/\/\/\/\/\/\/\/\
--*********************************************************