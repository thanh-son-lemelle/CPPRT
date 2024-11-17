-- Active: 1720430402366@@127.0.0.1@3306@discord
DROP DATABASE msn; 
CREATE DATABASE msn;
USE msn;

-- Table CONNEXION 
CREATE TABLE connexion (
    id INT PRIMARY KEY AUTO_INCREMENT, 
    email VARCHAR(255),
    password VARCHAR(255)
);

-- Passwords before using SHA-256 Hash Generator
-- a: a
-- b: b
-- Lucas: TPassword123!
-- Thanh: TPassword123!
-- Vanny: VPassword123!
-- Alicia: APassword123!
INSERT INTO connexion(email, password) VALUES
('a','ca978112ca1bbdcafac231b39a23dc4da786eff8147c4e72b9807785afee48bb'),
('b','3e23e8160039594a33894f6564e1b1348bbd7a0088d42c4acb73eeaed59c009d'),
('LucasM@gmail.com','ace4861b954d999648f6d25b03fbe7cd20a7e146d0ec176507582da960854836'),
('ThanhL@gmail.com','ec5ce95c505bbf09e9d744b5126f8c79c9f1ee36142357c3d8dfc8feac52b2f5'),
('VannyL@gmail.com','bb67dca615dcddee4f850961702eab2a7a313aae42dc92f9f7c11b3e4f20aa9a'),
('AliciaC@gmail.com','273e87c6d045f871974997b7b23d891fcef599a51bbd4c85c40db5356f28075d')

-- Table USER
-- id_role INT 
-- change_role BOOLEAN ?
CREATE TABLE user (
    id_user INT,
    first_name VARCHAR(255),
    last_name VARCHAR(255),
    pseudo VARCHAR(255),
    photo_profile INT,
    status_user VARCHAR(255),
    last_message DATETIME DEFAULT CURRENT_TIMESTAMP
);

INSERT INTO user(id_user, first_name, last_name, pseudo, photo_profile, status_user) VALUES
(1,'LTVA','SuperUserA','LucThaVanA',1,'Hi there !'),
(2,'LTVB','SuperUserB','LucThaVanB',1,'Hi there !'),
(3,'Lucas','Martinie','LucaRambar',2,'Hi there !'),
(4,'Thanh','Lemelle','ThanhTation',3,'Hi there !'),
(5,'Vanny','Lamorte','VanLam',4,'Hi there !'), 
(6,'Alicia','Cordial','AliciaLoo',4,'Hi there !');

-- Table MESSAGE
-- react int DEFAULT NULL
CREATE TABLE message (
    id_sender INT,
    id_receiver INT,
    time DATETIME,
    status_message ENUM('sent', 'delivered', 'read'),
    message BLOB
);

INSERT INTO message(id_sender, id_receiver, time, status_message, message) VALUES
(1, 2,'2024-11-14 12:10:20', 'sent', "Hi !"),
(2, 1,'2024-11-14 12:20:20', 'sent', "Are you ok ?"),
(1, 2,'2024-11-14 12:30:20', 'delivered', "I'm good, tanks ! What about you ?"),
(2, 1,'2024-11-14 12:40:20', 'read', "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed non risus. Suspendisse lectus tortor, dignissim sit amet, adipiscing nec, ultricies sed, dolor. Cras elementum ultrices diam. Maecenas ligula massa, varius a, semper congue, euismod non, mi. Proin porttitor, orci nec nonummy molestie, enim est eleifend mi, non fermentum diam nisl sit amet erat. Duis semper. Duis arcu massa, scelerisque vitae, consequat in, pretium a, enim. Pellentesque congue")
