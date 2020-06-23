/*
 Navicat Premium Data Transfer

 Source Server         : localhost_3305
 Source Server Type    : MySQL
 Source Server Version : 50724
 Source Host           : localhost:3305
 Source Schema         : test

 Target Server Type    : MySQL
 Target Server Version : 50724
 File Encoding         : 65001

 Date: 17/06/2020 10:58:48
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for t_friend
-- ----------------------------
DROP TABLE IF EXISTS `t_friend`;
CREATE TABLE `t_friend`  (
  `user_id` int(11) NOT NULL,
  `friend_id` int(11) NOT NULL
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of t_friend
-- ----------------------------
INSERT INTO `t_friend` VALUES (1, 2);
INSERT INTO `t_friend` VALUES (1, 5);
INSERT INTO `t_friend` VALUES (2, 1);
INSERT INTO `t_friend` VALUES (2, 3);
INSERT INTO `t_friend` VALUES (2, 4);
INSERT INTO `t_friend` VALUES (3, 2);
INSERT INTO `t_friend` VALUES (3, 4);
INSERT INTO `t_friend` VALUES (4, 2);
INSERT INTO `t_friend` VALUES (4, 3);
INSERT INTO `t_friend` VALUES (5, 1);
INSERT INTO `t_friend` VALUES (1, 3);
INSERT INTO `t_friend` VALUES (1, 4);
INSERT INTO `t_friend` VALUES (3, 1);
INSERT INTO `t_friend` VALUES (4, 1);
INSERT INTO `t_friend` VALUES (3, 5);
INSERT INTO `t_friend` VALUES (5, 3);
INSERT INTO `t_friend` VALUES (1, 6);
INSERT INTO `t_friend` VALUES (1, 7);
INSERT INTO `t_friend` VALUES (6, 1);
INSERT INTO `t_friend` VALUES (7, 1);

-- ----------------------------
-- Table structure for t_group
-- ----------------------------
DROP TABLE IF EXISTS `t_group`;
CREATE TABLE `t_group`  (
  `group_id` int(11) NOT NULL AUTO_INCREMENT,
  `group_name` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  PRIMARY KEY (`group_id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 4 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of t_group
-- ----------------------------
INSERT INTO `t_group` VALUES (1, 'study');
INSERT INTO `t_group` VALUES (2, 'game');
INSERT INTO `t_group` VALUES (3, 'music');

-- ----------------------------
-- Table structure for t_user
-- ----------------------------
DROP TABLE IF EXISTS `t_user`;
CREATE TABLE `t_user`  (
  `user_id` int(11) NOT NULL AUTO_INCREMENT,
  `username` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `password` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `state` tinyint(1) NOT NULL,
  PRIMARY KEY (`user_id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 8 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of t_user
-- ----------------------------
INSERT INTO `t_user` VALUES (1, '1', '1', 0);
INSERT INTO `t_user` VALUES (2, '2', '2', 0);
INSERT INTO `t_user` VALUES (3, '3', '3', 0);
INSERT INTO `t_user` VALUES (4, '4', '4', 0);
INSERT INTO `t_user` VALUES (5, 'ljf', 'ljf', 0);
INSERT INTO `t_user` VALUES (6, 'tom', 'tom', 0);
INSERT INTO `t_user` VALUES (7, 'anna', 'anna', 0);

-- ----------------------------
-- Table structure for t_user_group
-- ----------------------------
DROP TABLE IF EXISTS `t_user_group`;
CREATE TABLE `t_user_group`  (
  `user_id` int(11) NOT NULL,
  `group_id` int(11) NOT NULL
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of t_user_group
-- ----------------------------
INSERT INTO `t_user_group` VALUES (1, 1);
INSERT INTO `t_user_group` VALUES (1, 3);
INSERT INTO `t_user_group` VALUES (2, 2);
INSERT INTO `t_user_group` VALUES (3, 1);
INSERT INTO `t_user_group` VALUES (4, 1);
INSERT INTO `t_user_group` VALUES (5, 3);
INSERT INTO `t_user_group` VALUES (1, 2);
INSERT INTO `t_user_group` VALUES (5, 1);
INSERT INTO `t_user_group` VALUES (6, 1);
INSERT INTO `t_user_group` VALUES (7, 1);

SET FOREIGN_KEY_CHECKS = 1;
