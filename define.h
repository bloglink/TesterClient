/*******************************************************************************
 * Copyright [2016] < 青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version: 2.1.0.170427
 * author: zhaonanlin
 * brief: define
*******************************************************************************/
#ifndef DEFINE_H
#define DEFINE_H

#include <QApplication>

#define INI_PATH "./nandflash/global.ini"
#define SQL_PATH "./nandflash/aip.db"
#define INI_DEFAULT "aip9918.ini"

#define ADDR 6000
#define GUEST_LOGIN 1000 // 登录
#define GUEST_LOGIN_ERROR 1001 // 登录出错
#define GUEST_PUT_HEAD 1002 // 服务器获取文件信息
#define GUEST_GET_HEAD 1003 // 客户端获取文件信息
#define GUEST_DISPLAY 1004 // 显示信息
#define GUEST_DROPED 1005 // 客户端下线
#define ADMIN_LOGIN 1006 // 客户端登录
#define ADMIN_LOGIN_ERROR 1007 // 登录出错
#define ADMIN_LOGIN_SUCCESS 1008 // 登录成功
#define BUILD_TRANSMIT 1102 // 建立转发
#define BREAK_TRANSMIT 1103 // 取消转发
#define FILE_HEAD 2000 // 文件头
#define FILE_DATA 2001 // 文件内容
#define FILE_SUCCESS 2002 // 文件发送成功
#define FILE_ERROR 2003 // 文件内容错误
#define FILE_HEAD_ERROR 2009 // 文件头错误
#define SHELL_CMD 2004 // 发送系统命令
#define SHELL_DAT 2005 // 系统命令返回
#define ONLINE_DEVICES 2006 // 在线设备列表
#define HEART_BEAT 6000 // 心跳

#define CMD_JUMP 0x00 // 界面跳转
#define CMD_INIT 0x01 // 测试界面初始化
#define CMD_TEMP 0x02 // 显示温度
#define CMD_WAVE 0x03 // 更新波形
#define CMD_ITEM 0x04 // 更新测试项目
#define CMD_JUDGE 0x05 // 测试结果判定
#define CMD_DEBUG 0x06 // 调试信息显示
#define CMD_WAVE_BYTE 0x07 // 标准波形
#define CMD_WAVE_TEST 0x08 // 测试波形
#define CMD_WAVE_ITEM 0x09 // 波形标题
#define CMD_INIT_ITEM 0x0A // 添加测试项目
#define CMD_WAVE_HIDE 0x0B // 清空测试波形
#define CMD_CAN 0x10 // CAN消息
#define CMD_CHECK 0x11 // 查询板子状态
#define CMD_START 0x12 // 启动测试
#define CMD_STOP 0x13 // 停止测试
#define CMD_ALARM 0x14 // 报警
#define CMD_STATUS 0x15 // 状态查询
#define CMD_NET 0x16 // 网络已连接
#define CMD_SQL 0x17 // 数据库已连接
#define CMD_MODE 0x18 // 启动方式设置
#define CMD_ITEM_TEMP 0x19 // 临时结果显示
#define CMD_ITEM_REPLACE 0x1A
#define CMD_CODE 0x1B //

#define WIN_ID_SYS 0x00
#define WIN_ID_TEST 0x10 // 测试界面
#define WIN_ID_DCR 0x01 // 电阻界面
#define WIN_ID_MAG 0x02 // 反嵌界面
#define WIN_ID_INR 0x03 // 绝缘界面
#define WIN_ID_ACW 0x04 // 交耐界面
#define WIN_ID_DCW 0x05 // 直耐界面
#define WIN_ID_IMP 0x06 // 匝间界面
#define WIN_ID_IND 0x07 // 电感界面
#define WIN_ID_PWR 0x08 // 功率界面
#define WIN_ID_LVS 0x09 // 低启界面
#define WIN_ID_LCK 0x0A // 堵转界面
#define WIN_ID_OUT13 0x13 // 输出板
#define WIN_ID_OUT14 0x14 // 输出板
#define WIN_ID_OUT15 0x15 // 输出板
#define WIN_ID_OUT16 0x16 // 输出板
#define WIN_ID_OUT17 0x17 // 输出板
#define WIN_ID_AMP 0x18

#define CAN_ID_DCR 0x41 // 电阻板
#define CAN_ID_DCR_WAVE 0x441 // 电阻板反嵌波形
#define CAN_ID_INR 0x61 // 耐压板
#define CAN_ID_IMP 0x81 // 匝间板
#define CAN_ID_IMP_WAVE 0x481 // 匝间板波形
#define CAN_ID_IND 0xC1 // 电感板
#define CAN_ID_PWR 0xE1 // 功率板
#define CAN_ID_PG_WAVE 0x4E1 // PG波形
#define CAN_ID_13OUT 0x261 // 输出板13
#define CAN_ID_14OUT 0x281 // 输出板14
#define CAN_ID_15OUT 0x2A1 // 输出板15
#define CAN_ID_16OUT 0x2C1 // 输出板16
#define CAN_ID_17OUT 0x2E1 // 输出板17
#define CAN_ID_AMP 0x141

#endif // DEFINE_H
