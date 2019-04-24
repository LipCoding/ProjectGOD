#pragma once



#define SERVERIP "127.0.0.1"

const unsigned int MAX_BUFF_SIZE = 1024;
const unsigned int MAX_PACKET_SIZE = 255;
const unsigned int MAX_STR_SIZE = 100;
//
//const unsigned int MAX_USER = 10;
//
//const unsigned int MY_SERVER_PORT = 3500;
//

enum PACKETSTATE : unsigned char
{
	PACKETSTATE_NONE = 0,
	PACKETSTATE_LEFT = 1,
	PACKETSTATE_LOGIN_SUCCESS = 2,
	PACKETSTATE_LOGIN_ID_FAIL = 3,
	PACKETSTATE_LOGIN_PW_FAIL = 4,
	PACKETSTATE_JOIN_SUCCESS = 5,
	PACKETSTATE_JOIN_FAIL = 6
};

const unsigned char CS_PACKET_MOVE_FRONT = 0;
const unsigned char CS_PACKET_MOVE_BACK = 1;
const unsigned char CS_PACKET_CHAT = 4;
const unsigned char CS_PACKET_LOGIN = 6;
const unsigned char CS_PACKET_JOIN = 7;
const unsigned char CS_PACKET_MAINSERVER_CONNECT = 8;
const unsigned char CS_PACKET_ROTATE_X = 11;
const unsigned char CS_PACKET_ROTATE_Y = 12;
const unsigned char CS_PACKET_ROTATE_Z = 13;
const unsigned char CS_PACKET_ROTATE_INV_X = 14;
const unsigned char CS_PACKET_ROTATE_INV_Y = 15;
const unsigned char CS_PACKET_ROTATE_INV_Z = 16;

const unsigned char CS_PACKET_ATTACK = 50;
const unsigned char CS_PACKET_ATTACK_SWORD_SKILL1 = 51;

const unsigned char CS_PACKET_INVENTORY_ADD = 150;
const unsigned char CS_PACKET_INVENTORY_REMOVE = 151;
const unsigned char CS_PACKET_ROOTING_TABLE = 155;

const unsigned char SC_PACKET_MOVE = 0;
const unsigned char SC_PACKET_PUT_PLAYER = 1;
const unsigned char SC_PACKET_REMOVE_PLAYER = 2;
const unsigned char SC_PACKET_CHAT_NPC = 3;
const unsigned char SC_PACKET_CHAT_PLAYER = 4;
const unsigned char SC_PACKET_LOGIN = 6;
const unsigned char SC_PACKET_JOIN = 7;
const unsigned char SC_PACKET_CONNECT = 8;

const unsigned char SC_PACKET_ROTATE_X = 10;
const unsigned char SC_PACKET_ROTATE_Y = 11;
const unsigned char SC_PACKET_ROTATE_Z = 12;
const unsigned char SC_PACKET_ROTATE_X_CI = 13;
const unsigned char SC_PACKET_ROTATE_Y_CI = 14;
const unsigned char SC_PACKET_ROTATE_Z_CI = 15;

const unsigned char SC_PACKET_ATTACK_PLAYER = 50;
const unsigned char SC_PACKET_ATTACK_SWORD_SKILL1 = 51;
const unsigned char SC_PACKET_DIE_ANIMATION = 52;

const unsigned char SC_PACKET_COLLISION = 100;
const unsigned char SC_PACKET_INVENTORY_ADD = 150;
const unsigned char SC_PACKET_INVENTORY_REMOVE = 151;
const unsigned char SC_PACKET_ROOTING_TABLE = 155;


/*
#define SC_PACKET_POS 0
#define SC_PACKET_PUT_PLAYER 1
#define SC_PACKET_REMOVE_PLAYER 2
*/

//
//const unsigned char SC_POS = 0;
//const unsigned char SC_PUT_PLAYER = 1;
//const unsigned char SC_REMOVE_PLAYER = 2;

const unsigned int MAX_USER = 1000;
const unsigned int NPC_START = 1000;
const unsigned int MAX_NPC = 1100;
const unsigned int SERVER_PORT = 4000;

const unsigned int BOARD_WIDTH = 300;
const unsigned int BOARD_HIGHT = 300;
const unsigned int RADIUS = 100;

const unsigned char DB_LOGIN_UNKNOWN = 0x00;
const unsigned char DB_LOGIN_SUCCESS = 0x01;
const unsigned char DB_LOGIN_ID_FAIL = 0x02;
const unsigned char DB_LOGIN_PW_FAIL = 0x04;

const unsigned char DB_JOIN_UNKNOWN = 0x00;
const unsigned char DB_JOIN_SUCCESS = 0x01;
const unsigned char DB_JOIN_FAIL = 0x02;

const unsigned char DB_FETCH_UNKNOWN = 0x00;
const unsigned char DB_FETCH_NOCHARACTER = 0x01;
const unsigned char DB_FETCH_CHARACTER = 0x02;

const unsigned char DB_UPDATE_UNKNOWN = 0x00;
const unsigned char DB_UPDATE_SUCCESS = 0x01;
const unsigned char DB_UPDATE_FAIL = 0x02;

#pragma pack (push, 1)

struct packet_default
{
	unsigned short size;
	unsigned char type;
};

struct cs_packet_connect
{
	unsigned short size;
	unsigned char type;
};

struct cs_packet_up
{
	unsigned short size;
	unsigned char type;
	float dir_x;
	float dir_y;
	float dir_z;
};

struct cs_packet_rotate
{
	unsigned short size;
	unsigned char type;
};

struct cs_packet_chat
{
	unsigned short size;
	unsigned char type;
	unsigned int id;
	wchar_t message[MAX_STR_SIZE];
};

struct cs_packet_login
{
	unsigned short size;
	unsigned char type;
	unsigned int id;
	wchar_t login_id[MAX_STR_SIZE];
	wchar_t login_pw[MAX_STR_SIZE];
};

struct cs_packet_attack_player
{
	unsigned short size;
	unsigned char type;
	unsigned int id;
	unsigned targetid;
};

struct sc_packet_connect
{
	unsigned short size;
	unsigned char type;
	unsigned int id;
};

struct sc_packet_put_player
{
	unsigned short size;
	unsigned char type;
	unsigned int id;
	unsigned char objectSetType;
	float x;
	float y;
	float z;
	float angle;
};

struct sc_packet_pos
{
	unsigned short size;
	unsigned char type;
	unsigned int id;
	float x;
	float y;
	float z;
};

struct sc_packet_rotate
{
	unsigned short size;
	unsigned char type;
	unsigned int id;
	float angle;
};

struct sc_packet_animation_player
{
	unsigned short size;
	unsigned char type;
	unsigned int id;
};

struct sc_packet_remove_player
{
	unsigned short size;
	unsigned char type;
	unsigned int id;
};

struct sc_packet_collision_player
{
	unsigned short size;
	unsigned char type;
	unsigned int id;
	unsigned int taregetId;
};

struct sc_packet_login
{
	unsigned short size;
	unsigned char type;
	unsigned char state;
	unsigned int id;
	float x;
	float y;
	float z;
	wchar_t character_name[MAX_STR_SIZE];
};

struct sc_packet_attack_player
{
	unsigned short size;
	unsigned char type;
	unsigned int id;
	unsigned targetid;
	unsigned char objectSetType;
	int damage;
};

struct sc_packet_chat
{
	unsigned short size;
	unsigned char type;
	unsigned char state;
	unsigned int id;
	WCHAR message[MAX_STR_SIZE];
};

#pragma pack(pop)