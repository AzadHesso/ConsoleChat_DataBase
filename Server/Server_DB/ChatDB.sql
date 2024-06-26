CREATE DATABASE IF NOT EXISTS dbase
DEFAULT CHARACTER SET utf8 COLLATE utf8_unicode_ci;

USE dbase;

CREATE TABLE IF NOT EXISTS users(
    user_id INTEGER PRIMARY KEY AUTO_INCREMENT,
    name VARCHAR(50) NOT NULL,
    login VARCHAR(50) NOT NULL UNIQUE
    );

CREATE TABLE IF NOT EXISTS passwords (
    user_id INTEGER PRIMARY KEY,
    password_hash VARCHAR(32) NOT NULL,
    FOREIGN KEY (user_id) REFERENCES users(user_id)
    );

CREATE TABLE IF NOT EXISTS messages (
    message_id INTEGER PRIMARY KEY AUTO_INCREMENT,
    from_id INTEGER NOT NULL,
    to_id INTEGER NOT NULL,
    message TEXT CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL,
    FOREIGN KEY (from_id) REFERENCES users(user_id),
    FOREIGN KEY (to_id) REFERENCES users(user_id)
    ) CHARACTER SET utf8;

INSERT INTO users(name, login)  VALUES
    ('All', 'All'),
    ('Azad', 'Azo'),
    ('Danila', 'Dania'),
    ('Rayan', 'Ray');
   
-- 9998371779 хэш пароля 12345 
INSERT INTO passwords(user_id, password_hash) VALUES
    (1, '9998371779'),
    (2, '9998371779'),
    (3, '9998371779'),
    (4, '9998371779');

INSERT INTO messages(from_id, to_id, message) VALUES 
    (2, 1, 'Hello'),
    (3, 4, 'Hi, How are you!?'),
    (2, 3, 'What is your name!?'),
    (3, 2, 'How are you doing!?'),
    (2, 3, 'See you, Good bye'),
    (4, 1, 'Hello everybody!');