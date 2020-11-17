from enum import Enum

class MessagePack(Enum):
    # 注册
	PACK_Register = 0
	# 登陆
	PACK_Login = 1
	# 消息
	PACK_Message = 2
    # 验证
    # PACK_Validation=3