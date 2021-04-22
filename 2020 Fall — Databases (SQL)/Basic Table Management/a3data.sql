/*
	Create all tables
	Insert data into tables
*/


CREATE TABLE if NOT EXISTS vendor(
   Vno 			VARCHAR(50) PRIMARY KEY NOT NULL,
   VName       	VARCHAR(50) NOT NULL,
   City			VARCHAR(50) NOT NULL,
   VBalance     REAL NOT NULL
);


INSERT INTO vendor VALUES ('V1', 'IKEA', 'Toronto', 200.00), 
					('V2', 'Walmart', 'Waterloo', 671.05), 
					('V3', 'Esso', 'Windsor', 0.00), 
					('V4', 'Esso', 'Waterloo', 225.00);


CREATE TABLE if NOT EXISTS customer(
   Account 		VARCHAR(50) PRIMARY KEY NOT NULL,
   CName       	VARCHAR(50) NOT NULL,
   Province	   	VARCHAR(50) NOT NULL,
   CBalance     REAL NOT NULL,
   Crlimit		INT NOT NULL
);


INSERT INTO customer VALUES ('A1', 'Smith', 'ONT', 2515.00, 2000), 
					('A2', 'Jones', 'BC', 2014.00, 2500), 
					('A3', 'Doc', 'ONT', 150.00, 1000);


CREATE TABLE if NOT EXISTS transaction(
   Tno 			VARCHAR(50) PRIMARY KEY NOT NULL,
   Vno			VARCHAR(50) NOT NULL,
   Account		VARCHAR(50) NOT NULL,
   T_date		DATE NOT NULL,
   Amount		REAL NOT NULL
);


INSERT INTO transaction VALUES ('T1', 'V2', 'A1', '2020-07-15', 1325.00), 
					('T2', 'V2', 'A3', '2019-12-16', 1900.00), 
					('T3', 'V3', 'A1', '2020-09-01', 2500.00), 
					('T4', 'V4', 'A2', '2020-03-20', 1613.00), 
					('T5', 'V4', 'A3', '2020-07-31', 2212.00);

