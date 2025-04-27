CREATE TABLE `User` (
    `user_id` int PRIMARY KEY AUTO_INCREMENT,
    `name` varchar(255) NOT NULL UNIQUE,
    `password` varchar(255) NOT NULL,
    `role` enum('admin', 'chef', 'employee') NOT NULL DEFAULT 'employee',
    `notification_number` int NOT NULL DEFAULT 0
);

CREATE TABLE `Menu_Item` (
    `item_id` int PRIMARY KEY AUTO_INCREMENT,
    `name` varchar(255) NOT NULL UNIQUE,
    `price` decimal(10, 2) NOT NULL,
    `description` text NOT NULL,
    `category` enum('breakfast', 'lunch', 'dinner', 'all day') NOT NULL,
    `availability` boolean NOT NULL DEFAULT true,
    `likes` int NOT NULL DEFAULT 0,
    `dislikes` int NOT NULL DEFAULT 0,
    `recommend_rating` decimal(10, 2) NOT NULL DEFAULT 0.00,
    `diet_id` int,
    `spice_level_id` int,
    `cuisine_id` int,
    `sweetTooth` boolean NOT NULL DEFAULT false,
    FOREIGN KEY (`diet_id`) REFERENCES `Diet_Category`(`diet_id`) ON DELETE CASCADE,
    FOREIGN KEY (`spice_level_id`) REFERENCES `Spice_Level`(`spice_level_id`) ON DELETE CASCADE,
    FOREIGN KEY (`cuisine_id`) REFERENCES `Cuisine_Category`(`cuisine_id`) ON DELETE CASCADE
);

CREATE TABLE `Comment` (
    `comment_id` int PRIMARY KEY AUTO_INCREMENT,
    `user_id` int NOT NULL,
    `item_id` int NOT NULL,
    `comment` text NOT NULL,
    `comment_date` date NOT NULL,
    FOREIGN KEY (`user_id`) REFERENCES `User` (`user_id`) ON DELETE CASCADE,
    FOREIGN KEY (`item_id`) REFERENCES `Menu_Item` (`item_id`) ON DELETE CASCADE
);

CREATE TABLE `Menu_Item_Sentiment` (
    `item_id` int NOT NULL,
    `sentiment` varchar(255),
    `type` enum('positive', 'negative') NOT NULL,
    PRIMARY KEY (`item_id`, `sentiment`),
    FOREIGN KEY (`item_id`) REFERENCES `Menu_Item` (`item_id`) ON DELETE CASCADE
);

CREATE TABLE `Notification` (
    `notification_id` int PRIMARY KEY AUTO_INCREMENT,
    `message` text NOT NULL,
    `notification_date` date NOT NULL
);

CREATE TABLE `Daily_Menu` (
    `menu_date` date NOT NULL,
    `item_id` int NOT NULL,
    PRIMARY KEY(`item_id`, `menu_date`),
    FOREIGN KEY (`item_id`) REFERENCES `Menu_Item`(`item_id`) ON DELETE CASCADE
);

CREATE TABLE `User_Order` (
    `order_id` int NOT NULL AUTO_INCREMENT,
    `user_id` int NOT NULL,
    `order_date` date NOT NULL,
    PRIMARY KEY(`order_id`),
    FOREIGN KEY (`user_id`) REFERENCES `User`(`user_id`) ON DELETE CASCADE
);

CREATE TABLE `Order_Item` (
    `order_id` int NOT NULL,
    `item_id` int NOT NULL,
    PRIMARY KEY(`order_id`, `item_id`),
    FOREIGN KEY (`order_id`) REFERENCES `User_Order`(`order_id`) ON DELETE CASCADE,
    FOREIGN KEY (`item_id`) REFERENCES `Menu_Item`(`item_id`) ON DELETE CASCADE
);

CREATE TABLE `Discarded_Menu_Item` (
    `item_id` int NOT NULL,
    `date_discarded` date NOT NULL,
    PRIMARY KEY (`item_id`, `date_discarded`),
    FOREIGN KEY (`item_id`) REFERENCES `Menu_Item`(`item_id`) ON DELETE CASCADE
);

CREATE TABLE `Improvement_Required` (
    `item_id` int NOT NULL,
    `improvement_date` date NOT NULL,
    PRIMARY KEY (`item_id`, `improvement_date`),
    FOREIGN KEY (`item_id`) REFERENCES `Menu_Item`(`item_id`) ON DELETE CASCADE
);

CREATE TABLE `Improvement_Comment` (
    `comment_id` int PRIMARY KEY AUTO_INCREMENT,
    `user_id` int NOT NULL,
    `item_id` int NOT NULL,
    `comment` text NOT NULL,
    `comment_date` date NOT NULL,
    FOREIGN KEY (`user_id`) REFERENCES `User`(`user_id`) ON DELETE CASCADE,
    FOREIGN KEY (`item_id`) REFERENCES `Menu_Item`(`item_id`) ON DELETE CASCADE
);

CREATE TABLE `Diet_Category` (
    `diet_id` int PRIMARY KEY AUTO_INCREMENT,
    `diet_type` varchar(255) NOT NULL UNIQUE
);

CREATE TABLE `Spice_Level` (
    `spice_level_id` int PRIMARY KEY AUTO_INCREMENT,
    `spice_level` varchar(255) NOT NULL UNIQUE
);

CREATE TABLE `Cuisine_Category` (
    `cuisine_id` int PRIMARY KEY AUTO_INCREMENT,
    `cuisine_type` varchar(255) NOT NULL UNIQUE
);

CREATE TABLE `Food_Preference` (
    `user_id` int NOT NULL,
    `diet_id` int NOT NULL,
    `spice_level_id` int NOT NULL,
    `cuisine_id` int NOT NULL,
    `sweet_tooth` boolean NOT NULL DEFAULT true,
    PRIMARY KEY (`user_id`),
    FOREIGN KEY (`user_id`) REFERENCES `User`(`user_id`) ON DELETE CASCADE,
    FOREIGN KEY (`diet_id`) REFERENCES `Diet_Category`(`diet_id`),
    FOREIGN KEY (`spice_level_id`) REFERENCES `Spice_Level`(`spice_level_id`),
    FOREIGN KEY (`cuisine_id`) REFERENCES `Cuisine_Category`(`cuisine_id`)
);

-- DELIMITER $$
--
-- -- Notification table trigger
-- CREATE TRIGGER before_insert_notification
-- BEFORE INSERT ON Notification
-- FOR EACH ROW
-- BEGIN
--     SET NEW.notification_date = CURDATE();
-- END $$
--
-- -- Daily_Menu table trigger
-- CREATE TRIGGER before_insert_daily_menu
-- BEFORE INSERT ON Daily_Menu
-- FOR EACH ROW
-- BEGIN
--     SET NEW.menu_date = CURDATE();
-- END $$
-- 
-- -- Order table trigger
-- CREATE TRIGGER before_insert_order
-- BEFORE INSERT ON `User_Order`
-- FOR EACH ROW
-- BEGIN
--     SET NEW.order_date = CURDATE();
-- END $$
--
-- -- Comment table trigger
-- CREATE TRIGGER before_insert_comment
-- BEFORE INSERT ON `Comment`
-- FOR EACH ROW
-- BEGIN
--     SET NEW.comment_date = CURDATE();
-- END $$
-- 
-- CREATE TRIGGER `before_insert_discarded_menu_item`
-- BEFORE INSERT ON `Discarded_Menu_Item`
-- FOR EACH ROW
-- BEGIN
--     SET NEW.date_discarded = CURDATE();
-- END $$
-- 
-- CREATE TRIGGER set_improvement_date
-- BEFORE INSERT ON Improvement_Required
-- FOR EACH ROW
-- BEGIN
--     SET NEW.improvement_date = CURDATE();
-- END $$
-- 
-- CREATE TRIGGER set_comment_date
-- BEFORE INSERT ON Improvement_Comment
-- FOR EACH ROW
-- BEGIN
--     SET NEW.comment_date = CURDATE();
-- END $$
--
-- DELIMITER ;
-- 
-- 
-- 
-- DELIMITER $$
--
-- CREATE TRIGGER update_notification_number
-- BEFORE INSERT ON User
-- FOR EACH ROW
-- BEGIN
--     DECLARE min_notification_id INT;
--     SELECT MIN(notification_id) INTO min_notification_id 
--     FROM Notification 
--     WHERE DATE(notification_date) = CURDATE();
--     IF min_notification_id IS NULL THEN
--         SET min_notification_id = 0; 
--     END IF;
--     SET NEW.notification_number = min_notification_id;
-- END $$
--
-- DELIMITER ;
-- 
-- 
-- 
-- 
-- DELIMITER $$
-- 
-- SET GLOBAL event_scheduler = ON;
-- DELIMITER $$
-- CREATE EVENT IF NOT EXISTS clean_old_notifications
-- ON SCHEDULE EVERY 1 DAY
-- STARTS CURRENT_TIMESTAMP
-- DO
-- BEGIN
--     DELETE FROM Notification WHERE notification_date < CURDATE() - INTERVAL 2 DAY;
-- END $$
-- 
-- CREATE EVENT IF NOT EXISTS discard_low_rated_items
-- ON SCHEDULE EVERY 1 MONTH
-- STARTS '2024-07-01 00:00:00'
-- DO
-- BEGIN
--     INSERT INTO Discarded_Menu_Item (item_id, date_discarded)
--     SELECT item_id, CURDATE()
--     FROM Menu_Item
--     WHERE recommend_rating < 2;
-- END $$
-- 
-- DELIMITER ;
-- 
-- 
-- show procedure status where db = 'cafeteria';
-- SHOW EVENTS FROM cafeteria;
-- select trigger_name, event_manipulation, event_object_table, action_statement, action_timing from information_schema.triggers where trigger_schema = 'cafeteria';